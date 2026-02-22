

// Protokoll handler implementation file
#include "protokoll.h"

#ifndef PROTOKOLL_MANUAL // If PROTOKOLL_MANUAL is defined,
// the implementation will be provided in a separate file (e.g., Protokoll_2.cpp)
// This allows for manual control Aover the inclusion of the implementation, which can be useful for testing
// or when the implementation is large and you want to keep it separate from the header.

/// @brief  Initialize the Protokoll module
/// @return     0 on success, error code otherwise

/// @brief 


OpenRemiseProtokoll::OpenRemiseProtokoll()
  : _transport(nullptr),
    rxIndex(0)
{
}

/****************************************************************************************/
/****************************************************************************************/
/****************************************************************************************/

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
 
    data_controll.Data_send =false;
    data_controll.Data_received= false;  //s3 daten empangen ?
    data_controll.Data_error = NO_Data;
    data_controll.lastDataMillis = 0;
  
    return 0; // Placeholder return value
}

// Protocoll related functions are implemented here.

int8_t OpenRemiseProtokoll::update()
{ poll();     // Bytes einsammeln
  process();  // falls kompletter Datensatz → parsen

  // Timeout überwachen
  if (Checktimeout() == Data_Timeout) {
   data_controll.Data_error = Data_Timeout;
    return Data_Timeout;
  }

  return Data_Ok;
}


/****************************************************************************************/

Protokoll_Data_error_t OpenRemiseProtokoll::Get_Protokoll_Error (){
  return data_controll.Data_error;
};

Data_from_Stream_IDX OpenRemiseProtokoll :: getProtIndex(const char* key)
{
    for (size_t i = 0U; i < (sizeof(jsonMap) / sizeof(jsonMap[0])); ++i)
    {
        if (strcmp(key, jsonMap[i].jsonKey) == 0)
        {
            return jsonMap[i].dataId;
        }
    }

    return IDX_NONE;
}



/****************************************************************************************/


// collect data 
void OpenRemiseProtokoll::poll()
{
  while (_transport->available()) {
    char c = _transport->read();

    if (c == '\n') {
      rxBuffer[rxIndex] = '\0';
      data_controll. Data_received = true;
      rxIndex = 0;
      return;
    }

    if (rxIndex < RX_BUFFER_SIZE - 1) {
      rxBuffer[rxIndex++] = c;
    }
  }
}

/****************************************************************************************/

void OpenRemiseProtokoll::process()
{
  if (!data_controll.Data_received) return;
  data_controll. Data_received= false;

  DeserializationError err = deserializeJson(doc, rxBuffer);
  if (err) {
    data_controll.Data_error = Data_Parse_Error;
    return;
  }
//  daten  direkt in den Protokoll speicher schreiben  
//  speicherung als string 
    // JSON-Objekt durchlaufen
    for (JsonPair kv : doc.as<JsonObject>())
    {
        const char* key   = kv.key().c_str();
        const char* value = kv.value().as<const char*>();

        Data_from_Stream_IDX idx = getProtIndex(key);

        if (idx != IDX_NONE)
        {
           if (kv.value().is<const char*>())
           {
               value = kv.value().as<const char*>();
           }
           else
           {
               static char numBuffer[16];
               snprintf(numBuffer, sizeof(numBuffer), "%ld",
                        kv.value().as<long>());
               value = numBuffer;
           }
        }
    }

  data_controll.lastDataMillis = millis();
  data_controll.Data_error = Data_Valid;
}

  const char* OpenRemiseProtokoll:: getData(Data_from_Stream_IDX id)
    {
        if(id >= IDX_MAX)
            return (""); 
        return data[id];
    }

/****************************************************************************************/

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
#endif // PROTOKOLL_MANUAL