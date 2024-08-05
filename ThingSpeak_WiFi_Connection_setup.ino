#include <WiFi.h>
#include <HTTPClient.h>

//Network credentials
const char* ssid = "Jeevat";          // Your WiFi SSID
const char* password = "12345678";  // Your WiFi Password

// ThingSpeak settings
const char* server = "http://api.thingspeak.com";
const char* apiKey = "YHFENDIOMDHS";  // Replace with your ThingSpeak Write API Key
const int channelID = 1234567;  // Replace with your ThingSpeak channel ID

void setup() {
  Serial.begin(115200);
  connectToWiFi();
}

void loop() {
  // Main code here!!!
}

// Function to connect to WiFi
void connectToWiFi() {
  Serial.print("Connecting to WiFi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  
  Serial.println("Connected!");
  // Add additional code here if needed
}
