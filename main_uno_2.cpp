#include <Arduino.h>

int ledPin[6] = {11, 10, 9, 6, 5, 3}; // led 핀
int inputPin = 7;                     // 센서 핀
int val = 0;                          // 센서 값 읽는 변수
int pirstate = LOW;                   // 시리얼 모니터 출력 관련 변수
int ledtype = 3;                      // 앱에서 받을 led 종류 입력 변수
int brightness = 255;                 // 밝기 변수 0~255

void setup()
{
  for (int i = 0; i < 6; i++)
  {
    pinMode(ledPin[i], OUTPUT);
  }
  pinMode(inputPin, INPUT);
  Serial.begin(115200);
}

void loop()
{
  if (Serial.available())
  {
    String command = Serial.readStringUntil('\n');
    command.trim();

    if (command == "C")
    {

      // 활성화된 led를 강제로 끔
      for (int i = ledtype; i < ledtype + 3; i++)
      {
        digitalWrite(ledPin[i], LOW);
      }

      ledtype = (ledtype == 3) ? 0 : 3;
      Serial.println("LED is Changed");
    }

    else if (command.toInt() >= 0 && command.toInt() <= 255)
    {
      brightness = command.toInt();
      for (int i = ledtype; i < ledtype + 3; i++)
      {
        analogWrite(ledPin[i], brightness);
      }
    }
  }

  val = digitalRead(inputPin);
  if (val == HIGH)
  { // 센서가 감지한 경우
    for (int i = ledtype; i < ledtype + 3; i++)
    {
      analogWrite(ledPin[i], brightness);
    }
    if (pirstate == LOW)
    { // 시리얼 모니터 출력 함수
      Serial.println("Welcome!");
      pirstate = HIGH;
    }
  }
  else
  { // 센서가 감지하지 못한 경우
    for (int j = ledtype; j < ledtype + 3; j++)
    {
      digitalWrite(ledPin[j], LOW);
    }
    if (pirstate == HIGH)
    { // 시리얼 모니터 출력 함수
      Serial.println("Good Bye");
      pirstate = LOW;
    }
  }
}
