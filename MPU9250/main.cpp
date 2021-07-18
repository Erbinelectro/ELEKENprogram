#include "mbed.h"
#include "mpu9250.h"
#include "MadgwickAHRS.h"

Serial pc(USBTX, USBRX);
Timer t;
MPU9250 IMU(p28, p27);
Madgwick filter;  // 姿勢を計算するオブジェクトを作る

unsigned long microsPerReading, microsPrevious;

void setup() {
    pc.baud(115200);

    IMU.timer = &t;

    pc.printf("Timer start\r\n");
    t.start();

    if ((int)IMU.readByte(MPU9250_ADDRESS, WHO_AM_I_MPU9250) != 0x71) {
        pc.printf("cannnot find MPU9250\r\n");
        while (true);
    }
    else {
        pc.printf("find MPU9250\r\n");
    }

    IMU.initMPU9250();  // MPU9250を初期化する
    IMU.calibrateMPU9250(IMU.gyroBias, IMU.accelBias);  // キャリブレートする

    filter.begin(10);  // 10Hz  filterを初期化する

    microsPerReading = 1000000 / 10;
    microsPrevious = t.read_us();
}

#define INTERVAL 5
float rolls[INTERVAL], pitchs[INTERVAL], yaws[INTERVAL];
int maidx = 0; // moving average index
bool once = true;

int main() {

    setup();

    while (1) {
        if (t.read_us() - microsPrevious >= microsPerReading)
        {
            while (!(IMU.readByte(MPU9250_ADDRESS, INT_STATUS) & 0x01));

            IMU.readAccelData(IMU.accelCount);  // 加速度の生データーを取得する
            IMU.getAres();  // スケール値を取得する

            // x/y/z軸の加速度値を計算する
            IMU.ax = (float)IMU.accelCount[0] * IMU.aRes - IMU.accelBias[0];
            IMU.ay = (float)IMU.accelCount[1] * IMU.aRes - IMU.accelBias[1];
            IMU.az = (float)IMU.accelCount[2] * IMU.aRes - IMU.accelBias[2];

            IMU.readGyroData(IMU.gyroCount);  // ジャイロの生データーを取得する
            IMU.getGres();  // スケール値を取得する

            // x/y/z軸のジャイロ値を計算する
            IMU.gx = (float)IMU.gyroCount[0] * IMU.gRes;
            IMU.gy = (float)IMU.gyroCount[1] * IMU.gRes;
            IMU.gz = (float)IMU.gyroCount[2] * IMU.gRes;

            pc.printf("%6.2f %6.2f %6.2f\r\n", IMU.ax, IMU.ay, IMU.az);

            pc.printf("%6.2f %6.2f %6.2f", IMU.gx, IMU.gy, IMU.gz);

            filter.updateIMU(IMU.gx, IMU.gy, IMU.gz, IMU.ax, IMU.ay, IMU.az);

            rolls[maidx % INTERVAL] = filter.getRoll();
            pitchs[maidx % INTERVAL] = filter.getPitch();
            yaws[maidx % INTERVAL] = filter.getYaw();
            maidx++;

            float roll = 0.0;
            float pitch = 0.0;
            float yaw = 0.0;
            for (int i = 0; i < INTERVAL; i++)
            {
                roll += rolls[i];
                pitch += pitchs[i];
                yaw += yaws[i];
            }
            roll /= INTERVAL;
            pitch /= INTERVAL;
            yaw /= INTERVAL;

            pc.printf("%6.2f %6.2f %6.2f\r\m", roll, pitch, yaw);

            microsPrevious = microsPrevious + microsPerReading;
        }
    }
}