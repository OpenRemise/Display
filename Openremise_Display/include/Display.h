#pragma once

#include <Arduino.h>
#include <stdio.h>
#include <U8g2lib.h>
#include <Wire.h>
#include "Logo.h" // Include logo bitmaps

// Define the OpenRemiseDisplay class
class OpenRemiseDisplay {
 

    public:
         // Konstruktor
   OpenRemiseDisplay();

   // basis Methoden
    uint8_t begin();
    uint8_t update();
    uint8_t clear();
   // Anzeige Methoden
    void ShowError();
    void ShowWelcome();
    void ShowLearnMode();
    void ShowNormalMode(); 
    void ShowNetwork();
    
      
private:
    U8G2_SH1107_SEEED_128X128_1_HW_I2C u8g2;
    bool initialized_display;
}; 
