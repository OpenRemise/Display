// Protokoll handler implementation file
#include "protokoll.h"

/// @brief  Initialize the Protokoll module
/// @return     0 on success, error code otherwise

/// @brief 
OpenRemiseProtokoll::OpenRemiseProtokoll(Stream& transport)
: _transport(&transport)
{

}


int8_t OpenRemiseProtokoll::begin()
{
    // Implementation for initializing the Protokoll module
    // Clear Counter for messages, buffers, etc.
    // Check EEprom on validity
    // Initialize know values from EEprom
    // Check free memory from Epprom
    // Transfern known Json to runtime variables
    // Setup Protokoll parameters
    // read out EEPROM values ( json names only )
    // Read out Flash values ( json names only from learning mode )
    // Sort names in alphbetic order for faster search
    
    // Initialize learn mode active flag
    learn_mode_active = false;


    return 0; // Placeholder return value
}

// Protocoll related functions are implemented here.

int8_t OpenRemiseProtokoll::update()
{        // Implementation for getting JSON data
        // get data  from Uart until  ":" or end of stream
        // Parse JSON string and extract relevant information
        // Store extracted data in appropriate data structures
        // Return 0 on success, error code otherwise
  //https://arduinojson.org/v7/tutorial/deserialization 
            // detect json element with eeprom Stored names 
            // detect json element with flash Stored names
            // update runtime variables accordingly

    static uint8_t idx = 0;

    while (_transport->available()) {
        char c = _transport->read();

        if (c == '\n') {
            json_buffer[idx] = '\0';
            idx = 0;

            // hier JSON auswerten
            

            return parseJson(json_buffer);
        }

        if (idx < sizeof(json_buffer) - 1) {
            json_buffer[idx++] = c;
        }
    }


return 0;
}



int8_t OpenRemiseProtokoll::parseJson(const char* json_buffer) {
    // Implementation for parsing JSON data
    // Use ArduinoJson library to parse the JSON string
    // Extract relevant information and store in data structures
    // Return 0 on success, error code otherwise

StaticJsonDocument<356> doc;

deserializeJson(doc, _transport);
// Example: Extracting values from JSON
const char* id = doc[JSON_ID]; // "id" element 
const char* ip = doc[JSON_IP]; // "ip" element
const int status = doc[JSON_STATUS]; // "status" element
const char* service = doc[JSON_Service]; // "service" element
    //....
// Further processing of extracted data




        return 0; // Placeholder return value

    } ;

int8_t OpenRemiseProtokoll::StartLearnMode() {
    // Implementation for starting learn mode
    // Set system state to learn mode
    // Initialize learn mode parameters
        learn_mode_active = true;

        return learn_mode_active; // Placeholder return value

}   
int8_t OpenRemiseProtokoll::StopLearnMode() {
        // Implementation for stopping learn mode
        // Reset system state from learn mode to normal mode
        learn_mode_active = false;


        return learn_mode_active; // Placeholder return value
    }   
// this function will be not ussed in the first release
/*
int8_t OpenRemiseProtokoll::SendProtokollMessage(const Protokoll& message) {
          // Implementation for sending Protokoll message
            // Format message according to Protokoll specifications
            // Transmit message via appropriate communication interface






            return 0; // Placeholder return value

    }

*/

    //end of Protokoll/protokoll.Cpp
