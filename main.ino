/*
Author: R. Eren Arslan
Nov. 15 2024
*/

#include <Wire.h>        // Include the Wire library for I2C communication
#include <U8g2lib.h>     // Include the U8g2 library for OLED display
#include <DHT.h>         // Include the DHT library for DHT sensor

#define SCREEN_WIDTH 128    // Width of the OLED display
#define SCREEN_HEIGHT 64    // Height of the OLED display

// Initialize U8g2 for SSD1306 OLED using software I2C
U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ 10, /* data=*/ 8, /* reset=*/ U8X8_PIN_NONE);

#define DHTPIN 6        // Pin connected to the DHT sensor
#define DHTTYPE DHT11   // DHT 11 type sensor
DHT dht(DHTPIN, DHTTYPE); // Create DHT object

void setup() {
  Serial.begin(9600);   // Initialize serial communication
  dht.begin();          // Start the DHT sensor
  u8g2.begin();         // Start the OLED display
}

void loop() {
  delay(2000);          // Delay to stabilize readings

  // Read temperature and humidity from the sensor
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  
  // Calculate heat index ("feels-like" temperature)
  float heatIndex = dht.computeHeatIndex(temperature, humidity, false);

  // Clear the display buffer
  u8g2.clearBuffer();

  // Set font for the OLED
  u8g2.setFont(u8g2_font_6x12_tf);

  // Display temperature
  u8g2.setCursor(0, 20);
  u8g2.print("Temp: ");
  u8g2.print(temperature);
  u8g2.print(" C");

  // Display humidity
  u8g2.setCursor(0, 35);
  u8g2.print("Humidity: ");
  u8g2.print(humidity);
  u8g2.print("%");

  // Display heat index
  u8g2.setCursor(0, 50);
  u8g2.print("Feels Like: ");
  u8g2.print(heatIndex);
  u8g2.print(" C");

  // Display icon based on temperature range
  if (temperature > 30) {
    u8g2.drawCircle(100, 20, 10);  // Draw a sun icon for hot weather
  } else if (temperature < 15) {
    u8g2.drawDisc(100, 20, 10);    // Draw a filled circle for cold weather
  } else {
    u8g2.drawFrame(90, 10, 20, 20); // Draw a square for moderate weather
  }

  // Send the buffer to the OLED
  u8g2.sendBuffer();

  // Print readings to the serial monitor (optional for debugging)
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" C");
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println("%");
  Serial.print("Heat Index: ");
  Serial.print(heatIndex);
  Serial.println(" C");
}
