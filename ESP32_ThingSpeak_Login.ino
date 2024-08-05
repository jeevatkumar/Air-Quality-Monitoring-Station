#include <WiFi.h>
#include <HTTPClient.h>

// ThingSpeak settings
const char* server = "http://api.thingspeak.com";
const char* apiKey = "YHFENDIOMDHS";  // Replace with your ThingSpeak Write API Key
const int channelID = 1234567;  // Replace with your ThingSpeak channel ID

// Preset module credentials
const char* correctModuleName = "Station 1";
const char* correctModulePassword = "Electronics";

// Variables to hold user input
String inputString = ""; // A string to hold incoming data
bool stringComplete = false; // Whether the string is complete
bool wifiConnected = false; // Flag to track WiFi connection status
bool moduleLoggedIn = false; // Flag to track module login status

// Variables to store WiFi credentials
String ssid; // Store WiFi SSID
String wifiPassword; // Store WiFi password

void setup() {
  Serial.begin(115200); // Initialize serial communication at 115200 baud rate
  Serial.println("Welcome to the ESP32 Login System!");
  Serial.println("Please enter WiFi SSID: ");
}

void loop() {
  // Handle serial input directly in the loop
  while (Serial.available()) {
    char inChar = (char)Serial.read(); // Get the new byte
    inputString += inChar; // Add it to the inputString
    if (inChar == '\n') { // Check for newline
      inputString.trim(); // Remove any trailing newline or spaces
      stringComplete = true;
    }
  }

  if (stringComplete) {
    if (!wifiConnected) {
      if (ssid.isEmpty()) {
        ssid = inputString; // Store the entered SSID
        Serial.println("Enter WiFi Password: ");
      } else if (wifiPassword.isEmpty()) {
        wifiPassword = inputString; // Store the entered WiFi password
        connectToWiFi(ssid.c_str(), wifiPassword.c_str());
      }
    } else if (!moduleLoggedIn) {
      static String moduleName = "";

      if (moduleName.isEmpty()) {
        moduleName = inputString; // Store the entered module name

        // Check if module name is correct
        if (moduleName.equals(correctModuleName)) {
          Serial.println("Enter Module Password: ");
        } else {
          Serial.println("Incorrect Module Name. Please try again.");
          moduleName = ""; // Reset module name for re-entry
          Serial.println("Enter Module Name: ");
        }
      } else {
        if (inputString.equals(correctModulePassword)) {
          moduleLoggedIn = true; // Successful module login
          Serial.println("Module login successful!");
          Serial.println("System is now fully operational.");
          // Add additional functionality or tasks here
          sendToThingSpeak(0); // Send dummy data as a placeholder
        } else {
          Serial.println("Incorrect Module Password. Please try again.");
          moduleName = ""; // Reset module name for re-entry
          Serial.println("Enter Module Name: ");
        }
      }
    } else {
      // Handle other input if needed when already logged in
      Serial.println("You are already logged in.");
    }

    // Clear the input string and reset flag
    inputString = "";
    stringComplete = false;
  }
}

// Function to connect to WiFi
void connectToWiFi(const char* ssid, const char* password) {
  Serial.print("Connecting to WiFi...");
  WiFi.begin(ssid, password);

  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 10) {
    delay(1000);
    Serial.print(".");
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    wifiConnected = true;
    Serial.println("\nConnected to WiFi successfully!");
    Serial.println("Enter Module Name: ");
  } else {
    Serial.println("\nFailed to connect to WiFi. Please check the SSID and password.");
    this->ssid = ""; // Reset SSID for re-entry
    this->wifiPassword = ""; // Reset WiFi password for re-entry
    Serial.println("Please enter WiFi SSID: ");
  }
}

// Function to send data to ThingSpeak
void sendToThingSpeak(int value) {
  if (WiFi.status() == WL_CONNECTED) { // Check WiFi connection status
    HTTPClient http;

    String url = String(server) + "/update?api_key=" + apiKey + "&field1=" + String(value);
    http.begin(url.c_str());

    int httpResponseCode = http.GET();
    if (httpResponseCode > 0) {
      Serial.println("Data sent successfully!");
    } else {
      Serial.println("Error sending data.");
    }
    http.end();
  } else {
    Serial.println("WiFi not connected.");
  }
}
