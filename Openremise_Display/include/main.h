#pragma once

#include <SoftwareSerial.h> // Include Software Serial library
#include <Arduino.h> // Include Arduino core library  
#include "Display.h" // Include Display module
#include "Buttons.h"  // Include Button module
#include "protokoll.h" // Include Protokoll module Define of  Protokoll 




// Global Variables
#define ON 1
#define OFF 0
#define INIT 0
#define RUN 1   

//#define SERIAL_DEBUG // Enable Serial debugging
#define Feedback_LED_PIN 13 // Pin for Feedback LED
#define SLEEP_TIMEOUT 30000 // Time in milliseconds before entering sleep mode

// Pin usage definitions

//Button definition 
#define BUTTON_A_PIN 4
#define LED_A_PIN 5
#define BUTTON_B_PIN 6
#define LED_B_PIN 7

// SoftSerial pins for Protokoll communication
#define PROTOKOLL_RX_PIN 10
#define PROTOKOLL_TX_PIN 11
/* I2C  Just for reference if needed
#define I2C_SDA_PIN A4
#define I2C_SCL_PIN A5  
*/



// System state structure

struct SYS_
{
uint8_t State; // System state variable
uint8_t Error_Code; // Error code variable
double time_stamp; // System timestamp
double time_interval; // System time interval
double Last_Run; // Last recorded time /* data */
} SYS; // System structure instance

enum Mode {
    MODE_INIT,
    MODE_LEARN,
    MODE_NORMAL,
    MODE_NETWORK,
    MODE_SLEEP,
    MODE_ERROR =0xFF
};  

enum ErrorCodes {
    ERROR_NONE,
    ERROR_DISPLAY,
    ERROR_BUTTONS,
    ERROR_PROTOKOLL,
    ERROR_UNKNOWN = 0xFF

    // Add more error codes as needed
};

