#include <WiFi.h>              // For ESP32
#include <WebServer.h>          // Synchronous WebServer library
#include <DNSServer.h>
// Replace with your network credentials
const char* ssid = "Kyivstar_38";
const char* password = "zhenya06041982";
IPAddress apIP(192, 168, 1, 4);
DNSServer dnsServer;
const char *server_name = "www.myesp32.com";

// Create a WebServer object on port 80
WebServer server(80);

void handleRoot() {
  server.send(200, "text/plain", "Hello, world");
}

void handleNotFound() {
  server.send(404, "text/plain", "Not Found");
}
void handleGreet() {
  String name = "Guest";
  String surname = "GuestSurname";
  if (server.hasArg("name")) {
    name = server.arg("name");
  }
   if (server.hasArg("surname")) {
    surname = server.arg("surname");
  }
  server.send(200, "text/plain", "Hello, " + name + " " + surname);
}

void setup() {
  // Start the serial communication
  Serial.begin(115200);
  const byte DNS_PORT = 53;
  //dnsServer.start(DNS_PORT, server_name, apIP);
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Define routes
  server.on("/", handleRoot); // Handle root route
  server.onNotFound(handleNotFound); // Handle undefined routes
  server.on("/greet", handleGreet);

  // Start the server
  server.begin();
  Serial.println("HTTP server started");
  Serial.println("local IP:" + WiFi.localIP());
  Serial.println("local subnetMask:" + WiFi.subnetMask());
  Serial.println("local WiFi gateWay:" + WiFi.gatewayIP());
}

void loop() {
  // Handle client requests
  server.handleClient();
  dnsServer.processNextRequest();
}