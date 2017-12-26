#include <AS5048A.h>
#include <time.h>
/*
 * Arduino: black, blue white orange green purple
 * ESP32: black grey, blue brown white red 
 */

AS5048A angleSensor(SS, 1);

float current_angle = 0.0;
float right_angle = 0.3;
float left_angle  = 3.5;
boolean direction_right = true;
time_t transition_time = time(nullptr);
int steps = 0;
float stepAngle = 1.8;
float step_angle_radian = 0.0314159;
float max_speed = 2;
float time_step = step_angle_radian /  max_speed;
int ENABLED_PIN = 32;
int DIRECTION_PIN = 33; 
int PULSE_PIN = 25;
int COM_PIN = 26;
boolean directionHigh = true;

void initServoController() {
  pinMode(ENABLED_PIN, OUTPUT); 
  digitalWrite(ENABLED_PIN, HIGH); 
  pinMode(DIRECTION_PIN, OUTPUT); 
  digitalWrite(DIRECTION_PIN, HIGH); 
  pinMode(PULSE_PIN, OUTPUT); 
  digitalWrite(PULSE_PIN, HIGH); 
  pinMode(COM_PIN, OUTPUT); 
  digitalWrite(COM_PIN, HIGH);  
}

float speed_to_delay(float speed) {
  return (step_angle_radian / speed) - time_step;
}

void setup()
{
  Serial.begin(115200);
  
  angleSensor.init();
  initServoController(); 
}

float computeControlSpeed(float current_angle, float target_angle) {
  float error = ((target_angle - current_angle)) / (2 * PI);  
  if(abs(error) < 0.001) {
    error = 0.0;
  }
  return error;
}

float configure_speed(float speed) {
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
    int step_delay = speed_to_delay(speed);
    if(step_delay > 1) {
      delay(step_delay);
      Serial.println(step_delay);
    }
    digitalWrite(PULSE_PIN, HIGH);  
    delay(0); 
    digitalWrite(PULSE_PIN, LOW); 
  }
}


float quinticPoliynomial(float t, float q_init, float q_diff) {
  float t3 = t * t * t;
  float t4 = t3 * t;
  float t5 = t4 * t;
  return q_init + ( q_diff * ( (6.0 * t5) - (15.0 * t4) + (10 * t3) ) );
}

void loop()
{
   float current_angle = angleSensor.getRotationInRadians();
   float speed = computeControlSpeed(current_angle , 1.5);
   configure_speed(speed);
}

