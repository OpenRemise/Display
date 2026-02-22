// Protokoll handler implementation file


#include "protokoll.h"
#ifdef PROTOKOLL_MANUAL // If PROTOKOLL_MANUAL is defined, the implementation will be provided in a separate file (e.g., Protokoll_2.cpp)
// This allows for manual control over the inclusion of the implementation, which can be useful for testing
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
    data_controll.lastDataMillis = millis();
    data_controll.Flow_control = Flow_None; // Initialize flow control state





    return 0; // Placeholder return value
}

// Protocoll related functions are implemented here.

int8_t OpenRemiseProtokoll::update()
{ poll();     // Daten sammeln und einsortieren 
  process();  // Daten protokoll intern verarbeiten, Fehler und Timeout überwachen-

  // Timeout überwachen
  if (Checktimeout() == Data_Timeout) {
   data_controll.Data_error = Data_Timeout;
    return Data_Timeout;
  }

  return data_controll.Data_error;
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
 /* Zeichen Lesen  und bei "{" starten der daten .
 * Zeichen in Buffer speichern bis ":" empfangen wird 
 *KEY mit JSON Map vergleichen und Index für Daten Array merken
 * Zeichen in Buffer speichern bis "," oder "}" empfangen wird
 * Daten in Daten Array an Index speichern
 * bei "}" komplette Daten empfangen → Flag setzen für Verarbeitung
 * auslesen der daten in Main gescheid über getData Funktion mit Index.
 * Es wird der pointer auf den String zurückgegeben, damit die Daten direkt in der Display Funktion verwendet werden können.
 * damit nur einmal Datenhaltung im Protokoll  Kein Jsonarduino Object oder ähnliches nötig ist.
 * Die Daten werden als String gespeichert, damit sie direkt in der Display Funktion verwendet werden können. 
 los gehts  */
Data_from_Stream_IDX idx = IDX_NONE; // Variable to store the index of the current key being processed


  while (_transport->available()) {
    char c = _transport->read(); 

    switch (data_controll.Flow_control) {
      case Flow_None:
        if (c == '{') {
          data_controll.Flow_control = Flow_Reading_Key;
          rxIndex = 0; // Reset buffer index for new message
        }
        break;

      case Flow_Reading_Key:
       //reading the key until ":" is received
      // Once ":" is received, we can process the key and start reading the value --> Transition to Flow_Reading_Value state
        if (c == ':') {
          rxBuffer[rxIndex] = '\0'; // Null-terminate the buffer
          data_controll.Flow_control = Flow_Reading_Value; // Transition to processing data state
          idx = getProtIndex(rxBuffer); // Get index for the current key if no valid  Key is found, it will return IDX_NONE
          rxIndex = 0; // Reset buffer index for value processing
        } else if (rxIndex < RX_BUFFER_SIZE - 1) {
          rxBuffer[rxIndex++] = c; // Store character in buffer and increment index 
        }else {
          data_controll.Flow_control = Flow_Error; // Set error state for buffer overflow
          data_controll.Data_error = Data_Parse_Error; // Set data error for parse error due to buffer overflow
          return; // Exit function on error
        } 
      
        break;
        case Flow_Reading_Value:
        
        //reading the value until "," or "}" is received
        if (c == ',' || c == '}') {
          rxBuffer[rxIndex] = '\0'; // Null-terminate the buffer
          // Process the key-value pair here, e.g., store in data array based on key index
          
          if (idx != IDX_NONE) { //idx IDX_NONE bedeutet, dass kein gültiger Schlüssel gefunden wurde, daher sollten wir nur dann Daten speichern, wenn ein gültiger Schlüssel gefunden wurde
            // Store value in data array at the corresponding index
            strncpy(data[idx], rxBuffer, MAX_STRING - 1); // Copy value to data array with bounds checking
            data[idx][MAX_STRING - 1] = '\0'; // Ensure null-termination
          }
          if (c == '}') {// If '}' is received, it indicates the end of the message
            // Set flag to indicate complete message is ready for processing
            data_controll.Data_received = true; 
            data_controll.Flow_control = Flow_None; // Reset flow control state for next message
          }
          rxIndex = 0; // Reset buffer index for next key-value pair or message
        } else if (rxIndex < RX_BUFFER_SIZE - 1) {
          rxBuffer[rxIndex++] = c; // Store character in buffer and increment index 
        } else {
          data_controll.Flow_control = Flow_Error; // Set error state for buffer overflow
          data_controll.Data_error = Data_Parse_Error; // Set data error for parse error due to buffer overflow
          return; // Exit function on error
        }
        // This state can be used if you want to implement additional processing while receiving data
        // For now, we will just ignore incoming data until processing is done
        break;

      default:
        data_controll.Flow_control = Flow_Error; // Set error state for unexpected flow control value
        return;
    }
  }
   

}// End of poll function


/****************************************************************************************/

void OpenRemiseProtokoll::process()
{
  //process  will only check the timout an errors an set valid  flags


  if (!data_controll.Data_received) return;
  
  data_controll. Data_received= false;
  data_controll.lastDataMillis = millis();
  if (data_controll.Data_error != Data_Timeout) {
  data_controll.Data_error = Data_Valid;
  }

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

    //end of Protokoll/protokoll2.Cpp


    #endif