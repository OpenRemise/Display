// Protokoll handler implementation file
#include "protokoll.h"

/// @brief  Initialize the Protokoll module
/// @return     0 on success, error code otherwise

/// @brief 
OpenRemiseProtokoll::OpenRemiseProtokoll()
  : _transport(nullptr),
    rxIndex(0),
    rxComplete(false)
{
}


uint8_t OpenRemiseProtokoll::begin(Stream& transport)
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
 _transport = &transport;

  rxIndex = 0;
  rxComplete = false;


    data_controll.Data_send =false;
    data_controll. Data_received= false;  //s3 daten empangen 
    data_controll.Data_error = NO_Data;
    data_controll.lastDataMillis = 0;
  
    return 0; // Placeholder return value
}

// Protocoll related functions are implemented here.

int8_t OpenRemiseProtokoll::update()
{    // Transfer last data  from Jason to display
    if (true == data_controll.Data_received ) 
    { if( Data_Valid == data_controll.Data_error )
        {
             // transfer data
         }


    }


return 0;
}

void OpenRemiseProtokoll::poll()
{
  while (_transport->available()) {
    char c = _transport->read();

    if (c == '\n') {
      rxBuffer[rxIndex] = '\0';
      rxComplete = true;
      rxIndex = 0;
      return;
    }

    if (rxIndex < RX_BUFFER_SIZE - 1) {
      rxBuffer[rxIndex++] = c;
    }
  }
}


void OpenRemiseProtokoll::process()
{
  if (!rxComplete) return;

  rxComplete = false;

 
 /* DeserializationError err = deserializeJson(doc, rxBuffer);

  if (err) {
    data_controll.Data_error = Data_Parse_Error;
   // lastError = Data_InvalidJson;
    return;
  }

 // parseJson(doc);*/

  data_controll.lastDataMillis = millis();
  data_controll.Data_error = Data_Valid;
}


int8_t OpenRemiseProtokoll::parseJson(const char* json_buffer) {
    // Implementation for parsing JSON data
    // Use ArduinoJson library to parse the JSON string
    // Extract relevant information and store in data structures
    // Return 0 on success, error code otherwise


//deserializeJson(doc, _transport);
// Example: Extracting values from JSON
//const char* id = doc[JSON_ID]; // "id" element 
//const char* ip = doc[JSON_IP]; // "ip" element
//const int status = doc[JSON_STATUS]; // "status" element
//const char* service = doc[JSON_Service]; // "service" element
    //....
// Further processing of extracted data




        return 0; // Placeholder return value

    } ;



Protokoll_Data_error_t  OpenRemiseProtokoll:: Checktimeout()
{
   if (Data_Valid != data_controll.Data_error) {
    return NO_Data;   // optional, aber elegant
  }
if ( millis() - data_controll.lastDataMillis > PROTOKOLL_DATA_TIMEOUT){
    return Data_Timeout;
}
return  Data_Ok;
}

    //end of Protokoll/protokoll.Cpp
