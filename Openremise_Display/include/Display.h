#pragma once

#include <Arduino.h>
#include <stdio.h>
#include <U8g2lib.h>// Include the U8g2 library for display handling
#include <Wire.h> // Include Wire library for I2C communication
#include "Logo.h" // Include logo bitmaps

// Define for the OpenRemiseDisplay 
// define  for I2C Scanner

//#define I2C_SCANNER
//SCREEN DEFINES
#define SCREEN_Title_x 75
#define SCREEN_Title_y 30
#define LOGO_x 6
#define LOGO_y 5
#define SCREEN_FIXED_X 8
#define SCREEN_FIXED_Y  70
#define SCREEN_LINE_DIS 15
#define SCREEN_DATA_X 50
#define BLINKTIME 750   // Blinkzeit in Millisekunden

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
    void ShowWelcome();
    void ShowNormalMode();
    void ShowNetwork(); 
    void ShowError();
    void ShowLearn();
    void ShowUni();
    void ShowDisplay();
    void ShowInvalid();
    void ShowLearnMode();

    //API 
    
    void Set_Current_Screen(uint8_t screen_id);
    void Set_Current_content(const char* line1,const char* line2,const char* line3,const char* line4,const char* line5);
    uint8_t get_Current_Screen() ;
    void update_screencontent();
    void set_error_code(uint8_t error_code, const char* error_str = nullptr);
    void set_info_string( const char* info);
    void force_update();

//Types 
    enum ScreenId {
    SCREEN_WELCOME,
    SCREEN_MAIN,
    SCREEN_NETWORK,
    SCREEN_ERROR,
    SCREEN_LEARN,
    SCREEN_UNI,    // Screen for direct write 
    // Add more screens as needed
    SCREEN_INVALID,
    NUM_SCREENS
};
      
private:
    ScreenId current_screen;
    U8G2_SH1107_SEEED_128X128_1_HW_I2C u8g2;
    bool initialized_display;
    bool update_screen;
    
    bool BLink_STATE;
    uint32_t last_blink_time;


 
struct ScreenDescriptor {
    ScreenId id;
    bool blink;              // soll dieser Screen blinken?
    const char* const title; 
    const char* const linie_1;
    const char* const linie_2;
    const char* const linie_3;
    const char* const linie_4;
    const char* const linie_5;
};
const ScreenDescriptor screens[NUM_SCREENS] =  {
    {SCREEN_INVALID, false," "," "," "," "," "}, // Default screen, not used but placeholder for index 0
    {SCREEN_WELCOME, false," "," "," "," "," "},
    {SCREEN_MAIN, false,"Track ","Voltage : ","Current :","Status :","Mode :"},
    {SCREEN_ERROR, true,"ERROR ","CODE :","What happend "," "," " },     //Fehler-Screen blinkt
    {SCREEN_NETWORK, false,"Network "," IP ","Status ","SSID ","RSSI " },
    {SCREEN_LEARN, true," "," "," "," "," "},// Lernmodus blinkt
    {SCREEN_UNI,false," "," "," "," "," " } // universal screen
    };
struct Display_content_t {
         
         const char* Display_info_1 ;  
         const char* Display_info_2 ;
         const char* Display_info_3 ;  
         const char* Display_info_4 ; 
         const char* Display_info_5  ;    
    } Content;

uint8_t Display_error_code;
const char* Display_error_string;
const char* Display_info_string = nullptr;

/// @brief Toggle the blink state
// Hilfsfunktionen 

void toggleBlinkState(); 
void  blinking_frame();
int8_t findScreenDescrpitorIndex ();

}; 
