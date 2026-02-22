#pragma once
 #define PROTOKOLL_MANUAL // define this to prevent automatic inclusion of the implementation file
// This allows for manual control implementation, which can be useful for smaler 
 
#include "stdint.h"
#include <Arduino.h>

#ifndef PROTOKOLL_MANUAL // If PROTOKOLL_MANUAL is defined, the implementation will be provided in a separate file (e.g., Protokoll_2.cpp)
#include <ArduinoJson.h> // JSON handling library see https://arduinojson.org/v7
#endif 

// Define Protokoll related constants

// Command codes
#define ALIVE 0x07              // Keep-alive signal       1 byte of data
#define STATE1 0x11             // State update            6 bytes of data
#define STATE2 0x12             // State update            4 bytes of data
#define ERROR  0x13             // Error message           2 bytes of data
#define WRITE_DIRECT 0x02       // Direct write command    8 bytes of data
#define WRITE_END 0x03          // End write command       1 byte of data   
#define CLEAR_DISPLAY 0x04      // Clear display command   1 byte of data
#define JSON_DISPLAY_CLEAR "display_clear" // Clear display command True / false

/********
 * What S3  Sends to the Protokoll handler to be Known
 * all marked with # are not relevant to be  displayed. 
 * {
 * "state":"Suspended",
 * # "version":"0.6.1",
 * # "project_name":"Firmware",
 * # "compile_time":"18:21:03",
 * # "compile_date":"Feb  8 2026",
 * # "idf_version":"5.5.2",
 * "mdns":"BfB_remise",
 * "ip":"192.168.20.108",
 * # "mac":"b4:3a:45:f9:05:a0\u0000",
 * "rssi":-35,
 * "voltage":12413,
 * "current":0,
 * "temperature":33.4,
 * # "heap":16572060,
 * # "internal_heap":24243
 * }
 */

// More JSON elements can be added as needed
//define Json Data elements 
#define JSON_ID         "version"// Unique identifier for the SW Version S3
#define JSON_IP         "ip"    // IP address as an array of 4 integers
#define JSON_STATUS      "status"    // Status code
#define JSON_WIFI_SSID    "ssid"  // WiFi SSID
#define JSON_MDNS         "mdns"
#define JSON_WIFI_PWR     "rssi"   // WiFi Power

#define JSON_TRK_VOLTAGE  "voltage" // Track voltage
#define JSON_TRK_CURRENT  "current" // Track current
#define JSON_SUP_VOLTAGE  "sup_voltage" // Supply voltage
#define JSON_Service      "service"   // Service code

/*Unsuported yet
#define JSON_ERROR_CODE     "error_code"        // Error code

//Debug and Learn mode
#define JSON_LEARN_MODE "learn_mode" // start / stop Learn mode True / false/ display 
#define JSON_LEAN_VALUE "learn_value" // Value for learn mode 
#define JSON_TEXT_START "Write Start"
#define JSON_DISPLAY_TEXT "display_text" // start / stop Text to be shown on the display True / false 
#define JSON_TEXT_STOPP "Write stopp"
 */





// Modul defines 
#define PROTOKOLL_DATA_TIMEOUT 2000
#define RX_BUFFER_SIZE  30 // may  adapt to a better value
#define ERROR_TEXT_LEN  10 
#define ERROR_INFO_LEN  10
#define MAX_STRING  20


// Json Memory mapping  

enum  Data_from_Stream_IDX
{ IDX_NONE,//  zero element 
IDX_ID      ,  
IDX_IP        ,
IDX_STATUS     ,
IDX_SERVICE    ,
IDX_WIFI_SSID  ,
IDX_WIFI_PWR   ,
IDX_TRK_VOLTAGE,
IDX_TRK_CURRENT,
IDX_SUP_VOLTAGE,
IDX_JSON_MDNS   ,
IDX_MAX
};

struct JsonMap
{
    const char*          jsonKey;
    Data_from_Stream_IDX dataId;
};

static const JsonMap jsonMap[] =
{
    {JSON_ID         ,   IDX_ID          },
    {JSON_IP         ,   IDX_IP          },
    {JSON_WIFI_SSID  ,   IDX_WIFI_SSID   },
    {JSON_WIFI_PWR   ,   IDX_WIFI_PWR    },
    {JSON_STATUS     ,   IDX_STATUS      },
    {JSON_Service    ,   IDX_SERVICE     }, 
    {JSON_TRK_VOLTAGE,   IDX_TRK_VOLTAGE },
    {JSON_TRK_CURRENT,   IDX_TRK_CURRENT },
    {JSON_SUP_VOLTAGE,   IDX_SUP_VOLTAGE },
    {JSON_MDNS  ,        IDX_JSON_MDNS   }
  };
static const size_t JSON_MAP_SIZE = sizeof(jsonMap) / sizeof(jsonMap[0]);


// error definitions 

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

enum Flow_control_t{
  Flow_None,
  Flow_Reading_Key,
  Flow_Reading_Value,
  Flow_Error
};



// Define Class  

class OpenRemiseProtokoll {

public:
// Constructor
 OpenRemiseProtokoll();

// int8_t SendProtokollMessage(const Protokoll_data& message) ;
//Functions

uint8_t begin( Stream& transport);
int8_t update();

// return Pointer on Protokoll buffer !!
const char* getData(Data_from_Stream_IDX id);
Data_from_Stream_IDX  getProtIndex(const char* key);

Protokoll_Data_error_t Get_Protokoll_Error ();




private:
    // Private member variables and functions for Protokoll management
    // Example: Message buffers, state variables, etc.

  struct data_controll_t
  { bool Data_received;  //s3 daten empangen 
    bool Data_send;      // daten an S3 gesendet 
    unsigned long lastDataMillis;
    const unsigned long DATA_TIMEOUT_MS = PROTOKOLL_DATA_TIMEOUT ;
    Protokoll_Data_error_t Data_error;  
    Flow_control_t Flow_control; // optional, for managing data flow control if needed
    Data_from_Stream_IDX currentIdx;
  };

  data_controll_t data_controll;
// Data_from_Stream_t Data_from_Stream;
//### DATA Array  mit den passendne Strings
    char data[IDX_MAX][MAX_STRING];

  //runtime variables
    Stream* _transport = nullptr; // Transport stream (e.g., Serial)
    //Serial buffer 

    char rxBuffer[RX_BUFFER_SIZE];
     uint8_t rxIndex; 
      bool rxComplete;

#ifndef PROTOKOLL_MANUAL
// buffer for incoming JSON data
//char json_buffer[358]; // adjust size as needed
 JsonDocument doc ;
#endif

 // Privat functions 
 Protokoll_Data_error_t Checktimeout();  
 void poll();
 void process();

};



// end of PROTOKOLL_H   