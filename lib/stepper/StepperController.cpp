#include "Arduino.h"

#include <StepperController.h>

StepperController::StepperController() {
}

void StepperController::initialize(){
	pinMode(ENABLED_PIN, OUTPUT);
	digitalWrite(ENABLED_PIN, HIGH);
	pinMode(DIRECTION_PIN, OUTPUT);
	digitalWrite(DIRECTION_PIN, HIGH);
	pinMode(PULSE_PIN, OUTPUT);
	digitalWrite(PULSE_PIN, HIGH);
	pinMode(COM_PIN, OUTPUT);
	digitalWrite(COM_PIN, HIGH);
}

float StepperController::speed_to_delay(float speed) {
  return (step_angle_radian / speed) - time_step;
}

void StepperController::set_speed(float speed) {
  if(speed == 0) {
    digitalWrite(COM_PIN, LOW);
  } else {
    digitalWrite(COM_PIN, HIGH);
    if(speed > 0) {
        if(direction_right) {
          direction_right = false;
          digitalWrite(DIRECTION_PIN, LOW);
        }
    } else {
         if(!direction_right) {
            direction_right = true;
            digitalWrite(DIRECTION_PIN, HIGH);
         }
         speed = speed * -1;
    }
    float step_delay = speed_to_delay(speed);
    if(step_delay > 1) {
      delay(step_delay);
      Serial.println(step_delay);
    }
    digitalWrite(PULSE_PIN, HIGH);
    delay(0);
    digitalWrite(PULSE_PIN, LOW);
  }
}
