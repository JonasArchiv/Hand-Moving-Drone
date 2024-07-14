#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

Adafruit_MPU6050 mpu;
ESP8266WebServer server(80);

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

  server.on("/", HTTP_GET, handleRoot);
  server.begin();
}

void loop() {
  server.handleClient();
  printSensorData();
  delay(1000);
}

void handleRoot() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  float roll  = atan2(a.acceleration.y, a.acceleration.z) * 180.0 / PI;
  float pitch = atan2(-a.acceleration.x, sqrt(a.acceleration.y * a.acceleration.y + a.acceleration.z * a.acceleration.z)) * 180.0 / PI;

  String json = "{";
  json += "\"roll\":" + String(roll) + ",";
  json += "\"pitch\":" + String(pitch) + ",";
  json += "\"accelX\":" + String(a.acceleration.x);
  json += "}";

  server.send(200, "application/json", json);
}

void printSensorData() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  float roll  = atan2(a.acceleration.y, a.acceleration.z) * 180.0 / PI;
  float pitch = atan2(-a.acceleration.x, sqrt(a.acceleration.y * a.acceleration.y + a.acceleration.z * a.acceleration.z)) * 180.0 / PI;

  Serial.print("Roll: ");
  Serial.print(roll);
  Serial.print(" degrees, Pitch: ");
  Serial.print(pitch);
  Serial.print(" degrees, Accel X: ");
  Serial.print(a.acceleration.x);
  Serial.println(" m/s^2");
}
