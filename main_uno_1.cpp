#include <Adafruit_Sensor.h>
#include <Arduino.h>
#include <DHT.h>
#include <DHT_U.h>
#define DHTPIN A0
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

void setup()
{
  Serial.begin(115200);
  pinMode(A0, INPUT);  // 온습도 센서
  pinMode(5, OUTPUT);  // 쿨러 출력
  pinMode(6, OUTPUT);  // 쿨러 출력
  pinMode(13, OUTPUT); // 릴레이 모듈 출력, 릴레이 모듈이 꺼져있을 때 열선패드 작동
  dht.begin();
}

void loop()
{
  delay(2000);

  int h = dht.readHumidity();    // 습도 값 저장
  int t = dht.readTemperature(); // 온도 값 저장

  if (Serial.available())
  {

    String command = Serial.readStringUntil('\n');
    command.trim();

    if (command == "T")
    {
      Serial.print("Temperature: ");
      Serial.print(t);
      Serial.println(" °C");
    }

    else if (command == "H")
    {
      Serial.print("Humidity: ");
      Serial.print(h);
      Serial.println("%");
    }
  }
  // 온도 값이 25도보다 높거나 습도 값이 30%보다 높으면 쿨링팬 작동, 릴레이 모듈 키기
  if (t > 25.0 || h > 30)
  {
    analogWrite(5, 255);
    analogWrite(6, 0);
    digitalWrite(13, HIGH);
  }
  // 위의 조건이 아닐 때 릴레이 모듈을 끔
  else
  {
    analogWrite(5, 0);
    analogWrite(6, 0);
    digitalWrite(13, LOW);
  }
}
