#include <M5Stack.h>

#define CMD_SIZE            2   /**< コマンドサイズ */
#define CMD_TYPE            0   /**< コマンド構成データ: コマンド種類 */
#define CMD_PIN             1   /**< コマンド構成データ: ピン番号 */
#define CMD_WRITE_LOW       0   /**< コマンド種類: LOW書込み */
#define CMD_WRITE_HIGH      1   /**< コマンド種類: HIGH書込み */
#define CMD_WRITE_TOGGLE    2   /**< コマンド種類: ピン状態切替え */
#define CMD_READ            4   /**< コマンド種類: ピン状態読込み */
/** @} */

/**
 * @name    レスポンス
 */
/** @{ */
#define RES_SIZE            2   /**< レスポンスサイズ */
#define RES_PIN             0   /**< レスポンス構成データ: ピン番号 */
#define RES_PIN_VALUE       1   /**< レスポンス構成データ: ピン状態 */
/** @} */

/**
 * @name    ピンアサイン
 */
/** @{ */
#define PIN_LED             5  /**< ピン番号: LED */
/** @} */


uint8_t cmd[CMD_SIZE];          /**< コマンド */
uint8_t res[RES_SIZE];          /**< レスポンス */
uint8_t cmdRecievedBytes;       /**< コマンド受信済バイト数 */

bool PINstatus;

void setup()
{
    m5.begin(true, false, true, false);
    m5.Power.begin();
    m5.Lcd.setTextSize(2);
    M5.Lcd.setTextColor(WHITE, BLACK);
    m5.Lcd.fillScreen(BLACK);
    
    //Serial.begin(9600);
    
    cmdRecievedBytes = 0;
    
    pinMode(PIN_LED, OUTPUT);
    digitalWrite(PIN_LED, LOW);
    PINstatus = false;

    m5.Lcd.print("start\r\n");
}


void loop()
{
  //m5.Lcd.print("loop start\r\n");
  
  /* コマンド受信 */
  while ((Serial.available() > 0) && (cmdRecievedBytes < CMD_SIZE)) {
    cmd[cmdRecievedBytes] = Serial.read();
    cmdRecievedBytes++;
  }

  //m5.Lcd.print("read\r\n");
  
  /* コマンド実行 */
  if (cmdRecievedBytes == CMD_SIZE) {
    switch (cmd[CMD_PIN]) {
      case PIN_LED:
        /* ピン状態の更新 */
        switch (cmd[CMD_TYPE])
        {
          case CMD_WRITE_LOW:
            digitalWrite(cmd[CMD_PIN], LOW);
            PINstatus = false;
          break;
          case CMD_WRITE_HIGH:
            digitalWrite(cmd[CMD_PIN], HIGH);
            PINstatus = true;
          break;
          case CMD_WRITE_TOGGLE:
               if (digitalRead(cmd[CMD_PIN]) == LOW)
                {
                    digitalWrite(cmd[CMD_PIN], HIGH);
                    PINstatus = true;
                }
                else
                {
                    digitalWrite(cmd[CMD_PIN], LOW);
                    PINstatus = false;
                }
              break;
              case CMD_READ:
              default: break;
            }
            
            /* レスポンス送信 */
            res[RES_PIN] = cmd[CMD_PIN];
            res[RES_PIN_VALUE] = PINstatus;
            Serial.write(res, sizeof(res));
            break;
          default: break;
        }
        cmdRecievedBytes = 0;
    }
    
    return;
}
