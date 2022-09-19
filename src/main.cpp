#include <Arduino.h>

#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <DHT_U.h>
#include <stdio.h>

//#define WEATHERSTATION_SERVER

#ifdef WEATHERSTATION_SERVER

#define DHT_DATA 6  // the GPIO of the DHT11 Data Pin
DHT dht(DHT_DATA, DHT11); // our temperature and humidity sensor

#define PHOTOCELL_PIN A0
#define PHOTOCELL_R1 10000.0f
#define PHOTOCELL_DAY_THRESH 3000.0f

#endif

#define LCD_ADDRESS 0x27 // the i2c address of the display module
LiquidCrystal_I2C lcd(LCD_ADDRESS,2,1,0,4,5,6,7); // our LCD Display specific intializers for our I2C Driver/ Display

void setup() {

#ifdef WEATHERSTATION_SERVER
  dht.begin(); // begin the dht11 data acquisition
  pinMode(PHOTOCELL_PIN, INPUT);
#endif

  lcd.begin(16, 2);
  Serial.begin(9600); // begin the Serial Port for printing
  lcd.setBacklightPin(3,POSITIVE);
  lcd.setBacklight(HIGH);
  lcd.noBlink();

  lcd.home();
  lcd.print("Weather Station");

}

void loop() {

  #ifdef WEATHERSTATION_SERVER

  static float temp_f;  // store the temperature locally 
  static float humidity; // store the relative humidity 
  static float photocell_r; // the resistance of the photocell

  static char buffer[17]; // the write buffer to display on the LCD

  temp_f = dht.readTemperature(true); // read the temp in farenheit thus the true
  humidity = dht.readHumidity(); // read the relative humidity in %

  uint16_t v_photocell = analogRead(PHOTOCELL_PIN);
  photocell_r = PHOTOCELL_R1 * ((float)v_photocell / (1023.0f - (float)v_photocell)); // using voltage divider equation

  snprintf(buffer, 17, "H:%d%% T:%dF %s", (int)humidity, (int)temp_f, photocell_r < PHOTOCELL_DAY_THRESH? "Day ": "Nite");  // Write out the Humidity to the Print Buffer

  Serial.println(buffer); // print the buffer to the Serial Monitor

  lcd.setCursor(0, 1);
  lcd.print(buffer);  // print the Humidity Message Buffer to the LCD
  
  delay(2000); // wait a second between loops

  #else

  static char buffer[18] = {0};

  Serial.readBytes(buffer, 18);

  lcd.setCursor(0, 1); // set the cursor to the beginning of the appropriate row

  lcd.print(buffer); // print what we found with a null terminator
  
  #endif

}