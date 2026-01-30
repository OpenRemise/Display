#pragma once


#include <Arduino.h>
#include <stdio.h>

// some defines for button states

#define BUTTON_A_PRESSED  0x10
#define BUTTON_B_PRESSED  0x01
#define BUTTON_A_HOLD     0x20
#define BUTTON_B_HOLD     0x02
#define NO_BUTTONS        0x00
#define BUTTON_A          0x00 //Index for Button A
#define BUTTON_B          0x01 //Index for Button B
#define RESET_REQUEST     0xFF

// Define the Buttons class
class Buttons {
public:
    Buttons(); // Constructor
    
    uint8_t begin(int Pin_a, int LED_a ,int Pin_b, int LED_b );
    uint8_t read();// Read button states application side 
    uint8_t update() ; // Update button states HW side
    bool   SetLED(uint8_t led, bool state); // set LED state

private:
    // Private member variables and functions for button management
    // Example: Button state variables, debounce timers, etc.
   bool isPressed(uint8_t buttonId);// Check if button is pressed
    
    unsigned long Timer[2] = {0, 0}; // Timer for button A and B
    bool     TimerStart[2] = {0, 0}; // Start time for button A and B

    unsigned long HoldTime = 1000; // milliseconds
    unsigned long ResetTime = 5000;// milliseconds

    bool Button_A_Hold = false;
    bool Button_B_Hold = false;
    uint8_t pinA;
    uint8_t pinB;
    uint8_t ledA;
    uint8_t ledB;
    uint8_t Button = NO_BUTTONS;

};


// End of Buttons.h
