#include "FS_.h"

// Для файловой системы
File fsUploadFile;

//Выбрать систему ао которой будем работать
//FS fss;
//LittleFS fs;
#if FS_FS
FS fss = SPIFFS;
#else
FS fss = LittleFS;
#endif

// Инициализация FFS
void FS_init()
{
	delay(500);

	Serial.println("Inizializing FS...");
	fss.begin();

	Serial.println("done.");
}
// Здесь функции для работы с файловой системой
String getContentType(String filename)
{
	if (filename.endsWith(".htm"))
		return "text/html";
	else if (filename.endsWith(".html"))
		return "text/html";
	else if (filename.endsWith(".json"))
		return "application/json";
	else if (filename.endsWith(".css"))
		return "text/css";
	else if (filename.endsWith(".js"))
		return "application/javascript";
	else if (filename.endsWith(".png"))
		return "image/png";
	else if (filename.endsWith(".gif"))
		return "image/gif";
	else if (filename.endsWith(".jpg"))
		return "image/jpeg";
	else if (filename.endsWith(".ico"))
		return "image/x-icon";
	else if (filename.endsWith(".xml"))
		return "text/xml";
	else if (filename.endsWith(".php"))
		return "text/xml";
	else if (filename.endsWith(".pdf"))
		return "application/x-pdf";
	else if (filename.endsWith(".zip"))
		return "application/x-zip";
	else if (filename.endsWith(".gz"))
		return "application/x-gzip";
	return "text/plain";
}

/*
// Запись данных в файл config.json
bool saveConfigFile(String configJson)
{
	// Open file for writing
	File file = SPIFFS.open(configJson.c_str(), "W");
	;
	if (!file)
	{
		Serial.println("Failed to create file");
		return;
	}

	// Allocate a temporary JsonDocument
	// Don't forget to change the capacity to match your requirements.
	// Use arduinojson.org/assistant to compute the capacity.
	StaticJsonDocument<256> doc;

	// Set the values in the document
	doc["hostname"] = config.hostname;
	doc["port"] = config.port;

	// Serialize JSON to file
	if (serializeJson(doc, file) == 0)
	{
		Serial.println(F("Failed to write to file"));
	}

	// Close the file
	file.close();

	// Резервируем память для json обекта буфер может рости по мере необходимти предпочтительно для ESP8266
	DynamicJsonBuffer jsonBuffer;
	//  вызовите парсер JSON через экземпляр jsonBuffer
	JsonObject &json = jsonBuffer.parseObject(configJson);
	// Заполняем поля json
	json["SSDP"] = SSDP_Name;
	json["ssidAP"] = _ssidAP;
	json["passwordAP"] = _passwordAP;
	json["ssid"] = _ssid;
	json["password"] = _password;
	json["timezone"] = time_zone;
	json["t0_min"] = t0_min;
	json["t1_min"] = t1_min;
	json["t_timer"] = ms;
	// Помещаем созданный json в глобальную переменную json.printTo(jsonConfig);
	json.printTo(configJson);
	// Открываем файл для записи
	File configFile = SPIFFS.open(configJson.c_str(), "w");
	if (!configFile)
	{
		Serial.println("Failed to not open config file for writing");
		configFile.close();
		return false;
	}
	// Записываем строку json в файл
	json.printTo(configFile);
	configFile.close();
	return true;
}*/

void fileUpload(HTTPUpload &upload)
{
	if (upload.status == UPLOAD_FILE_START)
	{
		String filename = upload.filename;
		if (!filename.startsWith("/"))
			filename = "/" + filename;
		fsUploadFile = fss.open(filename, "w");
		filename = String();
	}
	else if (upload.status == UPLOAD_FILE_WRITE)
	{
		//DBG_OUTPUT_PORT.print("handleFileUpload Data: "); DBG_OUTPUT_PORT.println(upload.currentSize);
		if (fsUploadFile)
			fsUploadFile.write(upload.buf, upload.currentSize);
	}
	else if (upload.status == UPLOAD_FILE_END)
	{
		if (fsUploadFile)
			fsUploadFile.close();
	}
}

bool saveFile(String JsonFile, String Data)
{
	File f = fss.open(JsonFile.c_str(), "w");
	if (!f)
	{
		Serial.println("file open failed"); //  "открыть файл не удалось"
	}
	else
	{

		Serial.println("File data_01.json - Failed to open");
		return false;
	}
}

// Загрузка данных сохраненных в файл
bool loadFile(String fileJson, String Data)
{

	//File configFile = SPIFFS.open("/config.json", "r");
	File configFile = fss.open(fileJson.c_str(), "r");
	if (!configFile)
	{
		// если файл не найден
		Serial.println("Failed to open config file");
		//  Создаем файл запиав в него данные по умолчанию
		//saveConfigFile(configJson);
		configFile.close();
		return false;
	}
	// Проверяем размер файла, будем использовать файл размером меньше 1024 байта
	size_t size = configFile.size();
	if (size > 1024)
	{
		Serial.println("Config file size is too large");
		configFile.close();
		return false;
	}
	Serial.println("its read!!");
	// загружаем файл конфигурации в переменную
	Data = configFile.readString();

	configFile.close();
	return true;
}

void readFile_Serial(String path)
{
	Serial.println("Reading file: " + path);

	File file = fss.open(path, "r");
	if (!file)
	{
		Serial.println("Failed to open file for reading");
		return;
	}

	Serial.print("Read from file: ");
	while (file.available())
	{
		Serial.print(file.readString());
	}
	file.close();
}

void writeFile(String path, String message)
{
	Serial.println("Writing file: " + path);

	File file = fss.open(path, "w");
	if (!file)
	{
		Serial.println("Failed to open file for writing");
		return;
	}
	if (file.print(message))
	{
		Serial.println("File written");
	}
	else
	{
		Serial.println("Write failed");
	}
	delay(500); // Make sure the CREATE and LASTWRITE times are different
	file.close();
}

void appendFile(const char *path, const char *message)
{
	Serial.printf("Appending to file: %s\n", path);

	File file = fss.open(path, "a");
	if (!file)
	{
		Serial.println("Failed to open file for appending");
		return;
	}
	if (file.print(message))
	{
		Serial.println("Message appended");
	}
	else
	{
		Serial.println("Append failed");
	}
	file.close();
}

void renameFile(const char *path1, const char *path2)
{
	Serial.printf("Renaming file %s to %s\n", path1, path2);
	if (fss.rename(path1, path2))
	{
		Serial.println("File renamed");
	}
	else
	{
		Serial.println("Rename failed");
	}
}

void deleteFile(const char *path)
{
	Serial.printf("Deleting file: %s\n", path);
	if (fss.remove(path))
	{
		Serial.println("File deleted");
	}
	else
	{
		Serial.println("Delete failed");
	}
}

// To format all space in SPIFFS
// fs.format()
/*
	// Get all information of your SPIFFS or LittleFS
	FSInfo fs_info;
	fss.info(fs_info);

	Serial.println("File sistem info.");
	char str[60];
	sprintf(str, "%s %s byte", "Total space:      ", fs_info.totalBytes);
	Serial.println(str);

	sprintf(str, "%s %s byte", "Total space used:      ", fs_info.usedBytes);
	Serial.println(str);

	sprintf(str, "%s %s byte", "Block size:       ", fs_info.blockSize);
	Serial.println(str);

	sprintf(str, "%s %s byte", "Page size:       ", fs_info.totalBytes);
	Serial.println(str);

	sprintf(str, "%s %s byte", "Max open files       ", fs_info.maxOpenFiles);
	Serial.println(str);

	sprintf(str, "%s %s byte", "Max path lenght:       ", fs_info.maxPathLength);
	Serial.println(str);

	Serial.println();

	// Open dir folder
	
	Serial.println("Open DIR");
	Dir dir = fss.openDir("/");
	// Cycle all the content
	while (dir.next())
	{
		// get filename
		Serial.println(dir.fileName());
		/*Serial.print(" - ");
		// If element have a size display It else write 0
		if (dir.fileSize())
		{
			File f = dir.openFile("r");
			char str[60];
			sprintf(str, "%s byte", f.size());
			Serial.println(str);
			//Serial.println(f.size());
			f.close();
		}
		else
		{
			Serial.println("0");
		}
	}*/