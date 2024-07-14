#include <Wire.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <HTTPClient.h>
#include <MeMCore.h>

MeDCMotor motorLeft(M1);
MeDCMotor motorRight(M2);

const char* ssid = "SSID";
const char* password = "PW";
const char* serverName = "http://192.168.178.167/";

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverName);

    int httpCode = http.GET();
    if (httpCode > 0) {
      String payload = http.getString();

      StaticJsonDocument<200> doc;
      deserializeJson(doc, payload);

      float roll = doc["roll"];
      float pitch = doc["pitch"];
      float speedY = doc["speedY"];

      Serial.print("Roll: ");
      Serial.print(roll);
      Serial.print(" Pitch: ");
      Serial.print(pitch);
      Serial.print(" SpeedY: ");
      Serial.println(speedY);

      controlMBot(roll, pitch, speedY);
    }
    http.end();
  }
  delay(1000); 
}

void controlMBot(float roll, float pitch, float speedY) {
  int speedLeft = 0;
  int speedRight = 0;

  if (pitch > 5) {
    speedLeft = speedRight = 255;
  } else if (pitch < -5) { 
    speedLeft = speedRight = -255;
  } else if (roll > 5) { 
    speedLeft = 255;
    speedRight = -255;
  } else if (roll < -5) { 
    speedLeft = -255;
    speedRight = 255;
  } else {
    speedLeft = speedRight = 0;
  }

  motorLeft.run(speedLeft);
  motorRight.run(speedRight);
}
