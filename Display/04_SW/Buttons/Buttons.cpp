#include "Buttons.h"

/// @brief 
/// @param Pin_a 
/// @param LED_a 
/// @param Pin_b 
/// @param LED_b 
/// @return 
int8_t Buttons::begin(int Pin_a, int LED_a, int Pin_b, int LED_b)
{
    // Implementation for beginning button operation
    // Set up initial states, configure hardware, etc.
        // Initialization code for buttons
    // Set up button hardware or software components
    // Example: Configure GPIO pins, initialize button states, etc.
    this->pinA = pinA;
    this->pinB = pinB;
    this->ledA = ledA;      
    this->ledB = ledB;    
    
    pinMode(Pin_a, Input);
    pinMode(LED_a, Output);
    pinMode(Pin_b, Input);
    pinMode(LED_b, Output);
    digitalWrite(LED_a, LOW);
    digitalWrite(LED_b, LOW);
    Button= Button_none;
    Timer[BUTTON_A] = 0; // Timer for button A
    Timer[BUTTON_B] = 0; // Timer for button B

    HoldTime = 1000; // milliseconds
    ResetTime = 5000;// milliseconds

    Button_A_Hold = false;
    Button_B_Hold = false;
    writedigital(LED_a, LOW);
    writedigital(LED_b, LOW);

    return (true);
}


/// @brief 
///@todo shift logic from read to update
// make  Button´as private  member variable
/// @return 

void Buttons::update() {
    // Code to update button states
    // Poll button hardware or software components to refresh their states
    // Example: Read GPIO pins, debounce buttons, update state variables, etc.




}

/// @brief 
////@todo shift logic from read to update
/// @return 
uint8_t Buttons::read() {
    // Implementation for reading button states
    uint8_t Button= NO_BUTTONS;
    
    // Read button states
    if (isPressed(BUTTON_A)) {
        if (TimerStart[BUTTON_A] == false) {
            Timer[BUTTON_A] = millis(); // Start timer
            TimerStart[BUTTON_A] = true;
             Button |= BUTTON_A_PRESSED; //  mask the  1 in Button value
             Button_A_Hold = false;
        } else {
            
            // Check for hold time
            if (!Button_A_Hold && (millis() - Timer[BUTTON_A] >= HoldTime)) {
                Button |= BUTTON_A_HOLD; // mask the hold in Button value
                Button_A_Hold = true;
            }
        }
    }
    else {  //button released 
        TimerStart[BUTTON_A] = false;
        Button_A_Hold = false;
        //mask 0 into Button value
        Button &= 0xF0;
    }

    if (isPressed(BUTTON_B)) {
        if (TimerStart[BUTTON_B] == false) {
            Timer[BUTTON_B] = millis(); // Start timer
            TimerStart[BUTTON_B] = true;
             Button = Button || BUTTON_B_PRESSED; // mask the  1 in Button value
             Button_B_Hold = false;
        } else {
            // Check for hold time
             if (!Button_B_Hold && (millis() - Timer[BUTTON_B] >= HoldTime)) {
                Button = Button || BUTTON_B_HOLD;   
                Button_B_Hold = true;
            }
        }

    }else {  //button released 
        TimerStart[BUTTON_B] = false;
        Button_B_Hold = false;
        Button &= 0x0F;
    }
    
    
    
    //reset timer when both button are pressed for 5 sec
    if ((Button_B_Hold == true) && (Button_A_Hold == true)) {
       // reset command  after 5sec 
        TimerStart[BUTTON_A] = true;
        TimerStart[BUTTON_B] = true;
       
        
        Button |= Button_B_pressed;
        Button |= Button_A_pressed;
        if (millis() - Timer[BUTTON_A] >= ResetTime && millis() - Timer[BUTTON_B] >= ResetTime) {
            Button = NO_BUTTONS; // reset command
            TimerStart[BUTTON_A] = false;
            TimerStart[BUTTON_B] = false;
            Button_A_Hold = false ;
            Button_B_Hold = false;
            Button = RESET_REQUEST;
        }
        
    }
    else {
        // If not both pressed, ensure timers are not running
        //normal operation
        }
    


    digitalWrite(LED_a, Button_A_Hold); // light LED when button pressed
    digitalWrite(LED_b, Button_B_Hold); // light LED when button pressed 


    // detect edge  and start hold timer
  //reset timer when button released


    return Button; //  return value Return current button states

}


// privat functions 


bool Buttons::isPressed(uint8_t buttonId) {
    switch (buttonId) {
        case BUTTON_A:
            return digitalRead(pinA) == HIGH;

        case BUTTON_B:
            return digitalRead(pinB) == HIGH;

        default:
            return false;
    }
}


// End of Buttons/Buttons.cpp