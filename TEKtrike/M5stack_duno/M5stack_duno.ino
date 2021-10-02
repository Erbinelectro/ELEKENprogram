#include <M5Stack.h>
#include <Wire.h>

#define SERVO_ADDR 0x53

void Servo_write_us(uint8_t number, uint16_t us);
void Servo_write_angle(uint8_t number, uint8_t angle);
void sendSigs(int pulse[12]);
void setDefo();
void walk();

int defoSig[12] = {1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500};
//sigs[9] = 2000; sigs[10] = 2000; sigs[11] = 2000; 個の角度がデフォになっているロボットがよかった

/* R = right, L = Left , F = forward, B = backward
 * Servo0 = RF tekubi   90°のとき手首‐肘間のあれを垂直に付ける
 * Servo1 = RF hiji     90°のとき↑　　　　　　　と上腕を並行につける
 * Servo2 = RF kata     90°のとき上腕を45°ぐらい腹が輪に傾けて付ける
 * Servo3 = RB tekubi   
 * Servo4 = RB hiji
 * Servo5 = RB kata
 * Servo6 = LF tekubi
 * Servo7 = LF hiji
 * Servo8 = LF kata
 * Servo9 = LB tekubi
 * Servo10 = LB hiji
 * Serbo11 = LB kata
 * 
 * murinanode ikko zurasi
 * 
 * pulsewidth reange 700-2300(us)
 */
void setup() {
    M5.begin(true, false, true);
    M5.Lcd.setTextFont(4);
    M5.Lcd.setCursor(70, 100);

    M5.Lcd.printf("START");
    
    Wire.begin(21, 22, 100000);

    sendSigs(defoSig, 2);
    delay(2000);
}

void loop() {
  /*for(uint8_t i = 0; i < 12; i++){
    Servo_write_us(i, 700);
    // Servo_write_angle(i, 0);
    Serial.println("angle = 0");
  }
  delay(5000);

  
  for(uint8_t i = 0; i < 12; i++){
    Servo_write_us(i, 2300);
    // Servo_write_angle(i, 180);
    Serial.println("angle = 180");
  }
  delay(5000);*/
  if(Serial.available()){
    char _data = Serial.read();
    if(_data == 'A'){
      Serial.println("A");
      setDefo();
      delay(1000);
    }
    else if(_data == 'B' ){
      Serial.println("B");
      walk();
      delay(1000);
    }
  }
}

// number means the number of servo motor
// addr 0x01 means "control the number 1 servo by us"
void Servo_write_us(uint8_t number, uint16_t us) {
    Wire.beginTransmission(SERVO_ADDR);
    Wire.write(0x00 | number);
    Wire.write(us & 0x00ff);
    Wire.write(us >> 8 & 0x00ff);
    Wire.endTransmission();
}

// addr 0x11 means "control the number 1 servo by angle"
void Servo_write_angle(uint8_t number, uint8_t angle) {
    Wire.beginTransmission(SERVO_ADDR);
    Wire.write(0x10 | number);
    Wire.write(angle);
    Wire.endTransmission();
}

void sendSigs(int pulse[12], int controlMode){
  if (controlMode == 1) {
    for(uint8_t i = 0; i < 12; i++){
      Servo_write_angle(i, pulse[i]);
    }
  }else {
    for(uint8_t i = 0; i < 12; i++){
      Servo_write_us(i, pulse[i]);
    }
  }
  
}

void setDefo(){
  sendSigs(defoSig, 2);
}
 
void walk(){
  int sigs[12];
  for(int i = 0; i < 12; i++){
    sigs[i] = defoSig[i];
  }

  //RFとLBを上げる
  sigs[0] = 2000; sigs[1] = 2300; sigs[2] = 2300; sigs[9] = 2300; sigs[10] = 2300; sigs[11] = 1000;
  sendSigs(sigs, 2);
  delay(2000);

  //RBとLFを後ろに動かす
  sigs[3] = 1800; sigs[5] = 1200; sigs[6] = 1600; sigs[8] = 1600;
  sendSigs(sigs,2);
  delay(2000);

  //RFとLBを前に出しながら下げる
  sigs[0]= 1900; sigs[1] = 1700; sigs[2] = 1900; sigs[9] = 1900; sigs[10] = 1900; sigs[11] = 800;
  sendSigs(sigs, 2);
  delay(2000);
  
  //RBとLFを前に出しつつ上げる
  sigs[3] = defoSig[3]; sigs[5] = defoSig[5]; sigs[6] = defoSig[6]; sigs[8] = defoSig[8];
  sendSigs(sigs,2);
  delay(10);
  sigs[3] = 2000; sigs[4] = 2300; sigs[5] = 2300; sigs[6] = 2300; sigs[7] = 2300; sigs[8] = 1000;
  sendSigs(sigs,2);
  delay(990);

  //RFとLBを後ろに下げる
  sigs[0] = 1800; sigs[2] = 1200; sigs[9] = 1600; sigs[11] = 1600;
  sendSigs(sigs,2);
  delay(1000);

  //RBとLFを下ろす
  sigs[3] = defoSig[3]; sigs[4] = defoSig[4]; sigs[5] = defoSig[5]; sigs[6] = defoSig[6]; sigs[7] = defoSig[7]; sigs[8] = defoSig[8];
  sendSigs(sigs, 2);
  delay(1000);
}
