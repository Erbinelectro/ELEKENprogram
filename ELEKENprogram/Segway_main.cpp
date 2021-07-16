#include "mbed.h"

PwmOut Rin1(p25);//âÒì]ÉXÉsÅ[Éh
DigitalOut Rin2(p19);//ê≥ì]/îΩì]
PwmOut Lin1(p26);
DigitalOut Lin2(p20);

Serial pc(USBTX, USBRX);

AnalogIn direction(p16);
mpu9250 sensa(i2c, AD0_LOW)
I2C i2c(p28, p27);

//for 9jiku
const double PI = 3.1415926535897932384626433832795f;

const double ACCE_LPF_COEF = 0.9;
const double GYRO_LPF_COEF = 0.8;

double O_Gyro[3], O_Acce[3], O_Magn[3];
short int i = 0;

double gyroLPF[3];
double acceLPF[3];

double Gyro[2][3] = { 0 };
double Acce[2][3] = { 0 };

//for motor
float speed;
const float base_LRlevel = 0.5;
float LRlevel;

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
    wait_ms(10);
}

void start() {
    i2c.frequency(400000);

    sensa.setAccLPF(_460HZ);
    sensa.setGyro(_1000DPS);
    sensa.setAcc(_16G);

    pc.printf("\r\nlearning\r\n");
    learnOffsetValue();
    sensa.setOffset(O_Gyro[0], O_Gyro[1], O_Gyro[2],
        O_Acce[0], O_Acce[1], O_Acce[2],
        O_Magn[0], O_Magn[1], O_Magn[2]);
    pc.printf("\r\nlearned\r\n");
}

void culculateDegree(MadgwickFilter* mwf, Quaternion* q, float* w, float* x, float* y, float* z) {
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
}

int main() {
    
    start();

    Rin1 = 0.0;
    Lin1 = 0.0;
    wait_ms(500);
    Rin1 = 0.5;
    Lin1 = 0.5;

    while (1) {
        char data = i2c.getc();

        LRlevel = direction - base_LRlevel;
        Rin1 = speed + LRlevel;
        Lin1 = speed - LRlevel;

        if (data == 'w') {
            pc.printf("%c\r\n", data);
            Rin2 = 1;
            Lin2 = 1;
        }
        if (data == 's') {
            pc.printf("%c\r\n", data);
            Rin2 = 0;
            Lin2 = 0;
        }
    }
}

