#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#define DHTPIN A0
#define DHTTYPE DHT11

int COOLING_FAN_PIN_1 = 5;
int COOLING_FAN_PIN_2 = 6;
int RELAY_MODULE_PIN = 13;
int TEMPERATURE_THRESHOLD = 25; // 기본 온도 설정
int HUMIDITY_THRESHOLD = 30;

DHT dht(DHTPIN, DHTTYPE);
int cusTemp= TEMPERATURE_THRESHOLD;
int cusHumi = HUMIDITY_THRESHOLD;

void setup()
{
  Serial.begin(9600);
  pinMode(DHTPIN, INPUT);
  pinMode(COOLING_FAN_PIN_1, OUTPUT);
  pinMode(COOLING_FAN_PIN_2, OUTPUT);
  pinMode(RELAY_MODULE_PIN, OUTPUT);
  dht.begin();
}

void loop()
{
  int h = dht.readHumidity();
  int t = dht.readTemperature();

  Serial.print("t");
  Serial.println(dht.readTemperature());
  delay(5000);
  Serial.print("h");
  Serial.println(dht.readHumidity());
   
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    command.trim();
    
    Serial.println(command);

    if (command.startsWith("T")) { 
      cusTemp = command.substring(1).toFloat();
    }
    else if (command.startsWith("H")) {
      cusHumi = command.substring(1).toFloat();
    }
  }
   
  if (t > cusTemp || h > cusHumi)
  {
    analogWrite(COOLING_FAN_PIN_1, 150); //팬 가동
    analogWrite(COOLING_FAN_PIN_2, 0);
    digitalWrite(RELAY_MODULE_PIN, HIGH); //열선패드 중단
  }
  //현재 < 적정
  else
  {
    analogWrite(COOLING_FAN_PIN_1, 0); //팬 중단
    analogWrite(COOLING_FAN_PIN_2, 0);
    digitalWrite(RELAY_MODULE_PIN, LOW); //열선패드 가동
  }
  /*if (isnan(h) || isnan(t))
  {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
*/
  delay(5000);
}
