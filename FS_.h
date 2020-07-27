#pragma once
// Начнем с директив препроцессора. __WIFI_H – это произвольное уникальное имя (обычно используется имя заголовочного файла)
#ifndef __FS__H
#define __FS__H

#include <iostream>
#include "Setting_All.h"
//#include <Fs.h>
#include <LittleFS.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>

// Инициализация FFS
void FS_init();
// Здесь функции для работы с файловой системой
String getContentType(String filename);
/*bool handleFileRead(String path);
		void handleFileUpload();
		void handleFileDelete();
		void handleFileCreate();
		void handleFileList();*/
// Загрузка данных сохраненных в файл
bool loadFile(String fileJson, String Data);
// Запись данных в файл config.json
//bool saveConfigFile(String configJson);
bool saveFile(String JsonFile, String Data);
// Загрузка данных сохраненных в файл  *.json
//bool loadFile(String fileJson);
void fileUpload(HTTPUpload &upload);

void readFile_Serial(String path);
void writeFile(String path, String message);
void appendFile(const char *path, const char *message);
void renameFile(const char *path1, const char *path2);
void deleteFile(const char *path);

// Заканчиваем директивой
#endif // __FS__H
