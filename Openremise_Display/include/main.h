#pragma once

#include <SoftwareSerial.h> // Include Software Serial library
#include <Arduino.h> // Include Arduino core library  
#include "Buttons.h"  // Include Button module
#include "protokoll.h" // Include Protokoll module Define of  Protokoll 
#include "Display.h" // Include Display module
#define SW_Version "Rev 1.0.0"
// Global Variables
#define ON 1
#define OFF 0
#define INIT 0
#define RUN 1   

//#define SERIAL_DEBUG // Enable Serial debugging
#define Feedback_LED_PIN 13 // Pin for Feedback LED
#define SLEEP_TIMEOUT 30000 // Time in milliseconds before entering sleep mode
#define DISPLAY_UPDATE_INTERVAL 150 // Minimum time interval between display updates in milliseconds
#define WAIT_TIME_STARTTIME 15000 // Time to wait for data at startup in milliseconds
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
bool sleep_triggered; // Flag to track if sleep mode has been triggered
ScreenId actuallscreen; // Variable to track the current screen index
} SYS; // System structure instance

enum Mode {
    MODE_INIT,
    MODE_LEARN,
    MODE_NORMAL,
    MODE_NETWORK,
    MODE_RESET,
    MODE_SLEEP,
    MODE_ERROR =0xFF
};  

enum ErrorCodes {
    ERROR_NONE,
    ERROR_DISPLAY =0x01,
    ERROR_BUTTONS=0x02,
    ERROR_PROTOKOLL=0x04,
    ERROR_S3_NO_WIFI=0x10,
    ERROR_S3_SHORTCUT=0x20,
    
    // Add more error codes as needed
    ERROR_UNKNOWN = 0xFF
};

//system funktions 

void Set_LEDS (ScreenId idx);
void Set_Content(ScreenId idx);  
void Set_next_Screen( ScreenId id, uint8_t button_value );


