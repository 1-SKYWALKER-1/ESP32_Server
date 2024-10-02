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

void sendData(int code, String text){
    StaticJsonDocument<100> JSONData;
    JSONData["code"] = code;
    JSONData["text"] = text;
    char data[100];
    serializeJson(JSONData, data);
    server.send(code, "application/json", data);
}

void receiveData(){
   StaticJsonDocument<300> JSONData;
   // Deserialize the JSON document
   String jsonString = server.arg("plain");
  DeserializationError error = deserializeJson(JSONData, jsonString);
   
  // Test if parsing succeeds.
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    server.send(500,"application/json","Error in parsing");
    return;
  }else{
   if(JSONData.containsKey("direction")){
    Serial.println("HELOO MOTHERFUCker");
   // server.send(200,"application/json",String(JSONData["direction"].as<String>())+" Received");
    sendData(200,String(JSONData["direction"].as<String>())+" Received");
   }
  //  else{
  //    server.send(400,"application/json","Bad JSON");
    
   }
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
  determineDirection(direction);
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
