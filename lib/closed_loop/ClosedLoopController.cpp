#include "Arduino.h"

#include <ClosedLoopController.h>

ClosedLoopController::ClosedLoopController(AS5048A _angle_sensor,
		StepperController _stepper_controller,
		SemaphoreHandle_t _angle_sensor_semaphore,
		voidFuncPtr _on_timer):
	angle_sensor(_angle_sensor),
	stepper_controller(_stepper_controller),
	angle_sensor_semaphore(_angle_sensor_semaphore),
	on_timer(_on_timer) {
}

void ClosedLoopController::initialize() {
	angle_sensor.init();
	stepper_controller.initialize();
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, on_timer, true);
  timerAlarmWrite(timer, 1000, true);
  timerAlarmEnable(timer);
}

float ClosedLoopController::compute_control_speed(float current_angle, float target_angle) {
  float error = ((target_angle - current_angle)) / (2 * PI);
  if(abs(error) < 0.001) {
    error = 0.0;
  }
  return error;
}

float ClosedLoopController::getCurrentSensorAngle(){
	float current_angle;
	xSemaphoreTake(angle_sensor_semaphore, 0);
	current_angle = angle_from_sensor;
	xSemaphoreGive(angle_sensor_semaphore);
	return current_angle;
}

void ClosedLoopController::control_loop(float target_angle) {
	float current_angle = angle_sensor.getRotationInRadians();
	xSemaphoreTake(angle_sensor_semaphore, 0);
	angle_from_sensor = current_angle;
	xSemaphoreGive(angle_sensor_semaphore);
	float speed = compute_control_speed(current_angle , target_angle);
  stepper_controller.set_speed(speed);
}
