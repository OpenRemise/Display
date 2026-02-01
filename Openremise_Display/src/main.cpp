#include "main.h"

/* SH1107 128x128, I2C, Hardware I2C, Page-Mode */
OpenRemiseDisplay display;
// buttons for up and down 
Buttons buttons;
// Protokoll instance
OpenRemiseProtokoll protokoll(Serial); // Protokoll  kommunikation über HW Uart


// software serial for debug output
#ifdef SERIAL_DEBUG
SoftwareSerial Debug_port (PROTOKOLL_RX_PIN, PROTOKOLL_TX_PIN);
#endif

/// @brief Setup function for initialization
void setup() {
// unsigned long waittime;

#ifdef SERIAL_DEBUG
Debug_port.begin(9600);

  Debug_port.println(F("OpenRemise Display System Init"));
#endif
Serial.begin(115200);
protokoll.begin();


  SYS.State = MODE_INIT;
  SYS.Error_Code = ERROR_NONE; // Set error code to none initially


  // Buttons initialisieren
        buttons.begin(PIND4, PIND5, PIND6 ,PIND7); // Button and LED pins
          buttons.SetLED(BUTTON_A, true);
          buttons.SetLED(BUTTON_B, true);
  // Display initialisieren
#ifdef SERIAL_DEBUG
  Debug_port.println(F("SH1107 Init Test"));
#endif
    uint8_t ret = display.begin();
    if (ret != 0x00) {

#ifdef SERIAL_DEBUG
        Debug_port.println(F("Display init failed!"));
#endif
        SYS.State = MODE_ERROR;
        SYS.Error_Code = ERROR_DISPLAY;
         }    
     if (ret == 0x00) { 
      // if  display is ok  
        // Show welcome screen
        display.Set_Current_Screen(display.SCREEN_WELCOME);
        display.update();
     //  not blink with button LEDS 
      buttons.SetLED(BUTTON_A, false);
      buttons.SetLED(BUTTON_B, false);
      }
      else {
       // Blink both LEDs to indicate error
        while (1) {
            buttons.SetLED(BUTTON_A, true);
            buttons.SetLED(BUTTON_B, false);
            delay(500);
            buttons.SetLED(BUTTON_A, false);
            buttons.SetLED(BUTTON_B, true);
            delay(500);
        }
     }
buttons.SetLED(BUTTON_B, true);
// Protkoll initialisieren
protokoll.begin();
//Wait  for S3 sending json data on HW Uart mit 115200 Baud

#ifdef SERIAL_DEBUG
OpenRemiseProtokoll proto(Debug_port);
#else
OpenRemiseProtokoll proto(Serial);
#endif


// wait for data for 5 sec 

 // waittime = millis();
 
/*if no dat send with in the first 5 sec show error
if ((millis() - waittime) > 5000) {
  // no data received within 5 seconds
  SYS.State = MODE_ERROR;
  SYS.Error_Code = ERROR_PROTOKOLL; // Set error code to none initially

}*/




//Initend
buttons.SetLED(BUTTON_B, false);
buttons.SetLED(BUTTON_A, false);

// Set initial screen to normal mode

}

void loop() {
  //get  the input from enviroment
  // Update button states
  buttons.update();
  // update Protokoll module
  //protokoll.update(&jason_buffer);

  // figure out  the system reaktions
  
  // check events for display update
  // Check for button A press to show main screen
  if (buttons.read() & BUTTON_A) {
      display.Set_Current_Screen(display.SCREEN_MAIN) ;
  }
  // Check for button B press to show network info
  if (buttons.read() & BUTTON_B) {
      display.Set_Current_Screen(display.SCREEN_NETWORK) ;
  }

// Dominant Event is error state  

 if (SYS.State == MODE_ERROR) {
  display.Set_Current_Screen(display.SCREEN_ERROR) ;
  display.set_error_code(SYS.Error_Code);
 }
  // Reakt on system state changes

  // Update the display content
  display.update();


   }


