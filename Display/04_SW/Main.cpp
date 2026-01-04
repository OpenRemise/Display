Main.cpp

//Initialisierung 
#include <Arduino.h>
#include <EEPROM.h>

#include <stdint.h>
#include "Buttons/Buttons.h"
#include "Display/Display.h"
#include "Protokoll/protokoll.h"
//Global Variables
#include "main.h"


// Main setup and loop functions

void setup() {

 SYS_Init();

   // Initializecomponents here
   SYS_.State = Buttons_Init(SYS_.State);
   SYS_.State = Display_Init(SYS_.State);
   SYS_.State = Protokoll_Init(SYS_.State);
   SYS_.State = EEprom_Init(SYS_.State);
// Check for initialization errors
if (SYS_.State == ERROR_STATE) {
#ifdef SERIAL_DEBUG
Serial.println("Error during Initialization!");
#endif
Display_ShowError();
}else {
SYS_.State = RUN;
Display_ShowWelcome();
}
#ifdef SERIAL_DEBUG
    Serial.println("Initialization Complete.");
#endif
}
}

void loop() {
    // put your main code here, to run repeatedly:
    //Buttons_Task();
    //Display_Task();
    //Protokoll_Task();
    //EEprom_Task();

    //timing and state management
    if (SYS_.time_interval >= 1.0) { // 1 second interval
        SYS_.time_interval = 0.0;
        SYS_.time_stamp += 1.0;
        // Perform time-based tasks here
    }   

if (SYS_.LearnStatus == LEARN_MODE) {
    // Perform actions related to learn mode
   // Display new learnd Values 
    Display_ShowLearnMode();
    
  } elseif (SYS_.State == ERROR_STATE) {
    // Handle error state
    Display_ShowError();// Display error message Invert display
  } else {
    // Normal operation code
    Display_ShowNormalMode(); // Display normal operation information 
  }


   

}        
// System Initialization function
void SYS_Init() {
    SYS_.time_stamp = millis();
    SYS_.time_interval = 0.0;
    SYS_.Last_Run = SYS_.time_stamp; //  milliseconds since startup
    SYS_.LearnStatus = OFF; // Learn mode off
    // Initialize system state
    SYS_.State = INIT;
#ifdef SERIAL_DEBUG
//debugging via Serial Monitor via usb port
Serial.begin(9600);
Serial.println("System Initializing...");
    // Additional setup code can be added here
#endif
}   

// End of Main.cpp
