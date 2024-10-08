#include <WiFi.h> // Include the WiFi library

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

        // Attempt to connect to WiFi
        Serial.println("Connecting to WiFi...");
        WiFi.begin(ssid.c_str(), wifiPassword.c_str());

        int attempts = 0;
        while (WiFi.status() != WL_CONNECTED && attempts < 10) {
          delay(1000);
          Serial.println("Attempting to connect...");
          attempts++;
        }

        if (WiFi.status() == WL_CONNECTED) {
          wifiConnected = true;
          Serial.println("Connected to WiFi successfully!");
          Serial.println("Enter Module Name: ");
        } else {
          Serial.println("Failed to connect to WiFi. Please check the SSID and password.");
          ssid = ""; // Reset SSID for re-entry
          wifiPassword = ""; // Reset WiFi password for re-entry
          Serial.println("Please enter WiFi SSID: ");
        }
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
