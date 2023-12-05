#include <Arduino.h>
#include "SoftwareSerial.h"
#include "BluetoothSerial.h"

SoftwareSerial unoSerial1(16, 17);
SoftwareSerial unoSerial2(18, 19);
BluetoothSerial SerialBT;

void setup(void)
{
  Serial.begin(9600);
  SerialBT.begin("ESP32_TEAM_4");
  unoSerial1.begin(9600);
  unoSerial2.begin(9600);
  Serial.println("Bluetooth Device is Ready to Pair");
}

void loop(void)
{
  if (SerialBT.available())
  {
    char receivedChar = SerialBT.read();
    Serial.write(receivedChar);
    
    unoSerial1.write(receivedChar);
    unoSerial2.write(receivedChar);
  }
    if (unoSerial1.available()) // 메뉴 선택 후 각 보드에 데이터를 입력하고, 최종 값을 esp에 반환하고, 시리얼에 출력
    {
      char receivedChar_1 = unoSerial1.read();
      Serial.write(receivedChar_1);
      SerialBT.write(receivedChar_1);
    }
    
    if (unoSerial2.available())
    {
      char receivedChar_2 = unoSerial2.read();
      Serial.write(receivedChar_2);
      SerialBT.write(receivedChar_2);
    }
}



