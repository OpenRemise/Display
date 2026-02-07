#include "main.h"

/* SH1107 128x128, I2C, Hardware I2C, Page-Mode */
OpenRemiseDisplay display;
// buttons for up and down 
Buttons buttons;
// Protokoll instance
OpenRemiseProtokoll protokoll; // Protokoll  kommunikation über HW Uart


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
protokoll.begin(Serial);
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
// Protkoll initialisieren

//Wait  for S3 sending json data on HW Uart mit 115200 Baud

// wait for data for 5 sec 
 waittime = millis();
   display.update();
while(millis() < waittime + 5000); // Wait for 5 seconds
display.Set_Current_Screen(display.SCREEN_MAIN) ;
//Initend
buttons.SetLED(BUTTON_B, true);
buttons.SetLED(BUTTON_A, true);
// Set initial screen to normal mode

}

void loop() {
 
  bool sleep_triggered = false; // Flag to track if sleep mode has been triggered
  // Main loop


  unsigned long wait_time = millis();
 
  //get  the input from enviroment
  // update Protokoll module
  //protokoll.update(&jason_buffer);
  buttons.update(); // return also button state !!!
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
    SYS.Error_Code = ERROR_PROTOKOLL; // Clear error code on reset
    // Additional reset logic as needed
    SYS.Last_Run = millis(); // Update last run time to prevent immediate re-triggering of sleep mode
    }
 
 if( buttons.read() != NO_BUTTONS)
    {
      SYS.Last_Run = millis() ; // Update last run time 
      sleep_triggered = false; // Reset sleep mode flag on activity
    }
if( millis() - SYS.Last_Run >= SLEEP_TIMEOUT) { // Check for inactivity (10 seconds)
    sleep_triggered = true; // Set sleep mode flag
    SYS.State = MODE_SLEEP; // Update system state to sleep
    // Additional sleep mode logic as needed
    }

    // set led status based on Screens and sleep mode
if (sleep_triggered == false) {
    switch (display.get_Current_Screen() ) {
    case display.SCREEN_MAIN:
        // LED for main screen
      buttons.SetLED(BUTTON_A, LED_OFF);
      buttons.SetLED(BUTTON_B, LED_ON);
      
        break;
    case display.SCREEN_NETWORK:
        // LED for network screen
      buttons.SetLED(BUTTON_A, LED_ON);
      buttons.SetLED(BUTTON_B, LED_OFF);
        break;
    case display.SCREEN_ERROR:
        // LED for error screen
      buttons.SetLED(BUTTON_A, LED_ON);
      buttons.SetLED(BUTTON_B, LED_ON);
        break;
    default:
        break;}
}
else
{
    // Sleep mode triggered, turn off all LEDs
      buttons.SetLED(BUTTON_A, LED_OFF);
      buttons.SetLED(BUTTON_B, LED_OFF);
      }


// Dominant Event is error state  

 if (SYS.Error_Code != ERROR_NONE) {
  if (millis() - SYS.Last_Run > 5000) { // If 5 seconds have passed since error occurred
    SYS.Error_Code = ERROR_NONE; // Clear error code after displaying for 5 seconds
    display.set_error_code(SYS.Error_Code); // Update display to clear error message
    display.Set_Current_Screen(display.SCREEN_MAIN);
    }
    else
    {
    display.Set_Current_Screen(display.SCREEN_ERROR) ;
    display.force_update(); // Force update to show error screen immediately
    display.set_error_code(SYS.Error_Code, "KOM ERROR!"); // Set error code and message for display

    }


 //reste error code after displaying
 
 }

  // Reakt on system state changes

  // Update the display content
  display.update();
 

while(millis() < wait_time + 10);// wait for 10 ms before next loop iteration to avoid excessive CPU usage

   }


