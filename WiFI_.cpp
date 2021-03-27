#include "WiFi_.h"

extern Wifi_Router wifi_iD_router;
extern Wifi_Ap wifi_id_AP;
/*
Wifi_Ap wifi_default = {
	 "ESP8266_Termo",
	 "123456789",
	 {10, 128, 128, 1},
	 {10, 128, 128, 1},
	 {255, 255, 255, 0},
	 {10, 128, 128, 1}};

#if WIFI_ROUTER
Wifi_Router wifi_iD_router = {
	 "*******",
	 "********************************",
	 {10, 128, 10, 47},
	 {10, 128, 10, 1},
	 {255, 255, 255, 0},
	 {10, 128, 10, 1}};
#else
Wifi_Ap wifi_ip_AP = {
	 "ESP_TERM",
	 "********************************",
	 {10, 128, 128, 1},
	 {10, 128, 128, 1},
	 {255, 255, 255, 0},
	 {10, 128, 128, 1}};
#endif
*/
void Wifi_::WiFi_Init()
{
	// Попытка подключения к точке доступа
	WiFi.mode(WIFI_STA);
	WiFi.config(wifi_iD_router.IPADR, wifi_iD_router.IPROUTER, wifi_iD_router.netmaska, wifi_iD_router.DNS_1);
	byte tries = 50;
	WiFi.begin(wifi_iD_router._ssid.c_str(), wifi_iD_router._password.c_str());

	// Делаем проверку подключения до тех пор пока счетчик tries
	// не станет равен нулю или не получим подключение
	while (--tries && WiFi.status() != WL_CONNECTED)
	{
		Serial.print(".");
		delay(1000);
	}
	if (WiFi.status() != WL_CONNECTED)
	{
		// Если не удалось подключиться запускаем в режиме AP
		Serial.println("");
		Serial.println("WiFi up AP");
		StartAPMode();
	}
	else
	{
		// Иначе удалось подключиться отправляем сообщение
		// о подключении и выводим адрес IP
		Serial.println("");
		Serial.println("WiFi connected");
		Serial.print("IP address: ");
		Serial.println(WiFi.localIP());
		Serial.print("IP address: ");
		Serial.print("ESP Mac Address: ");
		Serial.println(WiFi.macAddress());
		Serial.print("Subnet Mask: ");
		Serial.println(WiFi.subnetMask());
		Serial.print("Gateway IP: ");
		Serial.println(WiFi.gatewayIP());
		Serial.print("DNS: ");
		Serial.println(WiFi.dnsIP());
	}
};

void Wifi_::StartAPMode()
{ // Отключаем WIFI
	WiFi.disconnect();
	// Меняем режим на режим точки доступа
	WiFi.mode(WIFI_AP);
	// Задаем настройки сети
	Serial.println();
	Serial.println("Configuring access point...");
	WiFi.softAPdisconnect();

	Serial.print("Setting soft-AP configuration ... ");
	//  "Задаем настройки программной точки доступа ... "
	Serial.println(WiFi.softAPConfig(wifi_id_AP.IP_AP, wifi_id_AP.IPADR_AP, wifi_id_AP.netmaska) ? "OK!" : "FIELD!");
	//  "Готово" : "Задать настройки не удалось"
	Serial.print("Setting soft-AP ... ");

	//  "Настройка программной точки доступа ... "
	//const char *ssid = "ESP_Term";
	//const char *password = "123456789";
	Serial.println(WiFi.softAP(wifi_id_AP.ssid, wifi_id_AP.password) ? "OK!" : "FIELD!");
	//  "Готово" : "Настройка не удалась"

	Serial.print("Soft-AP IP address = ");
	//  "IP-адрес программной точки доступа = "
	Serial.println(WiFi.softAPIP());
};
