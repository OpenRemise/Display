#include "Buttons.h"

/// @brief Constructor
Buttons::Buttons() {
    // Initialization code for Buttons class
    // Set up initial states, configure member variables, etc.
    Button_state = NO_BUTTONS;

}

/// @brief 
/// @param Pin_a 
/// @param LED_a 
/// @param Pin_b 
/// @param LED_b 
/// @return 
uint8_t Buttons::begin(int Pin_a, int LED_a, int Pin_b, int LED_b)
{
    // Implementation for beginning button operation
    // Set up initial states, configure hardware, etc.
        // Initialization code for buttons
    // Set up button hardware or software components
    // Example: Configure GPIO pins, initialize button states, etc.
    this->pinA = Pin_a;
    this->pinB = Pin_b;
    this->ledA = LED_a;      
    this->ledB = LED_b;    
    
    pinMode(Pin_a, INPUT);
    pinMode(LED_a, OUTPUT);
    pinMode(Pin_b, INPUT);
    pinMode(LED_b, OUTPUT);
 // Ensure LEDs are off at start
    digitalWrite(LED_a, LED_OFF);
    digitalWrite(LED_b, LED_OFF);
    Button_state = NO_BUTTONS;
// Initialize member variables 
 // timers 
    TimerStart[BUTTON_A] = false;
    TimerStart[BUTTON_B] = false;
    Timer[BUTTON_A] = 0; // Timer for button A
    Timer[BUTTON_B] = 0; // Timer for button B

    HoldTime = BUTTON_HOLD_TIME; // milliseconds
    ResetTime = BUTTON_RESET_TIME;// milliseconds
 // button hold states
    Button_A_Hold = false;
    Button_B_Hold = false;


    return (true);
}


/// @brief 
///@todo shift logic from read to update
// make  Button´as private  member variable
/// @return 

uint8_t Buttons::update() {
    // Code to update button states
    // Poll button hardware or software components to refresh their states
    // Example: Read GPIO pins, debounce buttons, update state variables, etc.


    // Read button states
    if (isPressed(BUTTON_A)) {
        if (TimerStart[BUTTON_A] == false) {
            Timer[BUTTON_A] = millis(); // Start timer
            TimerStart[BUTTON_A] = true;
             Button_state |= BUTTON_A_PRESSED; //  mask the  1 in Button value
             Button_A_Hold = false;

        } else {
            
            // Check for hold time
            if (!Button_A_Hold && (millis() - Timer[BUTTON_A] >= HoldTime)) {
                Button_state |= BUTTON_A_HOLD; // mask the hold in Button value
                Button_A_Hold = true;
            }
        }
    }
    else {  //button released 
        TimerStart[BUTTON_A] = false;
        Button_A_Hold = false;
        //mask 0 into Button value
        Button_state &= mask_out_Button_A;
    }


    if (isPressed(BUTTON_B)) {
        if (TimerStart[BUTTON_B] == false) {
            Timer[BUTTON_B] = millis(); // Start timer
            TimerStart[BUTTON_B] = true;
             Button_state |= BUTTON_B_PRESSED; // mask the  1 in Button value
             Button_B_Hold = false;
        } else {
            // Check for hold time
             if (!Button_B_Hold && (millis() - Timer[BUTTON_B] >= HoldTime)) {
                Button_state |= BUTTON_B_HOLD;   
                Button_B_Hold = true;
            }
        }

    }else {  //button released 
        TimerStart[BUTTON_B] = false;
        Button_B_Hold = false;
        Button_state &= mask_out_Button_B;
    }

    
    
    //reset timer when both button are pressed for 5 sec
    if ((Button_B_Hold == true) && (Button_A_Hold == true)) {
       // reset command  after 5sec 
        TimerStart[BUTTON_A] = true;
        TimerStart[BUTTON_B] = true;
       
        
        Button_state |= BUTTON_A_PRESSED;
        Button_state |= BUTTON_B_PRESSED;
        if (millis() - Timer[BUTTON_A] >= ResetTime && millis() - Timer[BUTTON_B] >= ResetTime) {
            TimerStart[BUTTON_A] = false;
            TimerStart[BUTTON_B] = false;
            Button_A_Hold = true ;
            Button_B_Hold = true;
            Button_state = RESET_REQUEST;// reset command
        }
        
    }
    else {
        
        //normal operation

        }
    





    // detect edge  and start hold timer
  //reset timer when button released



return (Button_state); //  return value Return current button states

}

/// @brief 
////@todo shift logic from read to update
/// @return 
uint8_t Buttons::read() {
    //  just get the Button states
    return Button_state; 

}

bool Buttons::SetLED(uint8_t led, bool state) {
    // Implementation for setting LED states
    switch (led) {
        case BUTTON_A:
            digitalWrite(ledA, state ? HIGH : LOW);
            break;
        case BUTTON_B:
            digitalWrite(ledB, state ? HIGH : LOW);
            break;
        default:
            return 1; // Error: Invalid LED identifier
    }
    return 0; // Success
}

// privat functions 


bool Buttons::isPressed(uint8_t buttonId) {
    switch (buttonId) {
        case BUTTON_A:
            return digitalRead(pinA) == LOW; // Assuming active LOW

        case BUTTON_B:
            return digitalRead(pinB) == LOW; // Assuming active LOW

        default:
            return false;
    }
}


// End of Buttons/Buttons.cpp