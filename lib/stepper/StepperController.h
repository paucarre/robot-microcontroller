#ifndef StepperController_h
#define StepperController_h
#define LIBRARY_VERSION 0.0.1

class StepperController{

	public:
		StepperController();
		void initialize();
		void close();
		void set_speed(float speed);

	private:
		float speed_to_delay(float speed);

		float right_angle = 0.3;
		float left_angle  = 3.5;
		boolean direction_right = true;
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

};
#endif
