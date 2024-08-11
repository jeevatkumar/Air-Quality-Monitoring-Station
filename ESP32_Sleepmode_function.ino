#include "arduino_secrets.h"

#include <Arduino.h> // Include Arduino library for core functions

const uint32_t TIME_TO_SLEEP = 60; // Define sleep duration in seconds
const uint32_t uS_TO_S_FACTOR = 1000000; // Conversion factor for seconds to microseconds

RTC_DATA_ATTR int bootCount = 0; // Persistent boot counter variable

void setup() {
  Serial.begin(115200); // Initialize serial communication
  delay(500); // Brief delay for serial monitor to open

  // Increment boot count and print it
  ++bootCount;
  Serial.println("Boot number: " + String(bootCount));

  // Print wakeup reason
  print_wakeup_reason();

  // Configure deep sleep
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  Serial.println("Sleep for " + String(TIME_TO_SLEEP) + " seconds");

  // Generate random value
  int random_value = random(10, 31);
  Serial.println("Random value: " + String(random_value));

  Serial.println("Going to sleep");
  Serial.flush(); // Ensure data is sent
  esp_deep_sleep_start(); // Enter deep sleep
}

void loop() {
  // This function is never called
}

void print_wakeup_reason() {
  esp_sleep_wakeup_cause_t reason; // Wakeup reason
  reason = esp_sleep_get_wakeup_cause(); // Get wakeup reason

  switch (reason) {
    case ESP_SLEEP_WAKEUP_TIMER: Serial.println("Wakeup by timer"); break;
    default: Serial.printf("Unexpected wakeup: %d\n", reason); break;
  }
}
