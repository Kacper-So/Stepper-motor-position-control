#include <Arduino.h>
#include <Wire.h>
#include "AS5600.h"

#define M0        3
#define M1        4
#define M2        5
#define DIR       7
#define STP       6

AS5600 encoder;   //  use default Wire
float position;

void setup()
{
  pinMode(M0, OUTPUT);
  pinMode(M1, OUTPUT);
  pinMode(M2, OUTPUT);
  pinMode(DIR, OUTPUT);
  pinMode(STP, OUTPUT);
  digitalWrite(M0, HIGH);
  digitalWrite(M1, HIGH);
  digitalWrite(M2, LOW);
  digitalWrite(DIR, HIGH);
  Serial.begin(115200);
  if(!encoder.begin(DIR)){
    for(;;){
      Serial.println("ENCODER NOT FOUND, RST");
      delay(1000);
    }
  } else {
    encoder.setDirection(AS5600_CLOCK_WISE);  // default, just be explicit.
  }
  delay(1000);
}


void loop()
{
  static uint32_t lastTime = 0;
  static uint32_t lastTime2 = 0;

  encoder.getCumulativePosition();

  //  update every 100 ms
  if (millis() - lastTime >= 100){
    lastTime = millis();
    position = ((float)encoder.readAngle()/4096.f) * 360.f;
    Serial.println(position);
  }
  if (millis() - lastTime2 >= 200){
    lastTime2 = millis();
		digitalWrite(STP, HIGH);
    delayMicroseconds(1000);
    digitalWrite(STP, LOW);
  }

  //  just to show how reset can be used
  if (encoder.getRevolutions() >= 1){
    encoder.resetPosition();
  }
}
