#include <AS5048A.h>
#include <StepperController.h>
#include <ClosedLoopController.h>
#include <MotorServer.h>
#include <time.h>
/*
 * Arduino: black, blue white orange green purple
 * ESP32: black grey, blue brown white red
 */


TaskHandle_t controller_task, server_task;

volatile SemaphoreHandle_t semaphore = xSemaphoreCreateBinary();
volatile SemaphoreHandle_t angle_target_semaphore = xSemaphoreCreateBinary();
volatile SemaphoreHandle_t angle_sensor_semaphore = xSemaphoreCreateBinary();
WiFiServer server(23);
MotorServer motor_server;

void IRAM_ATTR on_timer(){
  xSemaphoreGiveFromISR(semaphore, NULL);
}


volatile float angle_target = 1.3;
AS5048A angle_sensor(SS, 1);
StepperController stepper_controller;
ClosedLoopController closed_loop_controller(1.1275, angle_sensor, stepper_controller, angle_sensor_semaphore, on_timer);

void setup()
{
  Serial.begin(115200);
  closed_loop_controller.initialize();
  motor_server.initialize(server);

  xTaskCreatePinnedToCore(
    server_function,
    "server_task",
    4000,
    NULL,
    1,
    &server_task,
    1);
    
  xTaskCreatePinnedToCore(
    controller_function,
    "controller_task",
    4000,
    NULL,
    1,
    &controller_task,
    0);
 
}

void controller_function(void * parameters) {
  for(;;){
    float angle_target_local;
    xSemaphoreTake(angle_target_semaphore, 0);
    angle_target_local = angle_target;
    xSemaphoreGive(angle_target_semaphore);
    closed_loop_controller.control_loop(angle_target_local);
  }
}

float messageToAngle(String message) {
  float angle = 0; 
  int32_t exponential = 1;
  for(uint32_t it = 0; it < message.length(); ++it) {
    int32_t decimal_value = decimal(message.charAt(it));
    if(decimal_value != -1){
     angle = angle + (((float)decimal_value) / (float)exponential);
     exponential = exponential * 10;
    }
  }
  return angle;
}

void angleToMessage(float angle, char* message, uint32_t length) {
  dtostrf(angle,length, 5, message); 
}

void server_function(void * parameters){
  for(;;){
    WiFiClient client = server.available();
    if (client.connected() && client.available()){
      unsigned long timeout = millis();
      bool is_available = true;
      if(client.available()) {
        uint8_t command = client.read();
        if(command == '>') {
          String message = client.readStringUntil('\r');
          float angle = messageToAngle(message);
          xSemaphoreTake(angle_target_semaphore, 0);
          angle_target = angle;
          xSemaphoreGive(angle_target_semaphore);
          Serial.println(angle);
        } else if(command == '?') {
          float angle_from_sensor = closed_loop_controller.getCurrentSensorAngle();
          char message[15];
          angleToMessage(angle_from_sensor, message, 5);
          client.print(message); 
        }
        client.stop();
      }
    }
  delay(100);
  }
}


int32_t decimal(uint8_t character){
  switch(character){
    case '0':
      return 0;
    case '1':
      return 1;
    case '2':
      return 2;
    case '3':
      return 3;
    case '4':
      return 4;
    case '5':
      return 5;
    case '6':
      return 6;
    case '7':
      return 7;
    case '8':
      return 8;
    case '9':
      return 9;
  }
  return -1;
}

void loop() {
  sleep(1000);  
}
