#ifndef _MADGWICK_FILTER_HPP_
#define _MADGWICK_FILTER_HPP_

#include "mbed.h"
#include "Quaternion.hpp"
#include <math.h>

#define BETA_DEF 0.1

/**
*  @bref Madgwick Filter��p���āC�p���x�E�����x�E�n���C�f�[�^�𓝍����C�p���𐄒肷�郉�C�u�����ł��D
*  @note Quaternion.hpp�𗘗p����邱�Ƃ������߂������܂��D
*/
class MadgwickFilter {

public:
    /**
        @bref   �}�h�O�E�B�b�N�t�B���^�[(�}�b�W���E�B�b�N�t�B���^�[)�N���X�̃R���X�g���N�^
        @param  B   double�^, ���̒l��傫������Əd�͂̉e����傫�����悤�ɂȂ�܂��D
        @note   ���������̏ꍇ�CB = 0.1f���������܂��D
    */
    MadgwickFilter(float B = BETA_DEF);

public:
    /**
        @bref   MadgwickFilter�ɂ���Ċp���x�E�����x�E�n���C�f�[�^�𓝍����C�p���v�Z���܂��D
        @param  gx,gy,gz    �p���x�f�[�^�C[rad]�ɕϊ����Ă������Ă��������D
        @param  ax,ay,az    �����x�f�[�^, ���ɋK�i���͕K�v����܂���
        @param  mx,my,mz    �n���C�f�[�^, �L�����u���[�V�������m���ɍs���ĉ������D
        @note   �p���x��[rad]�ɂ��Ă��������D���̊֐��͏o���邾�������ɌĂяo�������������ǂ��Ǝv���܂��D
        @note   �O���Ń��[�p�X�t�B���^�Ȃǂ������邱�Ƃ������߂��܂��D
    */
    void MadgwickAHRSupdate(float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz);

    /**
    @bref   MadgwickFilter�ɂ���Ċp���x�E�����x�E�n���C�f�[�^�𓝍����C�p���v�Z���܂��D
    @param  gyro    �p���x�f�[�^�C[rad]�ɕϊ����Ă������Ă��������D
    @param  acc    �����x�f�[�^, ���ɋK�i���͕K�v����܂���
    @param  mag    �n���C�f�[�^, �L�����u���[�V�������m���ɍs���ĉ������D
    @note   �p���x��[rad]�ɂ��Ă��������D���̊֐��͏o���邾�������ɌĂяo�������������ǂ��Ǝv���܂��D
    @note   �O���Ń��[�p�X�t�B���^�Ȃǂ������邱�Ƃ������߂��܂��D
*/
    void MadgwickAHRSupdate(float* gyro, float* acc, float* mag);

    /**
        @bref   MadgwickFilter���p���x�Ɖ����x�݂̂œ������C�p���v�Z���X�V���܂��D
        @param  gx,gy,gz    �p���x�f�[�^�C[rad]�ɕϊ����Ă������Ă��������D
        @param  ax,ay,az    �����x�f�[�^, ���ɋK�i���͕K�v����܂���
        @note   �ʏ�̊֐��ł��C�n���C������0.0�ɂ���΂��̊֐����Ă΂�܂��D
    */
    void MadgwickAHRSupdateIMU(float gx, float gy, float gz, float ax, float ay, float az);

    /**
        @bref   �p�����l�����Ŏ擾���܂��D
        @param  Q   �N�H�[�^�j�I���N���X�̃C���X�^���X�A�h���X, w�Ei�Ej�Ek���X�V���܂��D
        @note   unity�ɓ����ۂ͎��������C�����Ă��������D
    */
    void getAttitude(Quaternion* Q);

    /**
        @bref   �p�����l�����Ŏ擾���܂��D
        @param  _q0 ����w, double�^, �A�h���X
        @param  _q1 ����i, double�^, �A�h���X
        @param  _q2 ����j, double�^, �A�h���X
        @param  _q3 ����k, double�^, �A�h���X
        @note   unity�ɓ����ۂ͎��������C�����Ă��������D
    */
    void getAttitude(float* _q0, float* _q1, float* _q2, float* _q3);

    /**
    @bref   �I�C���[�p�Ŏp�����擾���܂��D
    @param  val ���[���C�s�b�`�C���[�̏��ɔz��Ɋi�[���܂��D�R�ȏ�̗v�f�̔z������Ă��������D
    @note   �l��[rad]�ł��D[degree]�ɕϊ����K�v�ȏꍇ�͕ʓr�v�Z���ĉ������D
    */
    void getEulerAngle(float* val);
public:
    Timer madgwickTimer;
    Quaternion q;
    float q0, q1, q2, q3;
    float beta;

private:
    float result1, result2, result3, result4, result5;//�����̌v�Z�ɑ�����ď������y�����邽�߂̂��
};

MadgwickFilter::MadgwickFilter(float B) {
    q.w = 1.0f;
    q.x = 0.0f;
    q.y = 0.0f;
    q.z = 0.0f;
    beta = B;
    q0 = 1.0f;
    q1 = 0.0f;
    q2 = 0.0f;
    q3 = 0.0f;
    madgwickTimer.start();
}

void MadgwickFilter::getAttitude(Quaternion* Q) {
    *Q = q;
    return;
}



void MadgwickFilter::getAttitude(float* _q0, float* _q1, float* _q2, float* _q3) {
    *_q0 = q0;
    *_q1 = q1;
    *_q2 = q2;
    *_q3 = q3;
    return;
}


void MadgwickFilter::getEulerAngle(float val[3]) {
    float q0q0 = q0 * q0;
    float q1q1q2q2 = q1 * q1 - q2 * q2;
    float q3q3 = q3 * q3;

    result1 = 2 * (q0 * q1 + q2 * q3);
    result2 = q0q0 - q1q1q2q2 + q3q3;
    result3 = 2 * (q1 * q3 - q0 * q2);
    result4 = 2 * (q1 * q2 + q0 * q3);
    result5 = q0q0 + q1q1q2q2 - q3q3;

    val[0] = atan2(result1, result2);
    val[1] = -1 * asin(result3);
    val[2] = atan2(result4, result5);
}
//---------------------------------------------------------------------------------------------------
// AHRS algorithm update

inline void MadgwickFilter::MadgwickAHRSupdate(float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz) {

    static float acc_norm;
    static float deltaT = 0.0f;
    static unsigned int newTime = 0, oldTime = 0;
    static float recipNorm;
    static float s0, s1, s2, s3;
    static float qDot1, qDot2, qDot3, qDot4;
    static float hx, hy;
    static float _2q0mx, _2q0my, _2q0mz, _2q1mx, _2bx, _2bz, _4bx, _4bz, _2q0, _2q1, _2q2, _2q3, _2q0q2, _2q2q3, q0q0, q0q1, q0q2, q0q3, q1q1, q1q2, q1q3, q2q2, q2q3, q3q3;

    // Use IMU algorithm if magnetometer measurement invalid (avoids NaN in magnetometer normalisation)
    if ((mx == 0.0f) && (my == 0.0f) && (mz == 0.0f)) {
        MadgwickAHRSupdateIMU(gx, gy, gz, ax, ay, az);
        return;
    }

    // Rate of change of quaternion from gyroscope
    qDot1 = 0.5f * (-q1 * gx - q2 * gy - q3 * gz);
    qDot2 = 0.5f * (q0 * gx + q2 * gz - q3 * gy);
    qDot3 = 0.5f * (q0 * gy - q1 * gz + q3 * gx);
    qDot4 = 0.5f * (q0 * gz + q1 * gy - q2 * gx);

    // Compute feedback only if accelerometer measurement valid (avoids NaN in accelerometer normalisation)
    if (!((ax == 0.0f) && (ay == 0.0f) && (az == 0.0f))) {

        // Normalise accelerometer measurement
        acc_norm = sqrt(ax * ax + ay * ay + az * az);
        recipNorm = 1.0f / acc_norm;
        ax *= recipNorm;
        ay *= recipNorm;
        az *= recipNorm;

        // Normalise magnetometer measurement
        recipNorm = 1.0f / sqrt(mx * mx + my * my + mz * mz);
        mx *= recipNorm;
        my *= recipNorm;
        mz *= recipNorm;

        // Auxiliary variables to avoid repeated arithmetic
        _2q0mx = 2.0f * q0 * mx;
        _2q0my = 2.0f * q0 * my;
        _2q0mz = 2.0f * q0 * mz;
        _2q1mx = 2.0f * q1 * mx;
        _2q0 = 2.0f * q0;
        _2q1 = 2.0f * q1;
        _2q2 = 2.0f * q2;
        _2q3 = 2.0f * q3;
        _2q0q2 = 2.0f * q0 * q2;
        _2q2q3 = 2.0f * q2 * q3;
        q0q0 = q0 * q0;
        q0q1 = q0 * q1;
        q0q2 = q0 * q2;
        q0q3 = q0 * q3;
        q1q1 = q1 * q1;
        q1q2 = q1 * q2;
        q1q3 = q1 * q3;
        q2q2 = q2 * q2;
        q2q3 = q2 * q3;
        q3q3 = q3 * q3;

        // Reference direction of Earth's magnetic field
        hx = mx * q0q0 - _2q0my * q3 + _2q0mz * q2 + mx * q1q1 + _2q1 * my * q2 + _2q1 * mz * q3 - mx * q2q2 - mx * q3q3;
        hy = _2q0mx * q3 + my * q0q0 - _2q0mz * q1 + _2q1mx * q2 - my * q1q1 + my * q2q2 + _2q2 * mz * q3 - my * q3q3;
        _2bx = sqrt(hx * hx + hy * hy);
        _2bz = -_2q0mx * q2 + _2q0my * q1 + mz * q0q0 + _2q1mx * q3 - mz * q1q1 + _2q2 * my * q3 - mz * q2q2 + mz * q3q3;
        _4bx = 2.0f * _2bx;
        _4bz = 2.0f * _2bz;

        // Gradient decent algorithm corrective step
        s0 = -_2q2 * (2.0f * q1q3 - _2q0q2 - ax) + _2q1 * (2.0f * q0q1 + _2q2q3 - ay) - _2bz * q2 * (_2bx * (0.5f - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - mx) + (-_2bx * q3 + _2bz * q1) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - my) + _2bx * q2 * (_2bx * (q0q2 + q1q3) + _2bz * (0.5f - q1q1 - q2q2) - mz);
        s1 = _2q3 * (2.0f * q1q3 - _2q0q2 - ax) + _2q0 * (2.0f * q0q1 + _2q2q3 - ay) - 4.0f * q1 * (1 - 2.0f * q1q1 - 2.0f * q2q2 - az) + _2bz * q3 * (_2bx * (0.5f - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - mx) + (_2bx * q2 + _2bz * q0) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - my) + (_2bx * q3 - _4bz * q1) * (_2bx * (q0q2 + q1q3) + _2bz * (0.5f - q1q1 - q2q2) - mz);
        s2 = -_2q0 * (2.0f * q1q3 - _2q0q2 - ax) + _2q3 * (2.0f * q0q1 + _2q2q3 - ay) - 4.0f * q2 * (1 - 2.0f * q1q1 - 2.0f * q2q2 - az) + (-_4bx * q2 - _2bz * q0) * (_2bx * (0.5f - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - mx) + (_2bx * q1 + _2bz * q3) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - my) + (_2bx * q0 - _4bz * q2) * (_2bx * (q0q2 + q1q3) + _2bz * (0.5f - q1q1 - q2q2) - mz);
        s3 = _2q1 * (2.0f * q1q3 - _2q0q2 - ax) + _2q2 * (2.0f * q0q1 + _2q2q3 - ay) + (-_4bx * q3 + _2bz * q1) * (_2bx * (0.5f - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - mx) + (-_2bx * q0 + _2bz * q2) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - my) + _2bx * q1 * (_2bx * (q0q2 + q1q3) + _2bz * (0.5f - q1q1 - q2q2) - mz);
        recipNorm = 1.0f / sqrt(s0 * s0 + s1 * s1 + s2 * s2 + s3 * s3); // normalise step magnitude
        s0 *= recipNorm;
        s1 *= recipNorm;
        s2 *= recipNorm;
        s3 *= recipNorm;

        float deltaA = fabs(acc_norm - 1.00f);
        //beta = 0.1*exp(-1.0*deltaA*deltaA);
        //beta = 0.3*exp(-20.0*deltaA*deltaA);
        //beta = beta*exp(-30.0f*deltaA*deltaA);
        //printf("%f\r\n", beta);
        //beta = 1.0;
        //if(deltaA > 0.3)    beta = 0.0;
        // Apply feedback step
        qDot1 -= beta * s0;
        qDot2 -= beta * s1;
        qDot3 -= beta * s2;
        qDot4 -= beta * s3;
    }

    // Integrate rate of change of quaternion to yield quaternion
    newTime = (unsigned int)madgwickTimer.read_us();
    deltaT = (newTime - oldTime) / 1000000.0f;
    deltaT = fabs(deltaT);
    oldTime = newTime;

    q0 += qDot1 * deltaT;//(1.0f / sampleFreq);
    q1 += qDot2 * deltaT;//(1.0f / sampleFreq);
    q2 += qDot3 * deltaT;//(1.0f / sampleFreq);
    q3 += qDot4 * deltaT;//(1.0f / sampleFreq);

    // Normalise quaternion
    recipNorm = 1.0f / sqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
    q0 *= recipNorm;
    q1 *= recipNorm;
    q2 *= recipNorm;
    q3 *= recipNorm;

    q.w = q0;
    q.x = q1;
    q.y = q2;
    q.z = q3;
}

inline void MadgwickFilter::MadgwickAHRSupdate(float* gyro, float* acc, float* mag) {

    static float gx = 0, gy = 0, gz = 0.0f, ax = 0.0f, ay = 0.0f, az = 0.0f, mx = 0.0f, my = 0.0f, mz = 0.0f;
    static float acc_norm;
    static float deltaT = 0.0f;
    static unsigned int newTime = 0, oldTime = 0;
    static float recipNorm;
    static float s0, s1, s2, s3;
    static float qDot1, qDot2, qDot3, qDot4;
    static float hx, hy;
    static float _2q0mx, _2q0my, _2q0mz, _2q1mx, _2bx, _2bz, _4bx, _4bz, _2q0, _2q1, _2q2, _2q3, _2q0q2, _2q2q3, q0q0, q0q1, q0q2, q0q3, q1q1, q1q2, q1q3, q2q2, q2q3, q3q3;

    gx = gyro[0];
    gy = gyro[1];
    gz = gyro[2];
    ax = acc[0];
    ay = acc[1];
    az = acc[2];
    mx = mag[0];
    my = mag[1];
    mz = mag[2];

    // Use IMU algorithm if magnetometer measurement invalid (avoids NaN in magnetometer normalisation)
    if ((mx == 0.0f) && (my == 0.0f) && (mz == 0.0f)) {
        MadgwickAHRSupdateIMU(gx, gy, gz, ax, ay, az);
        return;
    }

    // Rate of change of quaternion from gyroscope
    qDot1 = 0.5f * (-q1 * gx - q2 * gy - q3 * gz);
    qDot2 = 0.5f * (q0 * gx + q2 * gz - q3 * gy);
    qDot3 = 0.5f * (q0 * gy - q1 * gz + q3 * gx);
    qDot4 = 0.5f * (q0 * gz + q1 * gy - q2 * gx);

    // Compute feedback only if accelerometer measurement valid (avoids NaN in accelerometer normalisation)
    if (!((ax == 0.0f) && (ay == 0.0f) && (az == 0.0f))) {

        // Normalise accelerometer measurement
        acc_norm = sqrt(ax * ax + ay * ay + az * az);
        recipNorm = 1.0f / acc_norm;
        ax *= recipNorm;
        ay *= recipNorm;
        az *= recipNorm;

        // Normalise magnetometer measurement
        recipNorm = 1.0f / sqrt(mx * mx + my * my + mz * mz);
        mx *= recipNorm;
        my *= recipNorm;
        mz *= recipNorm;

        // Auxiliary variables to avoid repeated arithmetic
        _2q0mx = 2.0f * q0 * mx;
        _2q0my = 2.0f * q0 * my;
        _2q0mz = 2.0f * q0 * mz;
        _2q1mx = 2.0f * q1 * mx;
        _2q0 = 2.0f * q0;
        _2q1 = 2.0f * q1;
        _2q2 = 2.0f * q2;
        _2q3 = 2.0f * q3;
        _2q0q2 = 2.0f * q0 * q2;
        _2q2q3 = 2.0f * q2 * q3;
        q0q0 = q0 * q0;
        q0q1 = q0 * q1;
        q0q2 = q0 * q2;
        q0q3 = q0 * q3;
        q1q1 = q1 * q1;
        q1q2 = q1 * q2;
        q1q3 = q1 * q3;
        q2q2 = q2 * q2;
        q2q3 = q2 * q3;
        q3q3 = q3 * q3;

        // Reference direction of Earth's magnetic field
        hx = mx * q0q0 - _2q0my * q3 + _2q0mz * q2 + mx * q1q1 + _2q1 * my * q2 + _2q1 * mz * q3 - mx * q2q2 - mx * q3q3;
        hy = _2q0mx * q3 + my * q0q0 - _2q0mz * q1 + _2q1mx * q2 - my * q1q1 + my * q2q2 + _2q2 * mz * q3 - my * q3q3;
        _2bx = sqrt(hx * hx + hy * hy);
        _2bz = -_2q0mx * q2 + _2q0my * q1 + mz * q0q0 + _2q1mx * q3 - mz * q1q1 + _2q2 * my * q3 - mz * q2q2 + mz * q3q3;
        _4bx = 2.0f * _2bx;
        _4bz = 2.0f * _2bz;

        // Gradient decent algorithm corrective step
        s0 = -_2q2 * (2.0f * q1q3 - _2q0q2 - ax) + _2q1 * (2.0f * q0q1 + _2q2q3 - ay) - _2bz * q2 * (_2bx * (0.5f - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - mx) + (-_2bx * q3 + _2bz * q1) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - my) + _2bx * q2 * (_2bx * (q0q2 + q1q3) + _2bz * (0.5f - q1q1 - q2q2) - mz);
        s1 = _2q3 * (2.0f * q1q3 - _2q0q2 - ax) + _2q0 * (2.0f * q0q1 + _2q2q3 - ay) - 4.0f * q1 * (1 - 2.0f * q1q1 - 2.0f * q2q2 - az) + _2bz * q3 * (_2bx * (0.5f - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - mx) + (_2bx * q2 + _2bz * q0) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - my) + (_2bx * q3 - _4bz * q1) * (_2bx * (q0q2 + q1q3) + _2bz * (0.5f - q1q1 - q2q2) - mz);
        s2 = -_2q0 * (2.0f * q1q3 - _2q0q2 - ax) + _2q3 * (2.0f * q0q1 + _2q2q3 - ay) - 4.0f * q2 * (1 - 2.0f * q1q1 - 2.0f * q2q2 - az) + (-_4bx * q2 - _2bz * q0) * (_2bx * (0.5f - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - mx) + (_2bx * q1 + _2bz * q3) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - my) + (_2bx * q0 - _4bz * q2) * (_2bx * (q0q2 + q1q3) + _2bz * (0.5f - q1q1 - q2q2) - mz);
        s3 = _2q1 * (2.0f * q1q3 - _2q0q2 - ax) + _2q2 * (2.0f * q0q1 + _2q2q3 - ay) + (-_4bx * q3 + _2bz * q1) * (_2bx * (0.5f - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - mx) + (-_2bx * q0 + _2bz * q2) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - my) + _2bx * q1 * (_2bx * (q0q2 + q1q3) + _2bz * (0.5f - q1q1 - q2q2) - mz);
        recipNorm = 1.0f / sqrt(s0 * s0 + s1 * s1 + s2 * s2 + s3 * s3); // normalise step magnitude
        s0 *= recipNorm;
        s1 *= recipNorm;
        s2 *= recipNorm;
        s3 *= recipNorm;

        float deltaA = fabs(acc_norm - 1.00f);
        //beta = 0.1*exp(-1.0*deltaA*deltaA);
        //beta = 0.3*exp(-20.0*deltaA*deltaA);
        //beta = beta*exp(-30.0f*deltaA*deltaA);
        //printf("%f\r\n", beta);
        //beta = 1.0;
        //if(deltaA > 0.3)    beta = 0.0;
        // Apply feedback step
        qDot1 -= beta * s0;
        qDot2 -= beta * s1;
        qDot3 -= beta * s2;
        qDot4 -= beta * s3;
    }

    // Integrate rate of change of quaternion to yield quaternion
    newTime = (unsigned int)madgwickTimer.read_us();
    deltaT = (newTime - oldTime) / 1000000.0f;
    deltaT = fabs(deltaT);
    oldTime = newTime;

    q0 += qDot1 * deltaT;//(1.0f / sampleFreq);
    q1 += qDot2 * deltaT;//(1.0f / sampleFreq);
    q2 += qDot3 * deltaT;//(1.0f / sampleFreq);
    q3 += qDot4 * deltaT;//(1.0f / sampleFreq);

    // Normalise quaternion
    recipNorm = 1.0f / sqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
    q0 *= recipNorm;
    q1 *= recipNorm;
    q2 *= recipNorm;
    q3 *= recipNorm;

    q.w = q0;
    q.x = q1;
    q.y = q2;
    q.z = q3;
}

//---------------------------------------------------------------------------------------------------
// IMU algorithm update

inline void MadgwickFilter::MadgwickAHRSupdateIMU(float gx, float gy, float gz, float ax, float ay, float az) {
    static float deltaT = 0;
    static unsigned int newTime = 0, oldTime = 0;
    static float recipNorm;
    static float s0, s1, s2, s3;
    static float qDot1, qDot2, qDot3, qDot4;
    static float _2q0, _2q1, _2q2, _2q3, _4q0, _4q1, _4q2, _8q1, _8q2, q0q0, q1q1, q2q2, q3q3;
    static float acc_norm;

    // Rate of change of quaternion from gyroscope
    qDot1 = 0.5f * (-q1 * gx - q2 * gy - q3 * gz);
    qDot2 = 0.5f * (q0 * gx + q2 * gz - q3 * gy);
    qDot3 = 0.5f * (q0 * gy - q1 * gz + q3 * gx);
    qDot4 = 0.5f * (q0 * gz + q1 * gy - q2 * gx);

    // Compute feedback only if accelerometer measurement valid (avoids NaN in accelerometer normalisation)
    if (!((ax == 0.0f) && (ay == 0.0f) && (az == 0.0f))) {

        // Normalise accelerometer measurement
        acc_norm = sqrt(ax * ax + ay * ay + az * az);
        recipNorm = 1.0f / acc_norm;
        ax *= recipNorm;
        ay *= recipNorm;
        az *= recipNorm;

        // Auxiliary variables to avoid repeated arithmetic
        _2q0 = 2.0f * q0;
        _2q1 = 2.0f * q1;
        _2q2 = 2.0f * q2;
        _2q3 = 2.0f * q3;
        _4q0 = 4.0f * q0;
        _4q1 = 4.0f * q1;
        _4q2 = 4.0f * q2;
        _8q1 = 8.0f * q1;
        _8q2 = 8.0f * q2;
        q0q0 = q0 * q0;
        q1q1 = q1 * q1;
        q2q2 = q2 * q2;
        q3q3 = q3 * q3;

        // Gradient decent algorithm corrective step
        s0 = _4q0 * q2q2 + _2q2 * ax + _4q0 * q1q1 - _2q1 * ay;
        s1 = _4q1 * q3q3 - _2q3 * ax + 4.0f * q0q0 * q1 - _2q0 * ay - _4q1 + _8q1 * q1q1 + _8q1 * q2q2 + _4q1 * az;
        s2 = 4.0f * q0q0 * q2 + _2q0 * ax + _4q2 * q3q3 - _2q3 * ay - _4q2 + _8q2 * q1q1 + _8q2 * q2q2 + _4q2 * az;
        s3 = 4.0f * q1q1 * q3 - _2q1 * ax + 4.0f * q2q2 * q3 - _2q2 * ay;
        recipNorm = 1.0f / sqrt(s0 * s0 + s1 * s1 + s2 * s2 + s3 * s3); // normalise step magnitude
        s0 *= recipNorm;
        s1 *= recipNorm;
        s2 *= recipNorm;
        s3 *= recipNorm;

        // Apply feedback step
        static float deltaA;
        deltaA = fabs(acc_norm - 1.00f);
        //beta = 0.5*exp(-20.0*deltaA*deltaA);
        if (deltaA > 0.3f) beta = 0.0f;
        else    beta = 0.1f;
        qDot1 -= beta * s0;
        qDot2 -= beta * s1;
        qDot3 -= beta * s2;
        qDot4 -= beta * s3;
    }

    // Integrate rate of change of quaternion to yield quaternion
    newTime = (unsigned int)madgwickTimer.read_us();
    deltaT = (newTime - oldTime) / 1000000.0f;
    deltaT = fabs(deltaT);
    oldTime = newTime;
    q0 += qDot1 * deltaT;;
    q1 += qDot2 * deltaT;;
    q2 += qDot3 * deltaT;;
    q3 += qDot4 * deltaT;;

    // Normalise quaternion
    recipNorm = 1.0f / sqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
    q0 *= recipNorm;
    q1 *= recipNorm;
    q2 *= recipNorm;
    q3 *= recipNorm;

    q.w = q0;
    q.x = q1;
    q.y = q2;
    q.z = q3;
}


#endif