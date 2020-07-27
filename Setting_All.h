#pragma once
// Начнем с директив препроцессора. __WIFI_H – это произвольное уникальное имя (обычно используется имя заголовочного файла)
#ifndef __SETTING_ALL_H
#define __SETTING_ALL_H

#include <iostream>
#include <Arduino.h>
//#include "WiFi_.h"
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266WebServerSecure.h>
#include <ESP8266HTTPUpdateServer.h>
//#include <Fs.h>
//#include <LittleFS.h>

//Thermo MAX6675
#include <Thermocouple.h>
#include <MAX6675_Thermocouple.h>

#define WIFI_ROUTER 1

//#define FS_FS 1

struct Wifi_Router
{
	String _ssid;		// Для хранения SSID
	String _password; // Для хранения пароля сети
	IPAddress IPADR;
	IPAddress IPROUTER;
	IPAddress netmaska;
	IPAddress DNS_1;
};
struct Wifi_Ap
{
	const char *ssid;		 // SSID AP точки доступа
	const char *password; // Пароль точки доступа
	IPAddress IPADR_AP;
	IPAddress IP_AP;
	IPAddress netmaska;
	IPAddress DNS_1;
};

//#define FS_LittleFS 1
// Заканчиваем директивой
#endif // __SETTING_ALL_H
