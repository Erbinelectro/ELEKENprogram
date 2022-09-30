/*
  セグウェイには2つのモーターを動かせるモータードライバが必要です。
  またはモータードライバを2つ用意してください。
  このプログラムは朱雀技研で売ってるCytron 40A 10V-45V SmartDrive DCモータドライバをPWMモードで動かすことを想定しています

  前提として、M5StackやESP32系統ではarduino unoなどで使えるanalogWiteによるPWM出力ができないので、ledcWriteを使う必要があります。
  これはsetup関数の中で、ledcSetup(ch, Hz, level)によりチャンネルの周波数と分解能を設定し、
  ledcAttachPin(pin,ch)によってgpioピンとチャンネルを紐づけます。
  このチャンネルは0から正の整数の値で用意できます。
  loop関数の中では実際に出力ができ、ON時間は（数字/分解能）/周波数で求まります。
  ex) 20Hz 8bit 25 のときは50msを256分割し、その25の分ONにしますという意味なので、約5msONになることがわかります。

  ボタンによる回転方向指定についてです。モータードライバーのin1がHIGHかLOWかによって回転方向が変わります。
  どっちに回したいかによって自分で設定しなおしてください。

  方向指定しつつ、回転速度を決めるのがセグウェイの制御の肝です。
  ボタンによって決めるのであれば、if文で方向指定をして、いい感じの速度で回せば良いですが、
  6軸センサによって決めるときは、roll、pitch角の正負によって方向指定をして、絶対値によって回転速度を決めると製品っぽくなります。
  ですが、6軸センサは難しいので、使うにしても方向指定のみに使うといいと思います。

*/

#include <M5Stack.h>

const double PWM_Hz = 2000;// PWM周波数
const uint8_t PWM_level = 8;// PWM分解能 8bit(1～256)
const uint8_t PWM_CH_L = 0;//L motorDriverに入れるpwmを扱うチャンネルを0、直す前は1になっていて、どちらも1では両方制御できないのが明白だとわかると思います。
const uint8_t PWM_CH_R = 1;//R motorDriverに入れるpwmを扱うチャンネルを1、

const int PWM_pin_L = 2;//L motor driver in2
const int PWM_pin_R = 5;//R motor driver in2

const int IO_pin_L = 16;//L motor driver in1
const int IO_pin_R = 17;//R motor driver in1

void setup() {
  M5.begin();
  M5.Power.begin();

  pinMode(PWM_pin_L, OUTPUT); 
  pinMode(PWM_pin_R, OUTPUT); 
  pinMode(IO_pin_L, OUTPUT);
  pinMode(IO_pin_R, OUTPUT);
  

  // チャンネルと周波数の分解能を設定
  ledcSetup(PWM_CH_L, PWM_Hz, PWM_level);
  ledcSetup(PWM_CH_R, PWM_Hz, PWM_level);
  // モータのピンとチャンネルの紐づけ
  ledcAttachPin(PWM_pin_L, PWM_CH_L);
  ledcAttachPin(PWM_pin_R, PWM_CH_R);

  //M5stackの起動時に回り続けないようにする。
  ledcWrite(PWM_CH_L,0);
  ledcWrite(PWM_CH_R,0);

  M5.Lcd.setTextSize(244);
  M5.Lcd.setTextColor(GREEN , BLACK);
  M5.Lcd.fillScreen(BLACK);
  
}

void loop() {
  M5.update();
  M5.Lcd.setCursor(0, 0);

  if (M5.BtnB.isPressed())
  {
    // デューティー比0.25(64/256)でPWM制御
    digitalWrite(IO_pin_L, HIGH);
    digitalWrite(IO_pin_R, HIGH);
    ledcWrite(PWM_CH_L,64);
    ledcWrite(PWM_CH_R,64);
    M5.Lcd.setCursor(0, 0);
    M5.Lcd.printf("↑");
  }

  else if (M5.BtnA.isPressed())
  {
    // デューティー比0.25(64/256)でPWM制御
    digitalWrite(IO_pin_L, HIGH);
    digitalWrite(IO_pin_R, LOW);
    ledcWrite(PWM_CH_L,8);
    ledcWrite(PWM_CH_R,64);
    M5.Lcd.setCursor(0, 0);
    M5.Lcd.printf("←");
  }

  else if (M5.BtnC.isPressed())
  {
    // デューティー比0.25(64/256)でPWM制御
    digitalWrite(IO_pin_L, LOW);
    digitalWrite(IO_pin_R, HIGH);
    ledcWrite(PWM_CH_L,64);
    ledcWrite(PWM_CH_R,8);
    M5.Lcd.setCursor(0, 0);
     M5.Lcd.printf("→");
  }
 /*  else if (M5.BtnC.isPressed())
  {
    // デューティー比0.25(64/256)でPWM制御
    ledcWrite(PWM_CH_L,);
    ledcWrite(PWM_CH_R,);
    M5.Lcd.setCursor(0, 0);
     M5.Lcd.printf("↓");
  }
  */

  else
  {
    M5.Lcd.setCursor(0, 0);
    M5.Lcd.printf("STOP");
    
    //とめる
    ledcWrite(PWM_CH_L,0);
    ledcWrite(PWM_CH_R,0);
  }
}
