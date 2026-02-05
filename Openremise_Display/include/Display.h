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
    uint8_t update(); // TAsk to update display
    uint8_t clear();
   // Anzeige Methoden
    void ShowError();
    void ShowWelcome();
    void ShowNormalMode(); 
    void ShowNetwork();

    void ShowLearnMode();
    void Set_Current_Screen(uint8_t screen_id);
    uint8_t get_Current_Screen() ;
    void set_error_code(uint8_t error_code) ;

//Types 
    enum ScreenId {
    SCREEN_WELCOME,
    SCREEN_MAIN,
    SCREEN_NETWORK,
    SCREEN_ERROR,
    SCREEN_LEARN,
    // Add more screens as needed
    SCREEN_INVALID,
    NUM_SCREENS
};
      
private:


struct ScreenDescriptor {
    ScreenId id;
    bool blink;              // soll dieser Screen blinken?
};

    U8G2_SH1107_SEEED_128X128_1_HW_I2C u8g2;
    bool initialized_display;
    bool update_screen;
   
    bool BLink_STATE;
    uint32_t last_blink_time;

    ScreenId current_screen;

    const ScreenDescriptor screens[NUM_SCREENS] =  {
    {SCREEN_WELCOME, false},
    {SCREEN_MAIN, false},
    {SCREEN_ERROR, true},     // nur der Fehler-Screen blinkt
    {SCREEN_NETWORK, false },
    {SCREEN_LEARN, true }
    };

   uint8_t Display_error_code;

/// @brief Toggle the blink state

void toggleBlinkState(); 
}; 
