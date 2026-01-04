
#ifndef PROTOKOLL_H
#define PROTOKOLL_H
// Protokoll.h contain definition of  data with in the Protokoll 
#include <cstdint>
// old School type definition
struct Protokoll {
    int id;               // Unique identifier for the protocol message
    uint8_t Data[8]    ;  // Data payload
};  

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

class protokoll
{
private:
    // Private member variables and functions for Protokoll management
    // Example: Message buffers, state variables, etc.

public:
int8_t StartLearnMode();
int8_t StopLearnMode();    
int8_t GetJSONdata(const char* JSONString);
int8_t SendProtokollMessage(const Protokoll& message);
int8_t begin();
}


#endif // PROTOKOLL_H   