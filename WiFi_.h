#pragma once
// Начнем с директив препроцессора. __WIFI_H – это произвольное уникальное имя (обычно используется имя заголовочного файла)
#ifndef __WIFI_H
#define __WIFI_H

#include <iostream>
#include "Setting_All.h"
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
/*
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
*/
class Wifi_
{
public:
	Wifi_() {}
	~Wifi_() {}
	void WiFi_Init();
	void StartAPMode();

protected:
private:
};

// Заканчиваем директивой
#endif // __WIFI_H
