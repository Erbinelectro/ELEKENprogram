#define M5STACK_MPU6886 

#include <M5Stack.h>
#include <cmath>

/********** For PWM **********/
const int PWM_pin_L = 2;
const int PWM_pin_R = 5;

const int PWM_CH_L = 0;
const int PWM_CH_R = 1;
const int PWM_Hz = 50;
const uint8_t PWM_level = 12;
//PWM resolution max1023?

const int CompensatePram = 10;

const int Uprate = 1;
const int Deforate = pow(2, PWM_level) / 20;
int rotateSpeed = 20;

signed int UDcount = 0;
/****************************/

/********** For I/O **********/
const int IO_pin_L = 16;
const int IO_pin_R = 17;


/********** For IMU **********/
const double RTD = 57.324f;//RAD_TO_DEG
const double DTR = 0.01745329251994329576923690768489f;//RAD_TO_DEG;

float accX, accY, accZ;
float gyroX, gyroY, gyroZ;
float pitch, roll, yaw;
float my_pitch, my_roll, my_yaw;
float default_pitch, default_roll, default_yaw;
float delta_pitch, delta_roll, delta_yaw;
float Temp;
/*******************************/

//prottype declear
void startin();
void setHoveringPos(char _command);
void Compensator(float roll, float yaw);

void setup() {
  M5.begin();
  M5.Power.begin();

  M5.IMU.Init();

  M5.Lcd.setTextSize(2);
  M5.Lcd.setTextColor(GREEN , BLACK);
  M5.Lcd.fillScreen(BLACK);
  
  m5.Lcd.setCursor(100, 100);
  m5.Lcd.printf("M5 boot\r\n");

  ledcSetup(PWM_CH_L, PWM_Hz, PWM_level);
  ledcSetup(PWM_CH_R, PWM_Hz, PWM_level);
  
  ledcAttachPin(PWM_pin_L, PWM_CH_L);
  ledcAttachPin(PWM_pin_R, PWM_CH_R);
  
  pinMode(PWM_pin_L, OUTPUT);
  pinMode(PWM_pin_R, OUTPUT);

  pinMode(IO_pin_L, OUTPUT);
  pinMode(IO_pin_R, OUTPUT);
  
  delay(1000);
  m5.Lcd.clear(WHITE);
  M5.Lcd.setTextColor(GREEN , WHITE);

  startin();
  m5.Lcd.printf("startin\r\n");
}

void loop() {
  M5.IMU.getGyroData(&gyroX,&gyroY,&gyroZ);
  M5.IMU.getAccelData(&accX,&accY,&accZ);
  M5.IMU.getAhrsData(&pitch,&roll,&yaw);

  float my_roll = atan(accY / accZ) * RAD_TO_DEG;
  float my_pitch = atan(-accX / sqrtf(accY*accY + accZ*accZ)) * RAD_TO_DEG;

  M5.Lcd.setCursor(0, 100);
  delta_pitch = (my_pitch - default_pitch) * DTR;
  delta_roll = (my_roll - default_roll) * DTR;
  M5.Lcd.printf("dP = %5.1f, dR = %5.1f", delta_pitch, delta_roll);
  //Serial.printf("deltaP = %5.1f, deltaR = %5.1f\r\n", pow(delta_roll, 2), pow(delta_roll, 2));
  
  if(delta_pitch < 0){
    digitalWrite(IO_pin_L, HIGH);
    digitalWrite(IO_pin_R, LOW);
  }
  else if(delta_pitch >= 0){
    digitalWrite(IO_pin_L, LOW);
    digitalWrite(IO_pin_R, HIGH);
  }

  ledcWrite(PWM_CH_L, (int)(abs(delta_roll) / 17 * 10 * 1000));
  ledcWrite(PWM_CH_R, (int)(abs(delta_roll) / 17 * 10 * 1000));
  
  delay(1);
}

void startin(){
  Serial.print("Set default XYZ\r\n");
  M5.IMU.getGyroData(&gyroX,&gyroY,&gyroZ);
  M5.IMU.getAccelData(&accX,&accY,&accZ);
  M5.IMU.getAhrsData(&pitch,&roll,&yaw);

  default_roll = atan(accY / accZ) * RAD_TO_DEG;
  default_pitch = atan(-accX / sqrtf(accY*accY + accZ*accZ)) * RAD_TO_DEG;
  
  Serial.print("ESC boot\r\n");
  ledcWrite(PWM_CH_L, 0);
  ledcWrite(PWM_CH_R, 0);
  delay(500);
    
  Serial.print("Motor start\r\n");
  ledcWrite(PWM_CH_L, 230);
  ledcWrite(PWM_CH_R, 230);//1dato over current
  delay(1000);

  ledcWrite(PWM_CH_L, Deforate);
  ledcWrite(PWM_CH_R, Deforate);

  m5.Lcd.clear(WHITE);
  //M5.Lcd.fillScreen(WHITE);
}

void setHoveringPos(char _command){
  Serial.printf("%c\r\n", _command);
  if(_command == 'w'){
    UDcount ++;
    rotateSpeed = Deforate + Uprate * UDcount;
    
    ledcWrite(PWM_CH_L, rotateSpeed);
    ledcWrite(PWM_CH_R, rotateSpeed);
    Serial.printf("w %d\r\n", rotateSpeed);
  }
  else if(_command == 's'){
    UDcount --;
    rotateSpeed = Deforate + Uprate * UDcount;
    
    ledcWrite(PWM_CH_L, rotateSpeed);
    ledcWrite(PWM_CH_R, rotateSpeed);
    Serial.printf("s %d\r\n", rotateSpeed);
  }
  else if(_command == 'r'){
    UDcount = 0;
    startin();
  }
  else if(_command == 'e'){
    ledcWrite(PWM_CH_L, 0);
    ledcWrite(PWM_CH_R, 0);
  }
}

void Compensator(float _roll, float _yaw, int _rotateSpeed){
  ledcWrite(PWM_CH_L, rotateSpeed + _roll * CompensatePram);
  ledcWrite(PWM_CH_R, rotateSpeed - _roll * CompensatePram);
}
