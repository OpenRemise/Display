#ifndef MAIN_H
#define MAIN_H
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
#define ERROR_STATE 0xff
#define LEARN_MODE 0x01
#define NORMAL_MODE 0x00
#define ERROR_MODE 0x0F
#define SERIAL_DEBUG // Enable Serial debugging
#define Feedback_LED_PIN 13 // Pin for Feedback LED

//Button definition 
#define BUTTON_A_PIN 2
#define BUTTON_B_PIN 3
#define LED_A_PIN 4
#define LED_B_PIN 5

struct SYS_
{
uint8_t Learnsatus; // Learn mode status
uint8_t State; // System state variable
double time_stamp; // System timestamp
double time_interval; // System time interval
double Last_Run; // Last recorded time /* data */
} SYS; // System structure instance


void SYS_Init();




#endif // MAIN_H