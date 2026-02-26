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
    #ifdef I2C_SCANNER
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
    #endif
    if (!u8g2.begin()) {
        Serial.println(F("OpenRemiseDisplay: INIT ERROR!"));
        return 0xFF; // Fehler
    }
   u8g2.setContrast(150); // Set contrast to 128 (half of max value)
  Set_Current_Screen(SCREEN_INVALID);// set to last screen to force update
  initialized_display = true;
  BLink_STATE = true;
  last_blink_time = 0;

 #ifdef SERIAL_DEBUG
  Serial1.println(F("OpenRemiseDisplay: INIT OK"));
#endif  
// Strings default values 
Display_error_code =0;
Display_info_string = " no info" ;
Display_error_string = "No information";
Set_Current_content("--", "--", "--", "--", "--");
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
    //Only Update screen on change or if blink state changed for blink screens
toggleBlinkState(); // manage blink state
// update screen if needed triggered by state change or blink state or button press
 if (update_screen) {
    switch (current_screen) {
        case SCREEN_WELCOME:
            ShowWelcome();
            break;
        case SCREEN_MAIN:
        case SCREEN_NETWORK:
        case SCREEN_UNI:
            ShowNormalMode();
            break;
        case SCREEN_ERROR:
            ShowError();
            break;
        case SCREEN_LEARN:
            ShowLearnMode();    
            break;
        case SCREEN_INVALID:
            ShowInvalid();
            break;
        default:
            break;
    }
    update_screen = false; // Reset update flag after updating
    digitalWrite(LED_BUILTIN, LOW); // Turn off built-in LED after updating display
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
void OpenRemiseDisplay::Set_Current_content ( const char* l1, const char* l2, const char* l3,const char* l4,const char* l5)
{
    content[0] = l1;
    content[1] = l2;
    content[2] = l3;
    content[3] = l4;
    content[4] = l5;
} 

// Anzeige Methoden
// Display methode  for all not generic  Screens 
void OpenRemiseDisplay::ShowNormalMode() {
    // Code to display normal mode information
    // Show normal operation details on the display
    // Example: Display status info, show operational data, etc.
    /*Display normal mode information on the screen 
*/
    int8_t myidx =findScreenDescrpitorIndex();
    
    u8g2.firstPage();
    do {
             u8g2.setDrawColor(1); // Normal mode for rest 
                        // frame im takt blinken lassen 
         blinking_frame(); 
   
        u8g2.setFont(u8g2_font_helvR08_tf);     
       
        //Statics from Screndescriptor 
        u8g2.drawStr(SCREEN_Title_x - (u8g2.getStrWidth(screens[myidx].title))/2 , SCREEN_Title_y, screens[myidx].title);
        u8g2.drawStr(SCREEN_FIXED_X, SCREEN_LINE_1Y, screens[myidx].linie_1);
        u8g2.drawStr(SCREEN_FIXED_X, SCREEN_LINE_2Y, screens[myidx].linie_2);
        u8g2.drawStr(SCREEN_FIXED_X, SCREEN_LINE_3Y,screens[myidx].linie_3);
        u8g2.drawStr(SCREEN_FIXED_X, SCREEN_LINE_4Y,screens[myidx].linie_4);
        //Data dynamik data 
        u8g2.drawStr(SCREEN_DATA_X, SCREEN_LINE_1Y,content[0]); 
        u8g2.drawStr(SCREEN_DATA_X, SCREEN_LINE_2Y,content[1]); 
        u8g2.drawStr(SCREEN_DATA_X, SCREEN_LINE_3Y,content[2]);
        u8g2.drawStr(SCREEN_DATA_X, SCREEN_LINE_4Y,content[3]);
       
    } while (u8g2.nextPage());
}


// Display methode  for direkt write 
  
// Generic Screens = Welcome, Error, Invalid, Learnmode
void OpenRemiseDisplay::ShowWelcome() {
    // Code to display a welcome message
    // Show welcome message on the display
    // Example: Display "Welcome" text, show logo, etc.
    // Display welcome message on the screen
 
    const char* line1=  "Welcome!";
    const char* line2 = "Open Remise Rev 0.6" ;
    const char* line3 = "Display Rev 0.2";
    const char* line4 = "Wait for data...";
    u8g2.firstPage();
    do {

        // Frame zeichnen
        u8g2.drawFrame(0,0,128,128);
        u8g2.setDrawColor(1); // Normal mode for rest
        u8g2.drawXBMP ((LOGO_x+5), (LOGO_y+5), 103, 21, openeremise_logo_103_21); // Display logo at (5,115)
        u8g2.setFont(u8g2_font_helvR08_tf);

        // Horizontales zentrieren
        int16_t x1 = (128 - u8g2.getStrWidth(line1)) / 2;
        int16_t x2 = (128 - u8g2.getStrWidth(line2)) / 2;
        int16_t x3 = (128 - u8g2.getStrWidth(line3)) / 2;
        int16_t x4 = (128 - u8g2.getStrWidth(line4)) / 2;

        // Y-Koordinaten
    
        u8g2.drawStr(x1, 60, line1);
        u8g2.drawStr(x2, 97, line2);
        u8g2.drawStr(x3, 110, line3 );
        u8g2.drawStr(x4, 120, line4);

    } while (u8g2.nextPage());

} 

void OpenRemiseDisplay::ShowError() {
    // Code to display an error message
    // Show error message on the display
    // Example: Display "Error" text, show error icon, etc.
    // Display error message on the screen
    u8g2.firstPage();
    do { 
        blinking_frame();    
      
        u8g2.setFont(u8g2_font_helvR08_tf);
        //Static Text 
        // Horizontales zentrieren
        int16_t x1 = (8); // Fixed position for title
        //int16_t x2 = (50); // Fixed position for labels
        
        u8g2.drawStr(SCREEN_Title_x, SCREEN_Title_y, "Error!"); // Title for error screen
 
            // error details anzeigen
       u8g2.setCursor(84, 45);
       u8g2.print(Display_error_code);   // Vom System erzeugt error code anzeigen, z.B. 32 = Communication error, 64 = Sensor failure, etc.  
       u8g2.drawStr(x1, 80,"What happened?"); // Label for error code         
       u8g2.drawStr(x1, 93, Display_error_string); // Display error string if available

        
        if (Display_info_string != nullptr)
        {
        u8g2.drawStr(x1, 108, Display_info_string);
        }
        //Data Platzhalter
   

    } while (u8g2.nextPage());


}  

void OpenRemiseDisplay::ShowInvalid ()
{ //set Error on main dont show screen
  do{
        u8g2.setFont(u8g2_font_helvR08_tf);
        //Static Text 
        // Horizontales zentrieren
        int16_t x1 = (8); // Fixed position for title
        //int16_t x2 = (50); // Fixed position for labels
        
        u8g2.drawStr(SCREEN_Title_x, SCREEN_Title_y, "Invalid Screen!"); // Title for error screen
 
            // error details anzeigen
       u8g2.setCursor(84, 45);
       u8g2.print(current_screen);   // Vom System erzeugt error code anzeigen, z.B. 32 = Communication error, 64 = Sensor failure, etc.  
       u8g2.drawStr(x1, 80,"Screen ID not valid!"); // Label for error code         
       u8g2.drawStr(x1, 93, "Please check your code!"); // Display error string if available

        
    } while (u8g2.nextPage());
}

void OpenRemiseDisplay::ShowLearnMode() {
    // Code to display learn mode information
    // Show learn mode details on the display
    // Example: Display "Learn Mode" text, show instructions, etc.
    // Display learn mode information on the screen

    const char* line1=  "Learn Mode";
    const char* line2 = "Please follow instructions" ;
    const char* line3 = "not implemented yet";
    u8g2.firstPage();
    do {
        // Frame zeichnen
        u8g2.drawFrame(0,0,128,128);
        u8g2.setDrawColor(1); // Normal mode for rest
        u8g2.setFont(u8g2_font_helvR08_tf);

        // Horizontales zentrieren
        int16_t x1 = (128 - u8g2.getStrWidth(line1)) / 2;
        int16_t x2 = (128 - u8g2.getStrWidth(line2)) / 2;

        // Y-Koordinaten
    
        u8g2.drawStr(x1, 60, line1);
        u8g2.drawStr(x2, 97, line2);  
        u8g2.drawStr(x2, 110, line3 );  

    } while (u8g2.nextPage());
}

/******************************************************************************************* */
/*Hilfsfunktionen ***************************************************************************/
/******************************************************************************************* */

void OpenRemiseDisplay::Set_Current_Screen(uint8_t screen_id) { 
   
    if (screen_id < NUM_SCREENS) {
       if (false == screens[current_screen].blink)
       {    
       if (static_cast<ScreenId>(screen_id) == current_screen) {
            return; // No change needed
             }  
        } 
        current_screen = static_cast<ScreenId>(screen_id);
        // update_screen = true; // Mark screen for update
    }
}

void OpenRemiseDisplay::force_update() {
    update_screen = true; // Mark screen for update
}   

ScreenId OpenRemiseDisplay::Get_Current_Screen()  {
    return (current_screen);
} 

void OpenRemiseDisplay::set_error_code(uint8_t error_code, const char* error_str) {
    // Code to set the error code and update the display accordingly
    // Update display based on the provided error code
    // Example: Show specific error messages or indicators
    // Update display content based on error code
    // This is a placeholder implementation; actual implementation may vary
    // If error_str is provided, store it for display purposes
    Display_error_code = error_code;
    if (error_str != nullptr) {
        // Store error_str in a member variable or buffer for later display
        // Example: store in a static buffer or member variable
        // For now, we just store the error code
        Display_error_string = error_str;
    }
}


void OpenRemiseDisplay::set_info_string( const char* info){
   if (info != nullptr)
   { Display_info_string = info;}

}

void OpenRemiseDisplay::toggleBlinkState() {
    // Code to toggle the blink state
    // Toggle the blink state variable
    if (last_blink_time == 0 || (millis() - last_blink_time) >= BLINKTIME) {
        last_blink_time = millis();
        BLink_STATE = !BLink_STATE;
            if (screens[current_screen].blink) {
                    update_screen = true; 
        
            }           
        }  
     } 

     void OpenRemiseDisplay::blinking_frame()
     {
        
         if(BLink_STATE && screens[current_screen].blink) {
                   //FRAME zeichnen
        //  u8g2.setDrawColor(1); // Normal mode
             // LOGO zeichnen 
           u8g2.drawXBMP (LOGO_x, LOGO_y, 32, 32, openeremise_logo); // Display small logo at (5,115)
             for(int i=0; i<5; i+=2) {
            u8g2.drawFrame(i, i, 128 - 2*i, 128 - 2*i); // Draw the inner frame
            }
        } 
        else {  
                     //FRAME zeichnen
      //      u8g2.setDrawColor(1); // Normal mode
         u8g2.drawXBMP (LOGO_x, LOGO_y, 32, 32, openeremise_logo); 
         
          u8g2.drawFrame(0,0,127,127); // Draw the outer frame only when not blinking
          digitalWrite(LED_BUILTIN, HIGH); 
          
        }

     }

     int8_t OpenRemiseDisplay::findScreenDescrpitorIndex()
     {
        for (uint8_t i =0u ;i < NUM_SCREENS; i++)
        if (screens[i].id == current_screen)
           { return (int8_t) i;
           }

           return (int8_t) -1;
     }


// End of Display.cpp
