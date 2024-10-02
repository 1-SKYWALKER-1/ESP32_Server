#include <WiFi.h>              
#include <WebServer.h>          
#include <ArduinoJson.h>

const char* ssid = "Kyivstar_38";
const char* password = "zhenya06041982";
String queryExample = "For example ip/car?direction=forward";

WebServer server(80);

void configureRoutes() {
  server.on("/car", handleDirection);
  server.on("/", handleRoot);
}

void handleRoot() {
  sendData(200, "Existing path: /car\n" + queryExample)
}

void sendData(int code, String text){
    StaticJsonDocument<100> JSONData;
    JSONData["code"] = code;
    JSONData["text"] = text;
    char data[100];
    serializeJson(JSONData, data);
    server.send(code, "application/json", data);
}

void determineDirection(String direction) {
    if(direction == "forward") {
      Serial.println("forward"); 
    } else if(direction == "left") {
      Serial.println("left");  
    } else if(direction == "right") {
      Serial.println("right");
    } else if (direction == "back") {
      Serial.println("back");
    } else {
      sendData(404, "Existing directions: left, forward, right, back!");
      return;
    }
    sendData(200, "car move " + direction);
}

void handleDirection() {
  if (!server.hasArg("direction")) {
    sendData(404, "Not Found, you should add parameter \"direction\"\n" + queryExample);
    return;
  }

  String direction = server.arg("direction");
  determineDirection(direction)
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
  // Handle client requests
  server.handleClient();
}