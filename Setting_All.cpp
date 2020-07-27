#include "Setting_All.h"

Wifi_Router wifi_iD_router = {
	 "AAAAAAA",
	 "12345648798",
	 {10, 128, 10, 47},
	 {10, 128, 10, 1},
	 {255, 255, 255, 0},
	 {10, 128, 10, 1}};

Wifi_Ap wifi_id_AP = {
	 "ESP_TERM",
	 "1234567898",
	 {10, 128, 128, 1},
	 {10, 128, 128, 1},
	 {255, 255, 255, 0},
	 {10, 128, 128, 1}};

//Для проверки вкл вайфай
const uint8_t GPIO_LED = 15;

// Для проверки поставим реле на ногu
int GPIO_RELE_TEN_1(5);
volatile byte TEN_1_state(LOW); //Нет напряжения

int GPIO_RELE_TEN_2(4);
volatile byte TEN_2_state(LOW); //Нет напряжения

int GPIO_RELE_TEN_3(6);
volatile byte TEN_3_state(LOW); //Нет напряжения

int GPIO_RELE_VENT_1(9);
volatile byte VENT_1_state(LOW); //Нет напряжения

int GPIO_RELE_VENT_2(10);
volatile byte VENT_2_state(LOW); //Нет напряжения

//int ADC_Pin(A0);

int timezone = 3; // часовой пояс GTM

char dataJson[1024];
char configJson[1024];
String File_configJson = "config.json";
String File_dataJson = "data_01.json";
