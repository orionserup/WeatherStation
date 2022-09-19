#include <Arduino.h>

#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <DHT_U.h>
#include <stdio.h>

#define DHT_DATA 7   // the GPIO of the DHT11 Data Pin
#define LCD_ADDRESS 0x27 // the i2c address of the display module

LiquidCrystal_I2C lcd(LCD_ADDRESS); // our LCD Display
DHT dht(DHT_DATA, DHT11); // our temperature and humidity sensor

void setup() {

  lcd.begin(16, 2, 1); 
  Serial.begin(9600); // begin the Serial Port for printing
  dht.begin(); // begin the dht11 data acquisition

}

void loop() {

  static float temp_f;  // store the temperature locally 
  static float humidity; // store the relative humidity 

  static char row1[16]; // first row of the LCD is 
  static char row2[16]; // the second row of the LCD

  temp_f = dht.readTemperature(true); // read the temp in farenheit thus the true
  humidity = dht.readHumidity(); // read the relative humidity in %

  lcd.clear(); // clear the LCD
  lcd.setCursor(1, 0); // rest the cursor position

  snprintf(row1, 16, "Temp: %2.2f", (double)temp_f);  // Write out the Temperature to the Print Buffer
  snprintf(row2, 16, "Humidity: %2.2f", (double)humidity);  // Write out the Humidity to the Print Buffer

  Serial.println(row1); // print the buffer to the Serial Monitor
  Serial.println(row2); // print the buffer to the Serial Monitor

  lcd.println(row1);  // print the Temp Message buffer to the LCD
  lcd.println(row2);  // print the Humidity Message Buffer to the LCD
  
  delay(2000); // wait a second between loops

}