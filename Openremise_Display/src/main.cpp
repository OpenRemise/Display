
//Initialisierung 
#include <Arduino.h>
#include <EEPROM.h>

#include <stdint.h>
#include "Buttons.h"
#include "Display.h"
#include "protokoll.h"
//Global Variables
#include "main.h"


// Main setup and loop functions
Buttons Button;
OpenRemiseDisplay Display;
OpenRemiseProtokoll Protokoll; // Protokoll instance  

void setup() {

SYS_Init();

   // Initializecomponents here
   SYS.State = Button.begin(BUTTON_A_PIN, LED_A_PIN, BUTTON_B_PIN, LED_B_PIN);
   SYS.State = Display.begin();
   SYS.State = Protokoll.begin(); // also init from  EEprom content 


// Check for initialization errors

if (SYS.State == ERROR_STATE) {
#ifdef SERIAL_DEBUG
Serial.println("Error during Initialization!");
#endif
Display.ShowError();
}else {
SYS.State = RUN;
Display.ShowWelcome();
}
#ifdef SERIAL_DEBUG
    Serial.println("Initialization Complete.");
#endif
}


void loop() {
    // put your main code here, to run repeatedly:
    //Buttons_Task();
    //Display_Task();
    //Protokoll_Task();
    //EEprom_Task();

    //timing and state management
    if (SYS.time_interval >= 1.0) { // 1 second interval
        SYS.time_interval = 0.0;
        SYS.time_stamp += 1.0;
        // Perform time-based tasks here
    }   

if (SYS.Learnsatus == LEARN_MODE) {
    // Perform actions related to learn mode
   // Display new learnd Values 
    Display.ShowLearnMode();

  } else if (SYS.State == ERROR_STATE) {
    // Handle error state
    Display.ShowError();// Display error message Invert display
  } else {
    // Normal operation code
    Display.ShowNormalMode(); // Display normal operation information 
  }


   

}        
// System Initialization function
void SYS_Init() {
    SYS.time_stamp = millis();
    SYS.time_interval = 0.0;
    SYS.Last_Run = SYS.time_stamp; //  milliseconds since startup
    SYS.Learnsatus = OFF; // Learn mode off
    // Initialize system state
    SYS.State = INIT;
#ifdef SERIAL_DEBUG
//debugging via Serial Monitor via usb port
Serial.begin(9600);
Serial.println("System Initializing...");
    // Additional setup code can be added here
#endif
}   

// End of Main.cpp
