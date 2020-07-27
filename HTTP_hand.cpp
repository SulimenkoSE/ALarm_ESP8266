#include "HTTP_hand.h"

ESP8266WebServer HTTP(80);
ESP8266HTTPUpdateServer httpUpdater;

extern int GPIO_RELE_TEN_1;
extern int GPIO_RELE_TEN_2;
extern int GPIO_RELE_TEN_3;
extern int GPIO_RELE_VENT_1;
extern int GPIO_RELE_VENT_2;

//extern int ADC_Pin;

extern int timezone; // часовой пояс GTM

extern char dataJson[];
extern char configJson[];
extern String File_configJson;
extern String File_dataJson;

extern FS fss;

void HTTP_init()
{
	//Start site
	HTTP.on("/", handleRoot);
	//Set setver all paths are not found so we can handle as per URI
	HTTP.onNotFound(handleWebRequests);
	//Reads ADC function is called from out index.html
	HTTP.on("/readADC", handleADC_NEW);
	//Reads Temperatur function is called from out index.html
	HTTP.on("/readTemperatura", handle_Temp);
	//Function is called from out index.html
	HTTP.on("/setLED", handleLED);
	HTTP.on("/statusPin", handleStatusPin);

	HTTP.on("/set_temp_0", handle_set_t0_min); //Установка минимальной температуры воды
	HTTP.on("/set_temp_1", handle_set_t1_min); //Установка минимальной температуры воздуха
	HTTP.on("/set_temp_ms", handle_set_ms);	 //Установка интервал азамеров в милисекундах

	HTTP.on("/restart", handle_Restart); // Перезагрузка модуля по запросу вида /restart?device=ok

	httpUpdater.setup(&HTTP); // Запускаем обновление

	HTTP.begin(); // Запускаем HTTP сервер

	//HTTP страницы для работы с FS
	//list directory
	HTTP.on("/list", HTTP_GET, handleFileList);
	//загрузка редактора editor
	HTTP.on("/edit", HTTP_GET, []() {
		if (!handleFileRead("/edit.htm"))
			HTTP.send(404, "text/plain", "FileNotFound");
	});
	//Создание файла
	HTTP.on("/edit", HTTP_PUT, handleFileCreate);
	//Удаление файла
	HTTP.on("/edit", HTTP_DELETE, handleFileDelete);
	//first callback is called after the request has ended with all parsed arguments
	//second callback handles file uploads at that location
	HTTP.on(
		 "/edit", HTTP_POST, []() {
			 HTTP.send(200, "text/plain", "");
		 },
		 handleFileUpload);
	//called when the url is not defined here
	//use it to load content from SPIFFS
	/*HTTP.onNotFound([]() {
		if (!handleFileRead(HTTP.uri()))
			HTTP.send(404, "text/plain", "FileNotFound");
	});*/
}

void handleRoot()
{
	String s; //Read HTML contents
	if (!handleFileRead("/index.html"))
	{
		HTTP.send(404, "text/plain", "FileNotFound");
	}
	else
	{
		HTTP.send(200, "text/html", s);
	}
}

void handle_set_t0_min()
{
	float t0_min = HTTP.arg("t0_min").toFloat();

	//js.jsonWrite(File_dataJson, "t0_min", t0_min);
	//fs_.saveFile(File_dataJson, dataJson);
}
void handle_set_t1_min()
{
	float t1_min = HTTP.arg("t1_min").toFloat();

	//js.jsonWrite(File_dataJson, "t1_min", t1_min);
	//fs_.saveFile(File_dataJson, dataJson);
}
void handle_set_ms()
{
	int ms = HTTP.arg("t_timer").toInt();

	//js.jsonWrite(File_dataJson, "ms", ms);
	//fs_.saveFile(File_dataJson, dataJson);
}

void handleStatusPin()
{
	String pinNumber = HTTP.arg("statusPin");
	//Serial.println(pinNumber);
	String t_state = "OFF";

	int val;
	if (pinNumber == "T1" || digitalRead(GPIO_RELE_TEN_1) == 1)
	{
		t_state = "ON"; //Включен
	}
	if (pinNumber == "T2" || digitalRead(GPIO_RELE_TEN_2) == 1)
	{
		t_state = "ON"; //Включен
	}
	if (pinNumber == "T3" || digitalRead(GPIO_RELE_TEN_3) == 1)
	{
		t_state = "ON"; //Включен
	}
	if (pinNumber == "V1" || digitalRead(GPIO_RELE_VENT_1) == 1)
	{
		t_state = "ON"; //Включен
	}
	if (pinNumber == "V1" || digitalRead(GPIO_RELE_VENT_2) == 1)
	{
		t_state = "ON"; //Включен
	}

	HTTP.send(200, "text/plane", t_state);
}

void handle_Temp()
{
	/*String Number = HTTP.arg("Num");
	if (Number == "0")
	{
		const char kl = js.jsonRead(dataJson, "t0");
		HTTP.send(200, "text/plane", kl);
	}
	if (Number == "1")
	{
		HTTP.send(200, "text/plane", js.jsonRead(dataJson, "t1"));
	}*/
}

void handleADC_NEW()
{
	/*int a = analogRead(A0);
	//a = map(a, 0, 1023, 0, 100);
	float b = a * 3.3 / 1024;
	String adcValue = String(b) + " V";
	HTTP.send(200, "text/plane", adcValue); //Send ADC value only to client ajax request*/
}

void handleLED()
{
	String t_state = HTTP.arg("LEDstate"); //Refer  xhttp.open("GET", "setLED?LEDstate="+led, true);
	//Serial.println(t_state);
	//if (t_state == "1")
	//{
	//	digitalWrite(GPIO_14, HIGH); //LED ON
	//	ledState = "ON"; //Feedback parameter
	//}
	//if (t_state == "0")
	//{
	//	digitalWrite(GPIO_14, LOW); //LED OFF
	//	ledState = "OFF"; //Feedback parameter
	//	timer1_disable();
	//}
	//if (t_state == "2") //StateOxrana OFF
	//{
	//	digitalWrite(GPIO_15, HIGH);
	//	Serial.println("The system is disarmed");
	//	alarmState = HIGH;
	//	ledState = "OFF";
	//}
	//if (t_state == "3")  //StateOxrana ON
	//{
	//	digitalWrite(GPIO_15, LOW);
	//	Serial.println("The system was set to security mode");
	//	alarmState = LOW;
	//	ledState = "ON";
	//}
	//HTTP.send(200, "text/plane", ledState); //Send web page
	HTTP.send(200, "text/plane", "ledState"); //Send web page
}

void handleWebRequests()
{
	if (loadFromSpiffs(HTTP.uri()))
		return;
	String message = "File Not Detected\n\n";
	message += "URI: ";
	message += HTTP.uri();
	message += "\nMethod: ";
	message += (HTTP.method() == HTTP_GET) ? "GET" : "POST";
	message += "\nArguments: ";
	message += HTTP.args();
	message += "\n";
	for (uint8_t i = 0; i < HTTP.args(); i++)
	{
		message += " NAME:" + HTTP.argName(i) + "\n VALUE:" + HTTP.arg(i) + "\n";
	}
	HTTP.send(404, "text/plain", message);
	Serial.println(message);
}

bool loadFromSpiffs(String path)
{
	String dataType = "text/plain";
	if (path.endsWith("/"))
		path += "index.htm";
	if (path.endsWith(".src"))
		path = path.substring(0, path.lastIndexOf("."));
	else if (path.endsWith(".html"))
		dataType = "text/html";
	else if (path.endsWith(".htm"))
		dataType = "text/html";
	else if (path.endsWith(".css"))
		dataType = "text/css";
	else if (path.endsWith(".js"))
		dataType = "application/javascript";
	else if (path.endsWith(".png"))
		dataType = "image/png";
	else if (path.endsWith(".gif"))
		dataType = "image/gif";
	else if (path.endsWith(".jpg"))
		dataType = "image/jpeg";
	else if (path.endsWith(".ico"))
		dataType = "image/x-icon";
	else if (path.endsWith(".xml"))
		dataType = "text/xml";
	else if (path.endsWith(".pdf"))
		dataType = "application/pdf";
	else if (path.endsWith(".zip"))
		dataType = "application/zip";
	File dataFile = fss.open(path.c_str(), "r");
	if (HTTP.hasArg("download"))
		dataType = "application/octet-stream";
	if (HTTP.streamFile(dataFile, dataType) != dataFile.size())
	{
	}

	dataFile.close();
	return true;
}
//добавлено 29.07.18 ======================================
void handle_GET()
{
	extern String File_configJson;
	std::string _SSDP;
	String data = HTTP.arg("plain");
#if (ARDUINOJSON_VERSION_MAJOR == 6)
	{
		// ArduinoJson 6
		StaticJsonDocument<1024> doc;
		DeserializationError error = deserializeJson(doc, configJson);
		if (error)
		{
			Serial.print("DeserializeJson() failed: ");
			Serial.println(error.c_str());
			return;
		}
		const char c = doc["value"];
		_SSDP.append(1, c);
	}
#else
	{
		// ArduinoJson 5
		StaticJsonBuffer<300> JSONbuffer; //Declaring static JSON buffer
		JsonObject &jObject = JSONbuffer.parseObject(data);
		_SSDP = jObject["SSDP"];
	}
#endif

	Serial.println(_SSDP.c_str());
	HTTP.send(200, "text/plain", "OK");
}

// Перезагрузка модуля по запросу вида http://192.168.0.101/restart?device=ok
void handle_Restart()
{
	String restart = HTTP.arg("device"); // Получаем значение device из запроса
	if (restart == "ok")
	{															  // Если значение равно Ок
		HTTP.send(200, "text / plain", "Reset OK"); // Oтправляем ответ Reset OK
		ESP.restart();										  // перезагружаем модуль
	}
	else
	{															  // иначе
		HTTP.send(200, "text / plain", "No Reset"); // Oтправляем ответ No Reset
	}
}

bool handleFileRead(String path)
{
	String contentType;
	if (path.endsWith("/"))
		path += "index.html";
	if (HTTP.hasArg("download"))
	{
		contentType = "application/octet-stream";
	}
	else
	{
		contentType = getContentType(path);
	}

	String pathWithGz = path + ".gz";
	if (fss.exists(pathWithGz) || fss.exists(path))
	{
		if (fss.exists(pathWithGz))
			path += ".gz";

		/*File testFile = fss.open(path, "r");
		if (testFile)
		{
			Serial.print("Read file content file ");
			Serial.println(testFile.fullName());
			//
         // File derivate from Stream so you can use all Stream method
         // readBytes, findUntil, parseInt, println etc
         //
			Serial.println(testFile.readString());
			Serial.println("Closed file!");
			testFile.close();
		}
		else
		{
			Serial.println("Problem on read file!");
		}*/

		File file = fss.open(path, "r");
		size_t sent = HTTP.streamFile(file, contentType);
		file.close();
		return true;
	}
	return false;
}

void handleFileUpload()
{
	File fsUploadFile;

	if (HTTP.uri() != "/edit")
		return;
	//Переход в файл Fs_.cpp
	fileUpload(HTTP.upload());
}

void handleFileDelete()
{
	if (HTTP.args() == 0)
		return HTTP.send(500, "text/plain", "BAD ARGS");
	String path = HTTP.arg(0);
	if (path == "/")
		return HTTP.send(500, "text/plain", "BAD PATH");
	if (!fss.exists(path))
		return HTTP.send(404, "text/plain", "FileNotFound");
	fss.remove(path);
	HTTP.send(200, "text/plain", "");
	path = String();
}

void handleFileCreate()
{
	if (HTTP.args() == 0)
		return HTTP.send(500, "text/plain", "BAD ARGS");
	String path = HTTP.arg(0);
	if (path == "/")
		return HTTP.send(500, "text/plain", "BAD PATH");
	if (fss.exists(path))
		return HTTP.send(500, "text/plain", "FILE EXISTS");
	File file = fss.open(path, "w");
	if (file)
		file.close();
	else
		return HTTP.send(500, "text/plain", "CREATE FAILED");
	HTTP.send(200, "text/plain", "");
	path = String();
}

void handleFileList()
{
	if (!HTTP.hasArg("dir"))
	{
		HTTP.send(500, "text/plain", "BAD ARGS");
		return;
	}

	String path = HTTP.arg("dir");
	Dir dir = fss.openDir(path);
	path = String();
	String output = "[";
	while (dir.next())
	{
		File entry = dir.openFile("r");
		if (output != "[")
			output += ',';
		bool isDir = false;
		output += "{\"type\":\"";
		output += (isDir) ? "dir" : "file";
		output += "\",\"name\":\"";
#if FS_FS
		output += String(entry.name()).substring(1); //2020 0 1
#else
		output += String(entry.name()).substring(0); //2020 0 1
#endif
		output += "\"}";
		entry.close();
	}
	output += "]";
	HTTP.send(200, "text/json", output);
}
