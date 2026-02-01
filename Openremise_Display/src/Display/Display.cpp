#include "Display.h"


// Konstruktor
OpenRemiseDisplay::OpenRemiseDisplay()
: u8g2(U8G2_R0, U8X8_PIN_NONE)  // Page-Mode SEEED 128x128
{

}
// Initialisierung des Displays

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
  Set_Current_Screen(SCREEN_WELCOME);
  initialized_display = true;
  Serial.println(F("OpenRemiseDisplay: INIT OK"));


    return 0x00; // alles OK
} 

// Update Display Inhalt Task 

uint8_t OpenRemiseDisplay::update() {
    // Code to update the display content
    // Update display content based on current state or input
    // check for changes and refresh display if necessary
    // Example: Redraw UI elements, refresh screen, etc.
    // Update display content based on current state 
    // blink takt generieren 
 toggleBlinkState(); // manage blink state
// update screen if needed triggered by state change or blink state or button press
 if (update_screen) {
    switch (current_screen) {
        case SCREEN_WELCOME:
            ShowWelcome();
            break;
        case SCREEN_MAIN:
            ShowNormalMode();
            break;
        case SCREEN_ERROR:
            ShowError();
            break;
        default:
            break;
    }
    update_screen = false; // Reset update flag after updating
}













return (0) ;

} 

uint8_t OpenRemiseDisplay::clear() {
    // Code to clear the display
    // Clear the display content
    // Example: Fill screen with blank color, reset buffers, etc.
    // Clear display content
    return (0) ;
}   

// Anzeige Methoden
 
void OpenRemiseDisplay::ShowWelcome() {
    // Code to display a welcome message
    // Show welcome message on the display
    // Example: Display "Welcome" text, show logo, etc.
    // Display welcome message on the screen
    //const char* line1 = "Open Remise";
    const char* line2 = "Welcome!";
    const char* line3 = "Display Rev 0.2";
    const char* line4 = "Wait for data...";
    u8g2.firstPage();
    do {
        
                // frame im takt blinken lassen 
        if (BLink_STATE && screens[current_screen].blink) {
            u8g2.setDrawColor(2); // XOR mode for blinking
        } else {                                
            u8g2.setDrawColor(1); // Normal mode
        }
        // Frame zeichnen
        u8g2.drawFrame(0,0,128,128);
         u8g2.setDrawColor(1); // Normal mode for rest
        u8g2.drawXBMP (10, 20, 103, 21, openeremise_logo_103_21); // Display logo at (5,115)
        u8g2.setFont(u8g2_font_helvR08_tf);

        // Horizontales zentrieren
        //int16_t x1 = (128 - u8g2.getStrWidth(line1)) / 2;
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
    const char* line1 = "Track ";
    const char* line2 = "Voltage ";
    const char* line3 = "Current";
    const char* line4 = "Status";

    u8g2.firstPage();
    do {
                // frame im takt blinken lassen 
        if (BLink_STATE && screens[current_screen].blink) {
            u8g2.setDrawColor(2); // XOR mode for blinking
        } else {                                
            u8g2.setDrawColor(1); // Normal mode
        }
        // Frame zeichnen
        u8g2.drawFrame(0,0,128,128);
            u8g2.setDrawColor(1); // rest text nur normal zeichnen
        u8g2.drawXBMP (3, 5, 48, 48, openeremise_logo_48_48); // Display logo at (5,115)
        u8g2.setFont(u8g2_font_helvR08_tf);
        //Static Text 
        // Horizontales zentrieren
        int16_t x1 = (8); // Fixed position for title
        int16_t x2 = (50); // Fixed position for labels

        // Y-Koordinaten
        u8g2.drawStr(75, 30, line1);
        u8g2.drawStr(x1, 78, line2);
        u8g2.drawStr(x1, 93, line3);
        u8g2.drawStr(x1, 108, line4);
        //Data Platzhalter
        u8g2.drawStr(x2, 78, "12,7 V ");
        u8g2.drawStr(x2, 93, "0,5 A");
        u8g2.drawStr(x2, 108, "ON");


    } while (u8g2.nextPage());
}   
   
void OpenRemiseDisplay::ShowError() {
    // Code to display an error message
    // Show error message on the display
    // Example: Display "Error" text, show error icon, etc.
    // Display error message on the screen
    const char* line1 = "Error";
    const char* line2 = "Code ";    
    const char* line3 = "What   happened?";
    const char* line4 = "   "; // pointer on Strings to be replaced with actual error descriptions 
    u8g2.firstPage();
    do {
        // frame im takt blinken lassen 
        if (BLink_STATE && screens[current_screen].blink) {
            u8g2.setDrawColor(2); // XOR mode for blinking
        } else {                                
            u8g2.setDrawColor(1); // Normal mode
        }
        // Frame zeichnen        
        u8g2.drawFrame(0,0,128,128);
        u8g2.setDrawColor(1); // Normal mode for rest

        u8g2.drawXBMP (3, 5, 48, 48, openeremise_logo_48_48); // Display logo at (5,115)
      
        u8g2.setFont(u8g2_font_helvR08_tf);
        //Static Text 
        // Horizontales zentrieren
        int16_t x1 = (8); // Fixed position for title
        int16_t x2 = (50); // Fixed position for labels

        // Y-Koordinaten
        u8g2.drawStr(75, 30, line1);
        u8g2.drawStr(x1, 78, line2);
        u8g2.drawStr(x1, 93, line3);
        u8g2.drawStr(x1, 108, line4);
        //Data Platzhalter
        u8g2.drawStr(x2, 78,  " 32");
        u8g2.drawStr(x2, 93,  "Data lost");
        u8g2.drawStr(x2, 108, "Kom error");
    } while (u8g2.nextPage());


}  

void OpenRemiseDisplay::ShowNetwork() {
    // Code to display network information
    // Show network details on the display
    // Example: Display "Network" text, show IP address, etc.
    // Display network information on the screen
    const char* line1 = "Network";
    const char* line2 = "IP ";
    const char* line3 = "Status";
    const char* line4 = "SSid";

    u8g2.firstPage();
    do {
        
        // frame im takt blinken lassen 
        if (BLink_STATE && screens[current_screen].blink) {
            u8g2.setDrawColor(2); // XOR mode for blinking
        } else {                                
            u8g2.setDrawColor(1); // Normal mode
        }
        // Frame zeichnen        
        u8g2.drawFrame(0,0,128,128);
        
        u8g2.setDrawColor(1); // Normal mode for rest
        
        u8g2.drawXBMP (3, 5, 48, 48, openeremise_logo_48_48); // Display logo at (5,115)
        u8g2.setFont(u8g2_font_helvR08_tf);
        //Static Text 
        // Horizontales zentrieren
        int16_t x1 = (8); // Fixed position for title
        int16_t x2 = (50); // Fixed position for labels

        // Y-Koordinaten
        u8g2.drawStr(75, 30, line1);
        u8g2.drawStr(x1, 78, line2);
        u8g2.drawStr(x1, 93, line3);
        u8g2.drawStr(x1, 108, line4);
        //Data Platzhalter
        u8g2.drawStr(x2, 78, "192.168.1.100");
        u8g2.drawStr(x2, 93, "Connected");
        u8g2.drawStr(x2, 108, "MyWiFiSSID");


    } while (u8g2.nextPage());
}   


//Hilfsfunktionen 

void OpenRemiseDisplay::Set_Current_Screen(uint8_t screen_id) { 
    if (screen_id < NUM_SCREENS) {
        current_screen = static_cast<ScreenId>(screen_id);
        update_screen = true; // Mark screen for update
    }
}

void OpenRemiseDisplay::set_error_code(uint8_t error_code) {
    // Code to set the error code and update the display accordingly
    // Update display based on the provided error code
    // Example: Show specific error messages or indicators
    // Update display content based on error code
    // This is a placeholder implementation; actual implementation may vary
    Display_error_code = error_code;
}



void OpenRemiseDisplay::toggleBlinkState() {
    // Code to toggle the blink state
    // Toggle the blink state variable
    if (last_blink_time == 0 || (millis() - last_blink_time) >= 500) {
        last_blink_time = millis();
        
        BLink_STATE = !BLink_STATE;
        if (screens[current_screen].blink) {
                    update_screen = true; // Mark screen for update
     }   

    
 }
}
// End of Display.cpp
