#ifndef MAIN_H
#define MAIN_H
#include <SoftwareSerial.h> // Include Software Serial library
#include <Arduino.h> // Include Arduino core library  
#include <Display.h> // Include Display module
#include "Buttons/Buttons.h"  // Include Button module
#include <Serial.h> // Include Serial communication module
#include <protokoll.h> // Include Protokoll module Define of  Protokoll 






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






struct SYS_
{
uint8_t Learnsatus; // Learn mode status
uint8_t State; // System state variable
double time_stamp; // System timestamp
double time_interval; // System time interval
double Last_Run; // Last recorded time /* data */
}  SYS; // System structure instance
void init_system();




#endif // MAIN_H