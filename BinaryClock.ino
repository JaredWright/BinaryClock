#include <Wire.h>
#include "RTClib.h"
#include <LiquidCrystal.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

RTC_DS1307 RTC;

/*
 * Pin mapping for Arduino Uno
 */
#define LCD_RS_PIN      12
#define LCD_ENABLE_PIN  11
#define HOUR_PIN        10
#define MINUTE_PIN      9
#define SECONDS_PIN     8
#define LCD_DATA_4_PIN  5
#define LCD_DATA_3_PIN  4
#define LCD_DATA_2_PIN  3
#define LCD_DATA_1_PIN  2

/*
 * Global state for NeoPixel LED Strips (the Clock's face)
 */
#define STRIP_LENGTH 59
Adafruit_NeoPixel hour_strip = Adafruit_NeoPixel(STRIP_LENGTH, HOUR_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel minute_strip = Adafruit_NeoPixel(STRIP_LENGTH, MINUTE_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel seconds_strip = Adafruit_NeoPixel(STRIP_LENGTH, SECONDS_PIN, NEO_GRB + NEO_KHZ800);
uint32_t hour_bit_color = minute_strip.Color(255, 0, 0);
uint32_t minute_bit_color = minute_strip.Color(0, 255, 0);
uint32_t seconds_bit_color = minute_strip.Color(255, 255, 255);
uint32_t background_color = minute_strip.Color(0, 255, 255);

/*
 * Global state for the LCD Screen
 */
 #define TITLE    "Binary Clock"
 #define VERSION  "1.0"
 #define AUTHORS  "Gerrrard, Soupav"
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

/*
 * Initialization
 */
void setup() {
  Serial.begin(115200);
  Wire.begin();
  initialize_strips();
  initialize_lcd();
}

/*
 * Main loop
 */
void loop() {
    DateTime current_time = RTC.now();
    update_clock_face(current_time);
}

/*
 * Initializes all LED strips with clock bits off and background bits on
 */
void initialize_strips()
{
  hour_strip.begin();
  minute_strip.begin();
  seconds_strip.begin();

  // Initialize all bits to the background color
  for(int i = 0; i < STRIP_LENGTH; ++i){
    hour_strip.setPixelColor(i, background_color);
  }
  for(int i = 0; i < STRIP_LENGTH; ++i){
    minute_strip.setPixelColor(i, background_color);
  }
  for(int i = 0; i < STRIP_LENGTH; ++i){
    seconds_strip.setPixelColor(i, background_color);
  }

  // Turn all of the non-background bits off
  for(int i = 0; i < 18; ++i){
    toggle_bit(i, false);
  }
  
  hour_strip.show();
  minute_strip.show();
  seconds_strip.show();
}

/*
 * Updates all bits on the clock's face, according to the given time
 */
void update_clock_face(DateTime current_time)
{
  int hours = current_time.hour();
  int minutes = current_time.minute();
  int seconds = current_time.second();

  for(unsigned int i = 0; i < 6; ++i){
      // Hours
      if(1 & (hours >> i)){
          toggle_bit(5 - i, true);
      }else{
          toggle_bit(5 - i, false);
      }
    
      // Minutes
      if(1 & (minutes >> i)){
          toggle_bit(11 - i, true);
      }else{
          toggle_bit(11 - i, false);
      }

      // Seconds
      if(1 & (seconds >> i)){
          toggle_bit(17 - i, true);
      }else{
          toggle_bit(17 - i, false);
      }
  }
  
  hour_strip.show();
  minute_strip.show();
  seconds_strip.show();
}

/* Toggles the given bit on or off. Bits are numbered 0 - 17 and organized on the clock as follows: 
 *      Hours Strip:      ----0----1----2----3----4----5----    (Top)
 *      Minutes Strip:    ----6----7----8----9----10---11---
 *      Seconds Strip:    ----12---13---14---15---16---17---    (Bottom)
*/
void toggle_bit(uint8_t bit_num, bool on)
{
  uint16_t bit_pos;
  Adafruit_NeoPixel * strip = NULL;
  uint32_t color;
  
  if(bit_num >= 0 && bit_num < 6){
    bit_pos = (60 - (bit_num * 10)) - 6;
    strip = &hour_strip;
    color = hour_bit_color;
  }else if(bit_num >=6 && bit_num < 12){
    bit_pos = (60 - ((bit_num - 6) * 10)) - 6;
    strip = &minute_strip;
    color = minute_bit_color;
  }else if(bit_num >= 12 && bit_num < 18){
    bit_pos = (60 - ((bit_num - 12) * 10)) - 6;
    strip = &seconds_strip;
    color = seconds_bit_color;
  }else{
    return;
  }
  
  for(int i = 0; i < 5; ++i){
    if(on){
      strip->setPixelColor(bit_pos + i, color);
    }else{
      strip->setPixelColor(bit_pos + i, 0);
    }
  }
}

/*
 * Initialization for the 16x2 LCD screen. Prints a static welcome message for now...
 */
void initialize_lcd()
{
  // Initialize LCD Columns/Rows
  lcd.begin(16, 2);
  print_welcome_page();
}

/*
 * Print a static welcome page to the LCD
 */
void print_welcome_page()
{
  // Print title, version, and authors
  lcd.setCursor(0, 0);
  lcd.print(TITLE);
  lcd.print(" ");
  lcd.print(VERSION);
  lcd.setCursor(0, 1);
  lcd.print(AUTHORS);
}

