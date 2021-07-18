#include "mbed.h"
#include "mpu9250.h"
#include "MadgwickAHRS.h"

Serial pc(USBTX, USBRX);
Timer t;
MPU9250 IMU(p28, p27);
Madgwick filter;  // �p�����v�Z����I�u�W�F�N�g�����

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

    IMU.initMPU9250();  // MPU9250������������
    IMU.calibrateMPU9250(IMU.gyroBias, IMU.accelBias);  // �L�����u���[�g����

    filter.begin(10);  // 10Hz  filter������������

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

            IMU.readAccelData(IMU.accelCount);  // �����x�̐��f�[�^�[���擾����
            IMU.getAres();  // �X�P�[���l���擾����

            // x/y/z���̉����x�l���v�Z����
            IMU.ax = (float)IMU.accelCount[0] * IMU.aRes - IMU.accelBias[0];
            IMU.ay = (float)IMU.accelCount[1] * IMU.aRes - IMU.accelBias[1];
            IMU.az = (float)IMU.accelCount[2] * IMU.aRes - IMU.accelBias[2];

            IMU.readGyroData(IMU.gyroCount);  // �W���C���̐��f�[�^�[���擾����
            IMU.getGres();  // �X�P�[���l���擾����

            // x/y/z���̃W���C���l���v�Z����
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