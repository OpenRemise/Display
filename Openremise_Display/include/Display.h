#pragma once

#include <Arduino.h>
#include <stdio.h>
#include <U8g2lib.h>// Include the U8g2 library for display handling
#include <Wire.h> // Include Wire library for I2C communication
#include "Logo.h" // Include logo bitmaps

// Define for the OpenRemiseDisplay 
// define  for I2C Scanner

//#define I2C_SCANNER
//SCREEN position DEFINES
#define SCREEN_Title_x 75
#define SCREEN_Title_y 30
#define LOGO_x 6
#define LOGO_y 5
#define SCREEN_FIXED_X 8
#define SCREEN_FIXED_Y 70
#define SCREEN_LINE_DIS 15
#define SCREEN_DATA_X 50
#define SCREEN_LINE_1Y SCREEN_FIXED_Y
#define SCREEN_LINE_2Y SCREEN_LINE_1Y +SCREEN_LINE_DIS
#define SCREEN_LINE_3Y SCREEN_LINE_2Y +SCREEN_LINE_DIS
#define SCREEN_LINE_4Y SCREEN_LINE_3Y +SCREEN_LINE_DIS
#define SCREEN_LINE_5Y SCREEN_LINE_4Y +SCREEN_LINE_DIS

#define BLINKTIME 750   // Blinkzeit in Millisekunden
//Types 
    enum ScreenId {
    SCREEN_WELCOME,
    SCREEN_NETWORK,
    SCREEN_MAIN,
    SCREEN_LEARN,
    SCREEN_UNI,    // Screen for direct write 
    NUM_MENUE_SCREENS, // Number of screens in the menu, used for cycling through screens
    // Add more screens as needed 
    SCREEN_ERROR,// should be the last screen before invalid because it is dominant and can only be left by reset or error resolution
    SCREEN_INVALID,// Invalid screen, can be used as default or error state on display issues 
    NUM_SCREENS
};

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
    void ShowWelcome(); // Show welcome screen
    void ShowNormalMode(); // Show dinamic content for normal operation
    void ShowNetwork(); //obsolete remove after cleanup
    void ShowError(); // Show error screen with error code and details
    void ShowUni(); // Show universal screen with free text, can be used for debug or special messages TODO
    void ShowDisplay();// obsolet remove after cleanup
    void ShowInvalid(); // Show invalid screen if screen id is not valid or other display issues
    void ShowLearnMode();   // Show learn mode screen, not implemented yet but could be used for special functions or settings in the future

    //API 
    
    void Set_Current_Screen(uint8_t screen_id);
    void Set_Current_content(const char* l1, const char* l2, const char* l3,const char* l4,const char* l5);
    ScreenId Get_Current_Screen() ;
    void update_screencontent();
    void set_error_code(uint8_t error_code, const char* error_str = nullptr);
    void set_info_string( const char* info);
    void force_update();


      
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
    {SCREEN_INVALID, false,"INVALID! "," "," "," "," "}, // Default screen, not used but placeholder for index 0
    {SCREEN_WELCOME, false," "," "," "," "," "},
    {SCREEN_MAIN, false,"Track ","Voltage : ","Current :","Status :","Mode :"},
    {SCREEN_ERROR, true,"ERROR ","CODE :","What happend "," "," " },     //Fehler-Screen blinkt
    {SCREEN_NETWORK, false,"Network "," IP ","Status ","SSID ","RSSI " },
    {SCREEN_LEARN, true,"LEARN "," "," "," "," "},// Lernmodus blinkt
    {SCREEN_UNI,false," UNI ","> ","> ","> ","> " } // universal screen
    };

const char* content[5]; // 5 lines  but only the pointer will be kept.



uint8_t Display_error_code;
const char* Display_error_string;
const char* Display_info_string = nullptr;

/// @brief Toggle the blink state
// Hilfsfunktionen 

void toggleBlinkState(); 
void  blinking_frame();
int8_t findScreenDescrpitorIndex ();

}; 
