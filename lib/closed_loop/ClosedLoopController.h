#ifndef ClosedLoopController_h
#define ClosedLoopController_h
#define LIBRARY_VERSION 0.0.1

#ifndef as5048_h
#include <AS5048A.h>
#endif

#ifndef StepperController_h
#include <StepperController.h>
#endif

typedef void (*voidFuncPtr)(void);

class ClosedLoopController{

	public:
		ClosedLoopController(AS5048A _angle_sensor, StepperController _stepper_controller, SemaphoreHandle_t _semaphore, voidFuncPtr _on_timer);
		void initialize();
		void control_loop(float target_angle);

	private:
		volatile SemaphoreHandle_t semaphore;
		hw_timer_t * timer = NULL;
		portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;
		AS5048A angle_sensor;
		StepperController stepper_controller;
		voidFuncPtr on_timer;

		float compute_control_speed(float current_angle, float target_angle);

};
#endif
