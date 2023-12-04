#include <Arduino.h>
#include "SoftwareSerial.h"
#include "BluetoothSerial.h"
// 라이브러리

SoftwareSerial unoSerial1(16, 17);
SoftwareSerial unoSerial2(18, 19);
BluetoothSerial SerialBT;

String inputSting = "";         // 블루투스를 통해 받은 문자열을 저장할 수 있는 변수
boolean stringComplete = false; // 문자열을 완전히 받았는지의 여부

void setup(void)
{
  Serial.begin(115200);
  SerialBT.begin("ESP32_TEAM_4");
  unoSerial1.begin(115200);
  unoSerial2.begin(115200);
  Serial.println("Bluetooth Device is Ready to Pair");
}

void loop(void)
{
  if (SerialBT.available())
  {
    char receivedChar = SerialBT.read();
    Serial.print(receivedChar);

    if (receivedChar == '\n')
    {
      stringComplete = true;
    }
    else
    {
      inputSting += receivedChar;
    }
  }
  if (stringComplete)
  {
    Serial.println(inputSting);

    if (inputSting.startsWith("P")) // P30입력하면 온도를 30으로 조절
    // P**을 입력하면, P뒤의 숫자를 세팅하려는 온도로 우노 보드 1에 전송
    {
      unoSerial1.print(inputSting);
    }
    else
    {
      if (inputSting == "T" || inputSting == "H") //T입력시 온도 출력, H입력 시 습도 출력
      {
        unoSerial1.print(inputSting);
      }
      else
      {
        unoSerial2.print(inputSting);
      }
    }

    inputSting = "";
    stringComplete = false;
  }

  if (unoSerial1.available()) // 메뉴 선택 후 각 보드에 데이터를 입력하고, 최종 값을 esp에 반환하고, 시리얼에 출력
  {
    char receivedChar_1 = unoSerial1.read();
    Serial.write("t" + receivedChar_1);
    SerialBT.write('t' + receivedChar_1);
  }
  
  if (unoSerial2.available())
  {
    char receivedChar_2 = unoSerial2.read();
    Serial.write("t" + receivedChar_2);
    SerialBT.write('t' + receivedChar_2);
  }
}