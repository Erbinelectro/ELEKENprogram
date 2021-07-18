#ifndef _MPU9250_I2C_H_
#define _MPU9250_I2C_H_


#define SLAVE_ADDR_LOW (0b1101000 << 1)//AD0 == LOW
#define SLAVE_ADDR_HIGH (0b1101001 << 1)//AD0 == HIGH
#define MAG_ADDR (0b0001100 << 1)

#define WRITE_FLAG 0b00000000
#define READ_FLAG  0b00000001
#define CONFIG 0x1A
#define GYRO_CONFIG 0x1B
#define ACCEL_CONFIG 0x1C
#define ACCEL_CONFIG2 0x1D
#define LP_ACCEL_ODR 0x1E
#define INT_PIN_CFG 0x37
#define ACCEL_XOUT_H 0x3B
#define ACCEL_XOUT_L 0x3C
#define ACCEL_YOUT_H 0x3D
#define ACCEL_YOUT_L 0x3E
#define ACCLE_ZOUT_H 0x3F
#define ACCEL_ZOUT_L 0x40
#define TEMP_OUT_H 0x41
#define TEMP_OUT_L 0x42
#define GYRO_XOUT_H 0x43
#define GYRO_XOUT_L 0x44
#define GYRO_YOUT_H 0x45
#define GYRO_YOUT_L 0x46
#define GYRO_ZOUT_H 0x47
#define GYRO_ZOUT_L 0x48
#define WHO_AM_I_MPU9250 0x75 //0x71�Ȃ炨��
#define XG_OFFSET_H 0x13
#define XG_OFFSET_L 0x14
#define YG_OFFSET_H 0x15
#define YG_OFFSET_L 0x16
#define ZG_OFFSET_H 0x17
#define ZG_OFFSET_L 0x18
#define XA_OFFSET_H 0x77
#define XA_OFFSET_L 0x78
#define YA_OFFSET_H 0x79
#define YA_OFFERT_L 0x80
#define ZA_OFFSET_H 0x81
#define ZA_OFFSET_L 0x82

#define WIA 0x00 //device ID
#define INFO 0x01
#define ST1 0x02
#define HXL 0x03//Low -> High�̏��ɒ���
#define HXH 0x04
#define HYL 0x05
#define HYH 0x06
#define HZL 0x07
#define HZH 0x08
#define ST2 0x09
#define CNTL1 0x0A
#define CNTL2 0x0B

#define ACC_LSB 0.0000610350//[G / LSB]
#define GYRO_LSB 0.007630 //[(degree / s) / LSB]
#define MAG_LSB 0.150 //[uT / LSB]

typedef enum AD0 {
    AD0_HIGH = 1,
    AD0_LOW = 0
} ad0;

typedef enum ACC_RANGE {
    _2G = 1,
    _4G = 2,
    _8G = 4,
    _16G = 8
} acc_range;

typedef enum GYRO_RANGE {
    _250DPS = 1,
    _500DPS = 2,
    _1000DPS = 4,
    _2000DPS = 8
} gyro_range;

typedef enum MAG_RATE {
    _8HZ = 0,
    _100HZ = 1
} mag_rate;

typedef enum A_BAND_WIDTH {
    NO_USE = 0b00000000,
    _460HZ = 0b00001000,
    _184HZ = 0b00001001,
    _92HZ = 0b00001010,
    _41HZ = 0b00001011,
    _20HZ = 0b00001100,
    _10HZ = 0b00001101,
    _5HZ = 0b00001110,
} a_band_width;

/**
*  @bref  mpu9250���r�I�ȒP�ɗ��p�ł���悤�ɂ������C�u����
*  @note  ���[�p�X�t�B���^�܂��̎������܂��ł��D�O���ł��ł��I
*  @author  Gaku MATSUMOTO
*/
class mpu9250
{

public:

    /**
    *  @bref  mpu9250�C���X�^���X�𐶐�����
    *  @param  _i2c  ���C���v���O�����Ő錾����I2C�C���X�^���X�̃A�h���X
    *  @param  celect  AD0�s����HIGH�Ȃ�AD0_HIGH�CLOW�Ȃ�AD0_LOW
    *  @note  �������Ȃ�����AD0_HIGH�ɂȂ�܂��D
    */
    mpu9250(I2C& _i2c, AD0 celect = AD0_HIGH);

    I2C* _nine;
public:
    void writeReg(char addr, char data);
    void writeReg(char addr, char reg, char data);
    char readReg(char addr, char reg);
    void readReg(char addr, char start_reg, char* buff, char num);

    /*!
     @bref  �����Z���T�ƒʐM���ł��Ă��邩�m�F����
     @note  true���Ԃ��Ă����琬���Cfalse�Ȃ�D�D�D
     */
    bool senserTest();

    /**
    * @fn bool mpu9250::mag_senserTest()
    * @bref �n���C�Z���T�ƒʐM���ł��Ă��邩�m�F����
    * @note true���Ԃ��Ă����琬���Cfalse�Ȃ�D�D�D
    */
    bool mag_senserTest();

    /**
     *    @bref  �����x�Z���T�̃����W��ݒ�
     *    @param  a_range _2G, _4G, _8G, _16G�̒�����I��
     *    @note  ���������Ł}4G�ɂȂ�
     */
    void setAcc(ACC_RANGE a_range = _4G);

    /**
     *    @bref  �p���x�Z���T�̃����W�ݒ�
     *    @param  g_range _250DPS, _500DPS, _1000DPS, _2000DPS�̒�����I��
     *    @note  ���������Ł}500DPS
     */
    void setGyro(GYRO_RANGE g_range = _500DPS);

    /**
     *    @bref  �n���C�Z���T�̃f�[�^���[�g�ݒ�
     *    @param  rate  _8HZ �� _100HZ��I��
     *    @note  ������8Hz�ɂ���K�v�͖����Ǝv���܂��D
     */
    void setMag(MAG_RATE rate = _100HZ);

    void init();


    /**
     *    @bref   I2C�̒ʐM���x��ύX�ł��܂��D�]���̂��Ƃ��Ȃ�����g�p���Ȃ��Ă����ł��E
     */
    void frequency(int Hz);

    /**
     *    @bref  mpu9250�̃f�W�^�����[�p�X�t�B���^�̐ݒ�
     *    @param  band  NO_USE, _460HZ, _184HZ, _92HZ, _41HZ, _20HZ, _10HZ, _5HZ����I��
     *    @note  �J�b�g�I�t���g���Ȃ̂��T���v�����[�g�Ȃ̂��悭������܂���D�������ʂ������܂���
     */
    void setAccLPF(A_BAND_WIDTH band);

    /**
     *    @bref  �[���_�̂����␳����I�t�Z�b�g�l��ݒ肷��
     *    @param  ax,ay,az    �����x�̃I�t�Z�b�g
     *    @param  gx,gy,gz    �p���x�̃I�t�Z�b�g
     *    @param  mx,my,mz    �n���C�̃I�t�Z�b�g
     *    @note  �ƂĂ��d�v�ł��D�n���C�͒���I�ɃL�����u���[�V���������Ă��������D���Ȃ݂ɁC�����̒l�͑���l��������Ă��܂��D
     */
    void setOffset(double ax, double ay, double az,
        double gx, double gy, double gz,
        double mx, double my, double mz);

    /**
     *    @bref  �����x���擾���܂��D
     *    @param  ax  x�������̉����x[G]
     *    @param  ay  y�������̉����x[G]
     *    @param  az  z�������̉����x[G]
     *    @note  �^��float �ł� double�ł��\���܂���D
     */
    template<typename T>void getAcc(T* ax, T* ay, T* az);

    /**
     *    @bref  �����x���擾���܂��D
     *    @param  acc  �e�������̉����x[G]�Cx,y,z�̏�
     *    @note  �^��float �ł� double�ł��\���܂���D
     */
    template<typename T>void getAcc(T* acc);

    /**
     *    @bref  �p���x���擾���܂��D
     *    @param  gx  x�������̊p���x[degree/s]
     *    @param  gy  y�������̊p���x[degree/s]
     *    @param  gz  z�������̊p���x[degree/s]
     *    @note  �^��float �ł� double�ł��\���܂���D
     */
    template<typename T>void getGyro(T* gx, T* gy, T* gz);

    /**
     *    @bref  �p���x���擾���܂��D
     *    @param  gyro  �e�������̊p���x[degree/s], x,y,z�̏�
     *    @note  �^��float �ł� double�ł��\���܂���D
     */
    template<typename T>void getGyro(T* gyro);

    /**
     *    @bref  �������x���擾���܂��D
     *    @param  mx  x�������̎������x[uT]
     *    @param  my  y�������̎������x[uT]
     *    @param  mz  z�������̎������x[uT]
     *    @note  �^��float �ł� double�ł��\���܂���D
     */
    template<typename T>void getMag(T* mx, T* my, T* mz);


    /**
     *    @bref  �������x���擾���܂��D
     *    @param  mag  �e�������̎������x[uT]�Cx,y,z�̏�
     *    @note  �^��float �ł� double�ł��\���܂���D
     */
    template<typename T>void getMag(T* mag);

    /**
     *    @bref  �p���x�Ɖ����x�𓯎��Ɏ擾���܂��D
     *    @param  imu �f�[�^������z��C�p���x[degree/s],�����x[G]�̏�
     *    @note  �z�񐔂�6�ȏ��
     */
    template<typename T>void getGyroAcc(T* imu);//gx,gy,gz,ax,ay,az

private:
    static char _addr;
    static double acc_coef;//coefficient
    static double gyro_coef;
    static double mag_coef;
    static double acc_offset[3];
    static double gyro_offset[3];
    static double mag_offset[3];
};



inline void mpu9250::writeReg(char addr, char data)
{
    _nine->write(addr | WRITE_FLAG, &data, 1, false);
}
inline void mpu9250::writeReg(char addr, char reg, char data)
{
    char temp[2] = { reg, data };
    _nine->write(addr | WRITE_FLAG, temp, 2, false);
}
inline char mpu9250::readReg(char addr, char reg)
{
    char buff[1];
    writeReg(addr, reg);
    _nine->read(addr | READ_FLAG, buff, 1, true);
    return buff[0];
}
inline void mpu9250::readReg(char addr, char start_reg, char* buff, char num)
{
    writeReg(addr, start_reg);
    _nine->read(addr | READ_FLAG, buff, num, true);
}

#endif