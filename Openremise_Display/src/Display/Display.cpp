#include "Display.h"


// Konstruktor
OpenRemiseDisplay::OpenRemiseDisplay()
: u8g2(U8G2_R0, U8X8_PIN_NONE)  // Page-Mode SEEED 128x128
{

}

uint8_t OpenRemiseDisplay::begin() {
    // Initialization code for the display
    // Initialize display hardware or software components
    // Exabegimple: Initialize display driver, set up buffers, etc.
    // Initialize display driver see  EA code example 
    // Display starten
        Serial.println("I2C Scanner");
  byte error, address;
  for(address = 1; address < 127; address++ ) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0) {
      Serial.print("Device found at 0x");
      Serial.println(address, HEX);
    }
  } 
    if (!u8g2.begin()) {
        Serial.println(F("OpenRemiseDisplay: INIT ERROR!"));
        return 0xFF; // Fehler
    }

    Serial.println(F("OpenRemiseDisplay: INIT OK"));


    return 0x00; // alles OK
} 

uint8_t OpenRemiseDisplay::update() {
    // Code to update the display content
    // Update display content based on current state or input
    // check for changes and refresh display if necessary
    // Example: Redraw UI elements, refresh screen, etc.
    // Update display content based on current state 

return (0) ;

} 

uint8_t OpenRemiseDisplay::clear() {
    // Code to clear the display
    // Clear the display content
    // Example: Fill screen with blank color, reset buffers, etc.
    // Clear display content
    return (0) ;
}   

 
void OpenRemiseDisplay::ShowWelcome() {
    // Code to display a welcome message
    // Show welcome message on the display
    // Example: Display "Welcome" text, show logo, etc.
    // Display welcome message on the screen
    const char* line1 = "Open Remise";
    const char* line2 = "Welcome!";
    const char* line3 = "Display Rev 0.2";
    const char* line4 = "Wait for data...";
      


    u8g2.firstPage();
    do {
        u8g2.drawXBMP (10, 20, 103, 21, openeremise_logo_103_21); // Display logo at (5,115)
        u8g2.drawFrame(0,0,128,128);
        u8g2.setFont(u8g2_font_helvR08_tf);

        // Horizontales zentrieren
        int16_t x1 = (128 - u8g2.getStrWidth(line1)) / 2;
        int16_t x2 = (128 - u8g2.getStrWidth(line2)) / 2;
        int16_t x3 = (128 - u8g2.getStrWidth(line3)) / 2;
        int16_t x4 = (128 - u8g2.getStrWidth(line4)) / 2;

        // Y-Koordinaten
       // u8g2.drawStr(x1, 30, line1);
        u8g2.drawStr(x2, 60, line2);
        u8g2.drawStr(x4, 90, line4);
        u8g2.drawStr(x3, 120, line3);

    } while (u8g2.nextPage());

}   
void OpenRemiseDisplay::ShowLearnMode() {
    // Code to display learn mode information
    // Show learn mode details on the display
    // Example: Display "Learn Mode" text, show instructions, etc.
    // Display learn mode information on the screen

}
void OpenRemiseDisplay::ShowNormalMode() {
    // Code to display normal mode information
    // Show normal operation details on the display
    // Example: Display status info, show operational data, etc.
    // Display normal mode information on the screen

}   
void OpenRemiseDisplay::ShowError() {
    // Code to display an error message
    // Show error message on the display
    // Example: Display "Error" text, show error icon, etc.
    // Display error message on the screen

}  

void OpenRemiseDisplay::ShowNetwork() {
    // Code to display network information
    // Show network details on the display
    // Example: Display "Network" text, show IP address, etc.
    // Display network information on the screen
    const char* line1 = "Network";
    const char* line2 = "IP     :";
    const char* line3 = "Status :";
    const char* line4 = "SSid   :";

    u8g2.firstPage();
    do {
        u8g2.drawFrame(0,0,128,128);
        u8g2.setFont(u8g2_font_helvR08_tf);
        //Static Text 
        // Horizontales zentrieren
        int16_t x1 = (10); // Fixed position for title
        int16_t x2 = (60); // Fixed position for labels

        // Y-Koordinaten
        u8g2.drawStr(x1, 15, line1);
        u8g2.drawStr(x1, 30, line2);
        u8g2.drawStr(x1, 45, line3);
        u8g2.drawStr(x1, 60, line4);
        //Data Platzhalter
        u8g2.drawStr(x2, 30, "192.168.1.100");
        u8g2.drawStr(x2, 45, "Connected");
        u8g2.drawStr(x2, 60, "MyWiFiSSID");


    } while (u8g2.nextPage());
}   

// End of Display.cpp
