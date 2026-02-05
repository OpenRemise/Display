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
unsigned long waittime;
  SYS.State = MODE_INIT;
  SYS.Error_Code = ERROR_NONE; // Set error code to none initially
Serial.begin(115200);
#ifdef SERIAL_DEBUG
Debug_port.begin(9600);

  Debug_port.println(F("OpenRemise Display Debugport activ"));
#endif

  // Buttons initialisieren
        buttons.begin(BUTTON_A_PIN, LED_A_PIN, BUTTON_B_PIN, LED_B_PIN); // Button and LED pins
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
       //  not blink with button LEDS 
      buttons.SetLED(BUTTON_A, true);
      buttons.SetLED(BUTTON_B, true);
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
// Protkoll initialisieren
protokoll.begin();
//Wait  for S3 sending json data on HW Uart mit 115200 Baud

#ifdef SERIAL_DEBUG
OpenRemiseProtokoll proto(Debug_port);
#else
OpenRemiseProtokoll proto(Serial);
#endif
// wait for data for 5 sec 
 waittime = millis();
   display.update();
while(millis() < waittime + 5000); // Wait for 5 seconds
display.Set_Current_Screen(display.SCREEN_MAIN) ;
//Initend
buttons.SetLED(BUTTON_B, false);
buttons.SetLED(BUTTON_A, false);
// Set initial screen to normal mode

}

void loop() {
  uint8_t ret_value ;
  // Main loop


  unsigned long wait_time = millis();

  //get  the input from enviroment
  // Update button states

  // update Protokoll module
  //protokoll.update(&jason_buffer);
  ret_value = buttons.update();
  // figure out  the system reaktions

    // check events for display update
  // Check for button A press to show main screen
  if (buttons.read() & mask_Button_A) {
      display.Set_Current_Screen(display.SCREEN_MAIN) ;
      SYS.State = MODE_NORMAL;
  }
  // Check for button B press to show network info
  if (buttons.read() & mask_Button_B) {
      display.Set_Current_Screen(display.SCREEN_NETWORK) ;
      SYS.State = MODE_NETWORK;
  }
 if(buttons.read() >= RESET_REQUEST) {
    // both buttons held for reset request
    SYS.State = MODE_ERROR;
    SYS.Error_Code = ERROR_BUTTONS; // Clear error code on reset
    // Additional reset logic as needed
 }

// Dominant Event is error state  

 if (SYS.State == MODE_ERROR) {
  display.Set_Current_Screen(display.SCREEN_ERROR) ;
  display.set_error_code(SYS.Error_Code);
  
 }
  // Reakt on system state changes

  // Update the display content
  display.update();

while(millis() < wait_time + 10);// wait for 1 sec

   }


