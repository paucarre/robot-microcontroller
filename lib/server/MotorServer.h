#ifndef MotorServer_h
#define MotorServer_h
#define LIBRARY_VERSION 0.0.1
#include <WiFi.h>

class MotorServer {

	public:
		MotorServer();
		void initialize(WiFiServer& server);

	private:
		const char *ssid     = "VMF6DA892";
		const char *password = "Vbzbcfs7aaph";


};
#endif
