#pragma once
// Начнем с директив препроцессора. __WIFI_H – это произвольное уникальное имя (обычно используется имя заголовочного файла)
#ifndef __HTTP_H
#define __HTTP_H

#include <iostream>
#include "Setting_All.h"
#include "FS_.h"
//#include "Json_.h"

void HTTP_init();
void handleRoot();
void handle_set_t0_min();
void handle_set_t1_min();
void handle_set_ms();
void handleStatusPin();
void handle_Temp();
void handleADC_NEW();
void handleLED();
void handleWebRequests();
bool loadFromSpiffs(String path);
void handle_GET();
// Перезагрузка модуля по запросу вида http://192.168.0.101/restart?device=ok
void handle_Restart();

bool handleFileRead(String path);
void handleFileUpload();
void handleFileDelete();
void handleFileCreate();
void handleFileList();
// Заканчиваем директивой
#endif // __WIFI_H
