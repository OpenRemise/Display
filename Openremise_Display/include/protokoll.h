#pragma once

// protokoll.h contain definition of  data with in the Protokoll 
#include "stdint.h"
#include <Arduino.h>
#include <ArduinoJson.h> // JSON handling library see https://arduinojson.org/v7
// Define Protokoll related constants

// Command codes
#define ALIVE 0x07              // Keep-alive signal       1 byte of data
#define STATE1 0x11             // State update            6 bytes of data
#define STATE2 0x12             // State update            4 bytes of data
#define ERROR  0x13             // Error message           2 bytes of data
#define WRITE_DIRECT 0x02       // Direct write command    8 bytes of data
#define WRITE_END 0x03          // End write command       1 byte of data   
#define CLEAR_DISPLAY 0x04      // Clear display command   1 byte of data

//define Json elements 
#define JSON_ID "id" // Unique identifier for the protocol message
#define JSON_IP "ip"    // IP address as an array of 4 integers
#define JSON_STATUS "status"    // Status code
#define JSON_Service "service"   // Service code
#define JSON_WIFI_SSID "wifi_ssid"  // WiFi SSID
#define JSON_WIFI_PWR "wifi_pwr"   // WiFi Power
#define JSON_TRK_VOLTAGE "track_voltage" // Track voltage
#define JSON_TRK_CURRENT "track_current" // Track current
#define JSON_SUP_VOLTAGE  "sup_voltage" // Supply voltage
#define JSON_ERROR_CODE "error_code"        // Error code
#define JSON_DISPLAY_CLEAR "display_clear" // Clear display command True / false

//Debug and Learn mode
#define JSON_LEARN_MODE "learn_mode" // start / stop Learn mode True / false/ display 
#define JSON_LEAN_VALUE "learn_value" // Value for learn mode 
#define JSON_DISPLAY_TEXT "display_text" // start / stop Text to be shown on the display True / false 
// More JSON elements can be added as needed


// Modul defines 
#define PROTOKOLL_DATA_TIMEOUT 2000
#define RX_BUFFER_SIZE  30 // may  adapt to a better value


enum Protokoll_Data_error_t{
NO_Data,
 Data_Ok,
 Data_Valid,

// add_as needed
 Data_Timeout =0XFB,
 Data_Parse_Error =0xFC,
 Data_Unknown_Field=0xFD,
 Data_Unknown =0xFE,
 Data_Error =0xFF
};
// Define Class  

class OpenRemiseProtokoll {

public:
// Constructor
 OpenRemiseProtokoll();


// int8_t GetJSONdata(const char* JSONString);
// int8_t SendProtokollMessage(const Protokoll_data& message) ;
//Functions

uint8_t begin( Stream& transport);

int8_t update();

int8_t parseJson(const char* json_buffer) ;

//data 



private:
    // Private member variables and functions for Protokoll management
    // Example: Message buffers, state variables, etc.
  struct data_controll_t
  { bool Data_received;  //s3 daten empangen 
    bool Data_send;      // daten an S3 gesendet 
    unsigned long lastDataMillis;
    const unsigned long DATA_TIMEOUT_MS = PROTOKOLL_DATA_TIMEOUT ;
    Protokoll_Data_error_t Data_error;  
  };
data_controll_t data_controll;




  //runtime variables
    Stream* _transport = nullptr; // Transport stream (e.g., Serial)
    //Serial buffer 

    char rxBuffer[RX_BUFFER_SIZE];
     uint8_t rxIndex; 
      bool rxComplete;

    // buffer for incoming JSON data
//char json_buffer[358]; // adjust size as needed
  //JsonDocument<256> doc;

 // Privat functions 
 Protokoll_Data_error_t Checktimeout();   
 void poll();
 void process();



};



// end of PROTOKOLL_H   