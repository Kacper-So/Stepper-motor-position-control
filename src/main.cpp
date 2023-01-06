#include <Arduino.h>
#include <Wire.h>
#include "AS5600.h"

#define M0                3
#define M1                4
#define M2                5
#define DIR               7
#define STP               6
#define STEP_ANGLE        1.8
#define STEP_MULTIPLIER_1 1
#define STEP_MULTIPLIER_2 0.5
#define STEP_MULTIPLIER_3 0.25
#define STEP_MULTIPLIER_4 0.125
#define STEP_MULTIPLIER_5 0.0625
#define STEP_MULTIPLIER_6 0.03125

AS5600 encoder;   //  use default Wire
float curr_position;
float set_position = 0;
float errorDIR1;
float errorDIR2;
float error;
String uart_data;

void setup(){
  randomSeed(analogRead(0));
  pinMode(M0, OUTPUT);
  pinMode(M1, OUTPUT);
  pinMode(M2, OUTPUT);
  pinMode(DIR, OUTPUT);
  pinMode(STP, OUTPUT);
  digitalWrite(M0, HIGH);
  digitalWrite(M1, HIGH);
  digitalWrite(M2, LOW);
  digitalWrite(DIR, HIGH);
  Serial.begin(9600);
  if(!encoder.begin(DIR)){
    for(;;){
      Serial.println("ENCODER NOT FOUND, RST");
      delay(1000);
    }
  }
  delay(1000);
}


void loop(){
  static uint32_t lastTime = 0;
  static uint32_t lastTime2 = 0;
  if (Serial.available()){
    uart_data = Serial.readStringUntil('\n');
    if (!(atoi(uart_data.c_str()) >= 0 && atoi(uart_data.c_str()) < 360)){
      Serial.println("WRONG");
    } else { 
      set_position = atoi(uart_data.c_str());
    }
  }
  if (millis() - lastTime2 >= 5000){
    lastTime2 = millis();
    set_position = random(360);
  }
  if (millis() - lastTime >= 10){
    lastTime = millis();
    curr_position = ((float)encoder.readAngle()/4096.f) * 360.f;
    Serial.println(curr_position);
    if(curr_position > set_position){
      errorDIR1 = 360 - curr_position + set_position;
    } else { 
      errorDIR1 = set_position - curr_position;
    }
    errorDIR2 = 360 - errorDIR1;
    if(errorDIR1 < errorDIR2){
      error = errorDIR1;
      digitalWrite(DIR, HIGH);
    } else {
      error = errorDIR2;
      digitalWrite(DIR, LOW);
    }
    if (error > STEP_ANGLE * STEP_MULTIPLIER_1){
      digitalWrite(M0, LOW);
      digitalWrite(M1, LOW);
      digitalWrite(M2, LOW);
      digitalWrite(STP, HIGH);
      delayMicroseconds(1000);
      digitalWrite(STP, LOW);
      delayMicroseconds(1000);
    } else if (error < STEP_ANGLE * STEP_MULTIPLIER_1 && error > STEP_ANGLE * STEP_MULTIPLIER_2){
      digitalWrite(M0, HIGH);
      digitalWrite(M1, LOW);
      digitalWrite(M2, LOW);
      digitalWrite(STP, HIGH);
      delayMicroseconds(1000);
      digitalWrite(STP, LOW);
      delayMicroseconds(1000);
    } else if (error < STEP_ANGLE * STEP_MULTIPLIER_2 && error > STEP_ANGLE * STEP_MULTIPLIER_3){
      digitalWrite(M0, LOW);
      digitalWrite(M1, HIGH);
      digitalWrite(M2, LOW);
      digitalWrite(STP, HIGH);
      delayMicroseconds(1000);
      digitalWrite(STP, LOW);
      delayMicroseconds(1000);
    } else if (error < STEP_ANGLE * STEP_MULTIPLIER_3 && error > STEP_ANGLE * STEP_MULTIPLIER_4){
      digitalWrite(M0, HIGH);
      digitalWrite(M1, HIGH);
      digitalWrite(M2, LOW);
      digitalWrite(STP, HIGH);
      delayMicroseconds(1000);
      digitalWrite(STP, LOW);
      delayMicroseconds(1000);
    } else if (error < STEP_ANGLE * STEP_MULTIPLIER_4 && error > STEP_ANGLE * STEP_MULTIPLIER_5){
      digitalWrite(M0, LOW);
      digitalWrite(M1, LOW);
      digitalWrite(M2, HIGH);
      digitalWrite(STP, HIGH);
      delayMicroseconds(1000);
      digitalWrite(STP, LOW);
      delayMicroseconds(1000);
    } else if (error < STEP_ANGLE * STEP_MULTIPLIER_5 && error > STEP_ANGLE * STEP_MULTIPLIER_6){
      digitalWrite(M0, HIGH);
      digitalWrite(M1, LOW);
      digitalWrite(M2, HIGH);
      digitalWrite(STP, HIGH);
      delayMicroseconds(1000);
      digitalWrite(STP, LOW);
      delayMicroseconds(1000);
    }
    if (encoder.getRevolutions() >= 1){
      encoder.resetPosition();
    }
  }
}
