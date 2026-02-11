// Protokoll handler implementation file
#include "protokoll.h"

/// @brief  Initialize the Protokoll module
/// @return     0 on success, error code otherwise

/// @brief 
OpenRemiseProtokoll::OpenRemiseProtokoll()
  : _transport(nullptr),
    rxIndex(0)
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
 
    data_controll.Data_send =false;
    data_controll.Data_received= false;  //s3 daten empangen 
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

Protokoll_Data_error_t OpenRemiseProtokoll::Get_Protokoll_Error (){
  return data_controll.Data_error;
};

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


void OpenRemiseProtokoll::process()
{
  if (!data_controll.Data_received) return;
  data_controll. Data_received= false;

  DeserializationError err = deserializeJson(doc, rxBuffer);
  if (err) {
    data_controll.Data_error = Data_Parse_Error;
    return;
  }

  // Nur Felder lesen, die du brauchst
  if (doc.containsKey(JSON_STATUS)) {
    strlcpy(Data_from_Stream.state,
            doc[JSON_STATUS],
            sizeof(Data_from_Stream.state));
  }

  if (doc.containsKey(JSON_IP)) {
    strlcpy(Data_from_Stream.ip,
            doc[JSON_IP],
            sizeof(Data_from_Stream.ip));
  }

  if (doc.containsKey(JSON_WIFI_SSID)) {
      strlcpy(Data_from_Stream.ssid,
            doc[JSON_WIFI_SSID],
            sizeof(Data_from_Stream.ssid));
  }
  if (doc.containsKey(JSON_WIFI_PWR)) {
      strlcpy(Data_from_Stream.rssi,
            doc[JSON_WIFI_PWR],
            sizeof(Data_from_Stream.rssi));
  }

  data_controll.lastDataMillis = millis();
  data_controll.Data_error = Data_Valid;
}

const Data_from_Stream_t& OpenRemiseProtokoll::getDatafromStream() const {
 return Data_from_Stream;
}



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
