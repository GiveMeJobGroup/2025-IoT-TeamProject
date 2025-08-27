/*
  Keyestudio ESP32-CAM Video Smart Car
  Motor Driver
  https://www.keyestudio.com
*/
#include "SetMotor.h"
void setup() {
  // put your setup code here, to run once:
  i2c_init();  //initialize iic
}

void loop() {
  // put your main code here, to run repeatedly:
  Car_forward(200, 200);    //The car moves forward
  delay(1000);              //delay 2s
  Car_backwards(200, 200);  //The car moves back
  delay(1000);
  Car_left(100, 100);  //The car turns left
  delay(1000);
  Car_right(100, 100);  //The car turns right
  delay(1000);
  Car_stop();  //The car stops
  delay(1000);
}