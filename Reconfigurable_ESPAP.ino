#include <WiFi.h>
#include <SPIFFS.h>
#include <WebServer.h>

const char* ssid = "ESP32-AP";
const char* password = "12345678";

WebServer server(80);

// Serve files from SPIFFS
void serveFile(String path) {
  if (path.endsWith("/")) path += "index.html";
  String contentType = "text/plain";
  if (path.endsWith(".html")) contentType = "text/html";
  else if (path.endsWith(".css")) contentType = "text/css";
  else if (path.endsWith(".js")) contentType = "application/javascript";

  File file = SPIFFS.open(path, "r");
  if (!file) {
    server.send(404, "text/plain", "File not found");
    return;
  }
  server.streamFile(file, contentType);
  file.close();
}

void setup() {
  Serial.begin(115200);
  WiFi.softAP(ssid, password);
  Serial.println("ESP32 AP IP: " + WiFi.softAPIP().toString());

  if (!SPIFFS.begin(true)) {
    Serial.println("SPIFFS failed to mount");
    return;
  }

  server.onNotFound([]() {
    serveFile(server.uri());
  });

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}
