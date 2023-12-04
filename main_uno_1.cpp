#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN A0
#define DHTTYPE DHT11
#define COOLING_FAN_PIN_1 5
#define COOLING_FAN_PIN_2 6
#define RELAY_MODULE_PIN 13
#define DEFAULT_TEMPERATURE_THRESHOLD 25.0 // 기본 온도 설정
#define HUMIDITY_THRESHOLD 30.0

DHT dht(DHTPIN, DHTTYPE);
float customTemperature = DEFAULT_TEMPERATURE_THRESHOLD; // 세팅 값 초기화

void handleSerialInput();                                  // 값을 시리얼 채널에 출력
void controlCoolingFan(float temperature, float humidity); // 쿨링팬 조절

void setup()
{
  Serial.begin(115200);
  pinMode(DHTPIN, INPUT);
  pinMode(COOLING_FAN_PIN_1, OUTPUT);
  pinMode(COOLING_FAN_PIN_2, OUTPUT);
  pinMode(RELAY_MODULE_PIN, OUTPUT);
  dht.begin();
}

void loop()
{
  delay(500);

  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t))
  {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  if (Serial.available())
  {
    handleSerialInput();
  }

  controlCoolingFan(t, h);
}

void handleSerialInput()
{
  String command = Serial.readStringUntil('\n');
  command.trim();

  if (command == "T")
  {
    Serial.print("Temperature: ");
    Serial.print(dht.readTemperature());
    Serial.println(" °C");
  }
  else if (command == "H")
  {
    Serial.print("Humidity: ");
    Serial.print(dht.readHumidity());
    Serial.println("%");
  }

  else if (command.startsWith("P")) // P뒤의 숫자를 설정하려는 온도로 값 대입
  {
    customTemperature = command.substring(1).toFloat();

    Serial.print("Custom Temperature Threashold set to: "); // 설정 후 출력
    Serial.print(customTemperature);
    Serial.println(" °C");
  }
}

// 설정된 온도에 달할 때 까지 팬을 돌리거나 정지시키도록 설정.
void controlCoolingFan(float temperature, float humidity)
{
  if (temperature > customTemperature || humidity > HUMIDITY_THRESHOLD)
  {
    analogWrite(COOLING_FAN_PIN_1, 255);
    analogWrite(COOLING_FAN_PIN_2, 0);
    digitalWrite(RELAY_MODULE_PIN, HIGH);
  }

  else
  {
    analogWrite(COOLING_FAN_PIN_1, 0);
    analogWrite(COOLING_FAN_PIN_2, 0);
    digitalWrite(RELAY_MODULE_PIN, LOW);
  }
}
