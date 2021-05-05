/* automated_plant_watering_v1.ino
 *
 * Written by: Daniel Di Giovanni
 *
 * April 28, 2021
 *
 * This program automates plant watering through an Arduino-compatible
 * board. It dectects when soil is dry (via a moisture sensor) and
 * supplies water accordingly (via a pump).
 */

#include <Arduino.h>
#include <U8x8lib.h>

#define MOISTURE 0
#define REDLED 4
#define MOSFET 2
#define BUTTON 6

// Creates an instance of the U8X8_SSD1306_128X64_NONAME_HW_I2C class
auto display = U8X8_SSD1306_128X64_NONAME_HW_I2C(U8X8_PIN_NONE);

// Moisture values above this threshold mean the plant is dry
const int moistureThreshold = 600;
// When watering, the pump will be on for this many milliseconds
const int pumpTime = 2500;
// Perform measurements on this interval of milliseconds
const long measureInterval = 600000;

// Timing interval between measurements
unsigned long initialMilliseconds = 0;
unsigned long currentMilliseconds;

// Timing interval that pump stays on for
unsigned long initialPumpTime;
unsigned long currentPumpTime;

// True if plant is too dry
bool water = false;

void displayOLED(unsigned long initialTime, unsigned long currentTime) {
  /* Displays debug information on the OLED */

  /* Collect and calculate information to display */

  // Get moisture measurement
  int moistureValue = analogRead(MOISTURE);

  // Get time (in milliseconds since the initial time
  // This is explicitly cast to long to avoid confusion
  // This value will always fit in a long
  long timeSinceInitial = (long) (currentTime - initialTime);
  // Get time (in milliseconds) left until next measurement
  long timeToMeasuring = measureInterval - timeSinceInitial;

  // Convert milliseconds to minutes and seconds
  int minutes = (int) (timeToMeasuring/1000) / 60;
  int seconds = (int) (timeToMeasuring/1000 - minutes * 60);

  /* Format and display information */

  // Displat moisure value
  display.setCursor(0, 0);
  display.print("Moisture: ");
  display.setCursor(0, 2);
  display.print(moistureValue);
  // Display if the moisture value means the plant is wet or dry
  if (moistureValue < moistureThreshold) {
    display.print(" (wet)");
  }
  else {
    display.print(" (dry)");
  }
  // Display time until plant moisture is evaluated again
  display.setCursor(0, 4);
  display.print("Next water check");
  display.setCursor(0, 6);
  if (minutes < 10) {
    // If minutes is a single digit, display a "0" in front of it
    display.print("0");
    display.print(minutes);
  }
  else {
    display.print(minutes);
  }
  display.print(":");
  if (seconds < 10) {
    // If minutes is a single digit, display a "0" in front of it
    display.print("0");
    display.print(seconds);
  }
  else {
    display.print(seconds);
  }
}

void setup() {
  /* Initialize pins and OLED display */

  pinMode(MOISTURE, INPUT);                // Set moisture sensor pin to input
  pinMode(MOSFET, OUTPUT);                 // Set pump pin to output
  pinMode(BUTTON, INPUT);                  // Set button pin to input
  pinMode(REDLED, OUTPUT);                 // Set red LED pin to output

  display.begin();                         // Start the OLED display
  display.setFlipMode(1);                  // Set to 1 or 0, depending on orientation of board
  display.clearDisplay();                  // Clear display
  display.setFont(u8x8_font_7x14B_1x2_r);  // Set font
}

void loop() {
  /* Check the moisture levels of the plant, turn on the pump to water the plant
   * if it is too dry.
   * Display information on the OLED if requested.
   */

  // Get current time in milliseconds
  currentMilliseconds = millis();

  /* Checking if pump needs to be turned on */

  // Check if the measuring interval has passed
  // If it has, perform a measurement
  if (currentMilliseconds - initialMilliseconds > measureInterval) {
    // Set the new initial milliseconds for the next interval
    initialMilliseconds = currentMilliseconds;

    // Check if the current moisture reading is greater than the moisture threshold
    // If it is, then the plant is too dry
    if (analogRead(MOISTURE) > moistureThreshold) {
      // Plant needs to be watered
      water = true;

      // Time in milliseconds that the pump was turned on
      initialPumpTime = millis();

      // Turn on pump and red LED
      digitalWrite(MOSFET, HIGH);
      digitalWrite(REDLED, HIGH);
    }
    else {
      // Plant does not need to be watered
      water = false;
      // This is to make sure the pump is off when the plant is not dry
      digitalWrite(MOSFET, LOW);
      digitalWrite(REDLED, LOW);
    }
  }

  /* Checking if pump needs to be turned off */

  // Check if plant needs to be watered
  if (water == true) {
    // Check if the pump has been on for the watering interval
    currentPumpTime = millis();
    if (currentPumpTime - initialPumpTime > pumpTime) {
      // Plant no longer being watered
      water = false;
      digitalWrite(MOSFET, LOW);
      digitalWrite(REDLED, LOW);
    }
  }
  else {
    // Turn off pump and red LED
    digitalWrite(MOSFET, LOW);
    digitalWrite(REDLED, LOW);
  }

  /* Checking for button press (to display debug information on OLED) */

  // If button is pressed, display debug information on OLED
  if (digitalRead(BUTTON) == HIGH) {
    displayOLED(initialMilliseconds, currentMilliseconds);
  }
  else {
    display.clearDisplay();
  }
}
