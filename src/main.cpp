#include <Arduino.h>
#include <Wire.h>
#include "AS5600.h"

#define M0        1
#define M1        1
#define M2        1
#define DIR       1
#define STP       1

AS5600 encoder;   //  use default Wire
float position;

void setup()
{
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

  encoder.getCumulativePosition();

  //  update every 100 ms
  if (millis() - lastTime >= 100){
    lastTime = millis();
    position = ((float)encoder.readAngle()/4096.f) * 360.f;
    Serial.println(position);
  }

  //  just to show how reset can be used
  if (encoder.getRevolutions() >= 1){
    encoder.resetPosition();
  }
}
