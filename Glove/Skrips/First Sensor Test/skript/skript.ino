#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <ESP8266WiFi.h>

Adafruit_MPU6050 mpu;

const char* ssid = "SSID";
const char* password = "PW";

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  } else {
    Serial.println("MPU6050 found!");
  }
}

void loop() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  Serial.print("Accel X: ");
  Serial.print(a.acceleration.x);
  Serial.print(" \tY: ");
  Serial.print(a.acceleration.y);
  Serial.print(" \tZ: ");
  Serial.print(a.acceleration.z);
  Serial.println(" m/s^2");

  Serial.print("Gyro X: ");
  Serial.print(g.gyro.x);
  Serial.print(" \tY: ");
  Serial.print(g.gyro.y);
  Serial.print(" \tZ: ");
  Serial.print(g.gyro.z);
  Serial.println(" rad/s");

  Serial.println();

  delay(1000);
}
