/*
 Name:		HOT_TEMP.ino
 Created:	14.07.2020 7:49:16
 Author:	Serhii
 Для настройки WiFi отредактировать #define WIFI_ROUTER 1 в файле Setting_fll.h
 Настройки очки доступа и роутера осуществляется в WiFI.cpp
 В том же файле все настройки по точке доступа и роутеру
Для настройки файловой системи FS должно быть розкомментировано #define FS_FS 1 в файле Setting_fll.h
Для настройки файловой системи LittltFS должно быть закомментировано #define FS_FS 1 в файле Setting_fll.h
*/

#include <iostream>
#include "Setting_All.h"
#include "WiFi_.h"
//#include "Json_.h"
#include "HTTP_hand.h"

//Thermo MAX6675
#include <Thermocouple.h>
#include <MAX6675_Thermocouple.h>

extern char dataJson;
extern char configJson;
extern String File_configJson;
extern String File_dataJson;

Wifi_ my_WiFi;

extern ESP8266WebServer HTTP;

//Thermo MAX6675
Thermocouple *thermocouple;
//Termo cjnfiguration
#define SO_PIN 12
#define CS_PIN 13
#define SCK_PIN 14

//const uint8_t ADC_Pin(A0);

void setup(void)
{
	delay(1000);
	Serial.begin(115200);
	while (!Serial)
	{
		;
	}

	Serial.println("FS_init started");
	delay(1000);
	FS_init();

#if WIFI_ROUTER
	extern Wifi_Router wifi_iD_router;
	my_WiFi.WiFi_Init();
#else
	extern Wifi_Ap wifi_id_AP;
	my_WiFi.StartAPMode();
#endif

	thermocouple = new MAX6675_Thermocouple(SCK_PIN, CS_PIN, SO_PIN);

	Serial.println("HTTP_init started");
	delay(1000);
	HTTP_init();

	Serial.println("HTTP server started");
}

void loop()
{
	HTTP.handleClient();
	// Reads temperature
	//const double celsius = thermocouple->readCelsius();

	//Output of information
	//Serial.print("Temperature: ");
	//Serial.print(celsius);
	//Serial.println(" C");

	//delay(50000);
}
