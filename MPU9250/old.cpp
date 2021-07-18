#include "mbed.h"
#include "mpu9250_i2c.h"
#include "MadgwickFilter.hpp"

I2C i2c(p9, p10);
mpu9250 sensa(i2c, AD0_LOW);
Serial pc(USBTX, USBRX, 921600);
BusOut o(LED1, LED2, LED3, LED4);

//variables
const double PI = 3.1415926535897932384626433832795f;

const double ACCE_LPF_COEF = 0.9;
const double GYRO_LPF_COEF = 0.8;

double O_Gyro[3], O_Acce[3], O_Magn[3];
uint8_t i = 0;

void learnOffsetValue() {
    double Gyro[3];
    double Acce[3];

    while (i < 100) {
        ++i;
        sensa.getGyro(Gyro);
        sensa.getAcc(Acce);

        for (int cnt = 0; cnt < 3; cnt++) {
            O_Gyro[cnt] += Gyro[cnt];
            O_Acce[cnt] += Acce[cnt];
        }
        wait_us(100);
    }
    for (int cnt = 0; cnt < 3; cnt++) {
        O_Gyro[cnt] /= 100;
        O_Acce[cnt] /= 100;
    }
    wait_ms(1);
}

void start() {
    i2c.frequency(400000);

    Timer* t = new Timer;
    t->start();
    o = 15 * sensa.senserTest();
    pc.printf("%d\r\n", t->read_us());
    t->stop();
    delete t;

    sensa.setAccLPF(_460HZ);
    sensa.setGyro(_1000DPS);
    sensa.setAcc(_16G);

    pc.printf("\r\nlearning\r\n");
    learnOffsetValue();
    sensa.setOffset(O_Gyro[0], O_Gyro[1], O_Gyro[2],
        O_Acce[0], O_Acce[1], O_Acce[2],
        O_Magn[0], O_Magn[1], O_Magn[2]);
    //printf("\r\nlearned\r\n");
}

int main() {
    double gyroLPF[3];
    double acceLPF[3];

    double Gyro[2][3] = { 0 };
    double Acce[2][3] = { 0 };

    start();
    pc.printf("start\r\n");

    MadgwickFilter mwf(0.05);
    Quaternion q;

    while (1) {
        sensa.getGyro(Gyro[1]);//maybe get as euler
        sensa.getAcc(Acce[1]);

        for (int c = 0; c < 3; c++) {
            gyroLPF[c] = GYRO_LPF_COEF * Gyro[0][c] + (1 - GYRO_LPF_COEF) * Gyro[1][c];
            acceLPF[c] = ACCE_LPF_COEF * Acce[0][c] + (1 - ACCE_LPF_COEF) * Acce[1][c];

            Gyro[0][c] = Gyro[1][c];
            Acce[0][c] = Acce[1][c];
        }

        //pc.printf("%f, %f, %f\r\n", Jyro[0], Jyro[1], Jyro[2]);
        //pc.printf("%f, %f, %f\r\n", Acce[0], Acce[1], Acce[2]);
        //pc.printf("\r\n");

        mwf.MadgwickAHRSupdateIMU(gyroLPF[0], gyroLPF[1], gyroLPF[2], acceLPF[0], acceLPF[1], acceLPF[2]);

        mwf.getAttitude(&q);

        pc.printf("x%d\r\n", (int)(q.x * 1000000));
        pc.printf("y%d\r\n", (int)(q.y * 1000000));
        pc.printf("z%d\r\n", (int)(q.z * 1000000));
        pc.printf("w%d\r\n", (int)(q.w * 1000000));

        pc.printf("%f ", q.x);
        pc.printf("%f ", q.y);
        pc.printf("%f ", q.z);
        pc.printf("%f\r\n\n", q.w);

        wait_ms(1);
    }
}