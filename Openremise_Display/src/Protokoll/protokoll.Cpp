// Protokoll handler implementation file
#include "protokoll.h"



/// @brief  Initialize the Protokoll module
/// @return     0 on success, error code otherwise


int8_t OpenRemiseProtokoll::begin() {
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
  

// Parse Data stream 
// Create Protokoll messages
// Handle command codes
// Start learning mode

int8_t OpenRemiseProtokoll::update(const char* JSONString) {
        // Implementation for getting JSON data
        // get data  from Uart until  ":" or end of stream
        // Parse JSON string and extract relevant information
        // Store extracted data in appropriate data structures
        // Return 0 on success, error code otherwise
  //https://arduinojson.org/v7/tutorial/deserialization/
            DynamicJsonDocument doc(512);
            DeserializationError error = deserializeJson(doc, JSONString);
            if (error) {
                // Handle JSON parsing error
                return (0xFF); // Error code for JSON parsing failure
            }
    
            // Example: Extracting an integer value from the JSON
            if (doc.containsKey(JSON_ID)) {
                int id = doc[JSON_ID];
                // Process the extracted id as needed
            }    
            
            // detect json element with eeprom Stored names 
            // detect json element with flash Stored names
            // update runtime variables accordingly

        return 0; // Placeholder return value

    } ;

int8_t OpenRemiseProtokoll::StartLearnMode() {
    // Implementation for starting learn mode
    // Set system state to learn mode
    // Initialize learn mode parameters

    return 0; // Placeholder return value

}   
int8_t OpenRemiseProtokoll::StopLearnMode() {
        // Implementation for stopping learn mode
        // Reset system state from learn mode to normal mode

        return 0; // Placeholder return value

    }   
// this function will be not ussed in the first release

int8_t OpenRemiseProtokoll::SendProtokollMessage(const Protokoll& message) {
          // Implementation for sending Protokoll message
            // Format message according to Protokoll specifications
            // Transmit message via appropriate communication interface
            return 0; // Placeholder return value

    }



    //end of Protokoll/protokoll.Cpp
