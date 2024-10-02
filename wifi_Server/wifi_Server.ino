#include <WiFi.h>              // For ESP32
#include <WebServer.h>          // Synchronous WebServer library

// Replace with your network credentials
const char* ssid = "Kyivstar_38";
const char* password = "zhenya06041982";
String queryExample = "For example ip/car?direction=forward";
// Create a WebServer object on port 80
WebServer server(80);
void handleRoot(){
  server.send(200, "text/plain", "Existing path: /car\n" + queryExample);
}
void handleDirection(){
  if(server.hasArg("direction")){
    String direction = server.arg("direction");
    
      if(direction == "forward") {
        
      }else if(direction == "left"){
          
      }else if(direction == "right"){
          
      }else if (direction == "back"){
          
      }else {
        server.send(404, "text/plain", "Existing directions: left, forward, right, back!");
        return;
        }
        server.send(200, "text/plain", "car move " + direction);
  } else{
    server.send(404, "text/plain", "Not Found, you should add parameter \"direction\"\n" + queryExample);
  }
}

void setup() {
  // Start the serial communication
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Define routes
  server.on("/car",handleDirection);
  server.on("/",handleRoot);
  // Start the server
  server.begin();
  Serial.println("HTTP server started");
  Serial.println(WiFi.localIP());
}

void loop() {
  // Handle client requests
  server.handleClient();
}