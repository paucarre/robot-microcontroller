#include <AS5048A.h>
#include <StepperController.h>
#include <ClosedLoopController.h>
#include <time.h>
/*
 * Arduino: black, blue white orange green purple
 * ESP32: black grey, blue brown white red
 */


TaskHandle_t controller_task, server_task;
SemaphoreHandle_t baton;

volatile SemaphoreHandle_t semaphore = xSemaphoreCreateBinary();
volatile SemaphoreHandle_t angle_semaphore = xSemaphoreCreateBinary();

void IRAM_ATTR on_timer(){
  xSemaphoreGiveFromISR(semaphore, NULL);
}

AS5048A angle_sensor(SS, 1);
StepperController stepper_controller;
ClosedLoopController closed_loop_controller(angle_sensor, stepper_controller, semaphore, on_timer);
volatile float angle_target = 1.3;

void setup()
{
  Serial.begin(115200);
  closed_loop_controller.initialize();
  
  xTaskCreatePinnedToCore(
    controller_function,
    "controller_task",
    1000,
    NULL,
    1,
    &controller_task,
    1);

  delay(500);  // needed to start-up task1

  xTaskCreatePinnedToCore(
    server_function,
    "server_task",
    1000,
    NULL,
    1,
    &server_task,
    0);
}

void server_function(void * parameters) {
  for(;;){
    delay(3000);
    xSemaphoreTake(angle_semaphore, 0);
    if(angle_target > 1.2) {
      angle_target = 0.5;
    } else {
      angle_target = 1.5;
    }
    Serial.print("Current Angle Target ");
    Serial.println(angle_target);
    xSemaphoreGive(angle_semaphore);
  }
}

void controller_function(void * parameters) {
  for(;;){
    float angle_target_local;
    xSemaphoreTake(angle_semaphore, 0);
    angle_target_local = angle_target;
    xSemaphoreGive(angle_semaphore);
    closed_loop_controller.control_loop(angle_target_local);
  }
}

void loop(){
}


