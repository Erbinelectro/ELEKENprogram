#include <M5Stack.h>
#include <Wire.h>

#define SERVO_ADDR 0x53

void Servo_write_us(uint8_t number, uint16_t us);
void Servo_write_angle(uint8_t number, uint8_t angle);
void sendSigs(int pulse[12]);
void walk();

/*
 * R = right, L = Left , F = forward, B = backward
 * Servo0 = head
 * Servo1 = RF asi
 * Servo2 = RF sune
 * Servo3 = RF futomomo
 * Servo4 = RB asi
 * Servo5 = RB suune
 * Servo6 = RB futomomo
 * Servo7 = LF asi
 * Servo8 = LF sune
 * Servo9 = LF futomomo
 * Servo10 = LB asi
 * Servo11 = LB sune
 * Serbo12 = LB futomomo
 * 
 * murinanode ikko zurasi
 * 
 * maybe max width 4095
 */
void setup() {
    M5.begin(true, false, true);
    M5.Lcd.setTextFont(4);
    M5.Lcd.setCursor(70, 100);
    M5.Lcd.print("Servo Example");

    Wire.begin(21, 22, 100000);

    delay(1000);
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

/*
 * R = right, L = Left , F = forward, B = backward
 * Servo0 = head
 * Servo1 = RF asi
 * Servo2 = RF sune
 * Servo3 = RF futomomo
 * Servo4 = RB asi
 * Servo5 = RB suune
 * Servo6 = RB futomomo
 * Servo7 = LF asi
 * Servo8 = LF sune
 * Servo9 = LF futomomo
 * Servo10 = LB asi
 * Servo11 = LB sune
 * Serbo12 = LB futomomo
 * 
 * murinanode ikko zurasi
 * 
 * maybe max width 4095
 */
 
void walk(){
  //capable value range is 700 to 2300
  int sigs[12] = {1000,1600,700,700,700,700,1000,700,700,700,700};
  sendSigs(sigs,2);
  delay(500);
}

void loop() {
  /*
  for(uint8_t i = 0; i < 12; i++){
    Servo_write_us(i, 700);
    // Servo_write_angle(i, 0);
  }
  delay(5000);

  
  for(uint8_t i = 0; i < 12; i++){
    Servo_write_us(i, 2300);
    // Servo_write_angle(i, 180);
  }
  delay(5000);
  */
  walk();
}


/*void loop() {
  Servo_write_angle(10, 90);
}*/
