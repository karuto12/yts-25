#include <WiFi.h>
#include <WebServer.h>
#include <WebSocketsServer.h>
#include <FS.h>
#include <SPIFFS.h>

// Access Point credentials
const char *ssid = "ESP32-Chat";
const char *password = "12345678";

WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);

void onMessage(uint8_t num, WStype_t type, uint8_t *payload, size_t length) {
  switch (type) {
    case WStype_CONNECTED: {
      IPAddress ip = webSocket.remoteIP(num);
      Serial.printf("Client %u connected from %s\n", num, ip.toString().c_str());
      webSocket.sendTXT(num, "Welcome to ESP32 Chat!");
      break;
    }

    case WStype_TEXT: {
      IPAddress ip = webSocket.remoteIP(num);
      String msg = String((char *)payload);
      Serial.printf("Received from %s: %s\n", ip.toString().c_str(), msg.c_str());
      webSocket.broadcastTXT("[" + ip.toString() + "]: " + msg);
      break;
    }

    case WStype_DISCONNECTED: {
      Serial.printf("Client %u disconnected\n", num);
      break;
    }

    default:
      break;
  }
}


void setup() {
  Serial.begin(115200);
  
  // Start file system
  if (!SPIFFS.begin(true)) {
    Serial.println("SPIFFS mount failed");
    return;
  }
  IPAddress local_ip(192, 168, 10, 1);       // Your desired IP
  IPAddress gateway(192, 168, 10, 1);        // Usually same as local_ip in AP mode
  IPAddress subnet(255, 255, 255, 0);        // Standard subnet

  WiFi.softAPConfig(local_ip, gateway, subnet);


  // Set up Access Point
  WiFi.softAP(ssid, password);
  Serial.println("AP IP address: " + WiFi.softAPIP().toString());

  // Serve index.html
  server.on("/", HTTP_GET, []() {
    File file = SPIFFS.open("/index.html", "r");
    server.streamFile(file, "text/html");
    file.close();
  });

  server.begin();
  webSocket.begin();
  webSocket.onEvent(onMessage);
}

void loop() {
  server.handleClient();
  webSocket.loop();
}
