#include <WiFi.h>              
#include <WebServer.h>          
#include <ArduinoJson.h>

const char* ssid = "Kyivstar_38";
const char* password = "zhenya06041982";
String queryExample = "For example ip/car?direction=forward";

WebServer server(80);

void configureRoutes() {
  server.on("/car",HTTP_POST,receiveData);
  server.on("/", HTTP_GET, handleRoot);
}

void handleRoot() {
  sendData(200, "Existing path: /car\n" + queryExample);
}

void sendResponse(int code, String text) {
    StaticJsonDocument<100> JSONData;
    JSONData["code"] = code;
    JSONData["text"] = text;
    char data[100];
    serializeJson(JSONData, data);
    server.send(code, "application/json", data);
}

void receiveData() {
  StaticJsonDocument<300> JSONData;
  String jsonString = server.arg("plain");
  DeserializationError error = deserializeJson(JSONData, jsonString);
   
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    sendResponse(500, "Error in parsing")
    return;
  }
  
  if (JSONData.containsKey("direction")) {
    determineDirection(String(JSONData["direction"].as<String>()));
  } else {
    sendResponse(400, "Bad JSON");
  }
}

void determineDirection(String direction) {
    if (direction == "forward") {
      Serial.println("forward"); 
    } else if (direction == "left") {
      Serial.println("left");  
    } else if (direction == "right") {
      Serial.println("right");
    } else if (direction == "back") {
      Serial.println("back");
    } else {
      sendResponse(404, "Existing directions: left, forward, right, back!");
      return;
    }
    sendResponse(200, "Сar moving " + direction);
}

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  configureRoutes();

  server.begin();
  Serial.println("HTTP server started");
  Serial.println(WiFi.localIP());
}

void loop() {
  server.handleClient();
}
