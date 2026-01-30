#include <Arduino.h>
#include <Wire.h>
#include "Display.h"
#include "Buttons.h"

/* SH1107 128x128, I2C, Hardware I2C, Page-Mode */
OpenRemiseDisplay display;
// buttons for up and down 
Buttons buttons;


void setup() {
  Serial.begin(115200);
  // Buttons initialisieren
        buttons.begin(PIND4, PIND5, PIND6 ,PIND7); // Button and LED pins
          buttons.SetLED(BUTTON_A, true);
          buttons.SetLED(BUTTON_B, true);
  // Display initialisieren
  Serial.println(F("SH1107 Init Test"));
    uint8_t ret = display.begin();
    if (ret != 0x00) {
        Serial.println(F("Display init failed!"));
         }
   
    
     if (ret == 0x00) { 
      // if  display is ok  
        // Show welcome screen
        display.ShowWelcome();
     // if  not blink with button LEDS 
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
//Wait  for S3json data send 
// if no dat send with in the first 5 sec show error

buttons.SetLED(BUTTON_B, true);



//Initend
buttons.SetLED(BUTTON_B, false);
buttons.SetLED(BUTTON_A, false);
display.clear();
display.ShowNormalMode();

}

void loop() {
  //absichtlich leer
  /* wait vor Event 
  buttons.update();
  if ( buttons.read() != NO_BUTTONS ) {
     if (buttons.read() & BUTTON_A_PRESSED) {
        Serial.println(F("Button A pressed!"));
     }
     if (buttons.read() & BUTTON_B_PRESSED) {
        Serial.println(F("Button B pressed!"));
     }
  }
  delay(100);*/
   }


