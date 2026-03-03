#include "main.h"

/* SH1107 128x128, I2C, Hardware I2C, Page-Mode */
OpenRemiseDisplay display;
// buttons for up and down
Buttons buttons;
// Protokoll instance
OpenRemiseProtokoll protokoll; // Protokoll  kommunikation über HW Uart
// software serial for debug output
#ifdef SERIAL_DEBUG
SoftwareSerial Debug_port(PROTOKOLL_RX_PIN, PROTOKOLL_TX_PIN);
#endif
/// @brief Setup function for initialization

void setup()
{
  SYS.State = MODE_INIT;
  SYS.Error_Code = ERROR_NONE; // Set error code to none initially
  SYS.time_stamp = millis();
  SYS.time_interval = WAIT_TIME_STARTTIME; // Set time interval for error display or other timed events
  SYS.Last_Run = millis();  // Initialize last run time to current time

// Schnittstellen initialisieren
#ifdef SERIAL_DEBUG
  Debug_port.begin(9600); // Debug port mit 9600 Baud software serial
  Debug_port.println(F("OpenRemise Display Debugport activ"));
#endif
  // Display initialisieren
#ifdef SERIAL_DEBUG
  Debug_port.println(F("SH1107 Init Test"));
#endif
  // Display initialisieren
  uint8_t ret = display.begin();
  if (ret != 0x00)
  {
#ifdef SERIAL_DEBUG
    Debug_port.println(F("Display init failed!"));
#endif
    SYS.State = MODE_ERROR;
    SYS.Error_Code = ERROR_DISPLAY;
  }
    // Buttons initialisieren
  buttons.begin(BUTTON_A_PIN, LED_A_PIN, BUTTON_B_PIN, LED_B_PIN); // Button and LED pins initialisieren
  buttons.SetLED(BUTTON_A, true);                                  // LED A an für Initialisierung#
  buttons.SetLED(BUTTON_B, true);                                 // LED B an für Initialisierung

    // Display initialisiert, Buttons initilisiert jetzt LEDs entsprechend setzen
  if (ret == 0x00)
  {
    // if  display is ok
    // Show welcome screen
    display.Set_Current_Screen(SCREEN_WELCOME);
    display.force_update(); // Force update to show welcome screen immediately
    // Set LEDs based on current screen
    //  not blink with button LEDS
   
      display.update();
  }
  else
  {
    // if display init failed, blink both LEDs to indicate error
    while (1)
    {
      buttons.SetLED(BUTTON_A, true);
      buttons.SetLED(BUTTON_B, false);
      delay(500);
      buttons.SetLED(BUTTON_A, false);
      buttons.SetLED(BUTTON_B, true);
      delay(500);
    }

  }
 
  Serial.begin(115200); // Protokoll kommunikation über HW Uart mit 115200 Baud
  // Protokoll initialisieren
  protokoll.begin(Serial); // Protokoll mit HW Serial als Transport initialisieren
                     

  /* todo  rework */
  /* first there will be debug data on the RX
   *  wait  for the  first komplett Json data  set  in betrween {}
   */
  // Wait  for S3 sending json data on HW Uart mit 115200 Baud

  // wait for data for 5 sec
 
  /*todo real implementation should wait for specific data or event, here we just wait for 5 seconds to simulate waiting for data */
  while (millis() < SYS.time_stamp + SYS.time_interval)
   {
    // Here we could also check for specific data or events to proceed, for now we just wait for the time interval to pass
    // For example, we could check if protokoll has received specific data or if buttons have been pressed to proceed with initialization
    // This is just a placeholder implementation to simulate waiting for data during initialization
    protokoll.update();

    if(protokoll.Get_Protokoll_Error() == Data_Valid)
    { 
    display.Set_Current_content( protokoll.getData(IDX_ID),SW_Version,"","","")  ;
      display.force_update();
      display.update();


    }
    

  }


   SYS.actuallscreen = display.Get_Current_Screen(); // Get current screen to provide the right content and set LEDs accordingly
   display.Set_Current_Screen(SCREEN_NETWORK);
   display.force_update(); // Force update to show network screen immediately
   // Initend, now main loop will take over and react on events and update display accordingly
  // setruntime interval for theloop
  SYS.time_interval = DISPLAY_UPDATE_INTERVAL; // Set time interval for loop iteration (e.g., 10 ms)
  Set_LEDS(SYS.actuallscreen); // Set LEDs based on current screen
}

/// @brief
// Main loop only get all information of the actuall screen and providet them to the adisplay
// important is that only the information for the actual screen will be
// taken from Protokoll modul.
// The buttons will be turned off after 5sec of inactivity ( no Button pushed, no event occured)
void loop()
{ 
  //SYS.sleep_triggered = false; // Flag to track if sleep mode has been triggered
  // Main loop
  SYS.time_stamp = millis(); // Update current time stamp at the beginning of the loop

  // get  the input from enviroment
  //  update Protokoll module with new data from HW Uart
  protokoll.update();
  // Ask  buttons
  buttons.update(); // return also button state !!!
                    // figure out  the system reaktions
  // Change  Screen  depending on button input and system state
 
  Set_next_Screen(display.Get_Current_Screen(), buttons.read()); // todo  implement this function to change screen based on button input and current screen
  Set_LEDS(display.Get_Current_Screen());                        // set LED status based on current screen and sleep mode
  Set_Content(display.Get_Current_Screen());                      // set content for the current screen based on data from Protokoll module and current screen

  // Check for inactivity and trigger sleep mode if needed
  if (buttons.read() != NO_BUTTONS)
  {
    SYS.Last_Run = millis();     // Update last run time
    SYS.sleep_triggered = false; // Reset sleep mode flag on activity
  }
  if (millis() - SYS.Last_Run >= SLEEP_TIMEOUT)
  {                             // Check for inactivity (10 seconds)
    SYS.sleep_triggered = true; // Set sleep mode flag
    SYS.State = MODE_SLEEP;     // Update system state to sleep
    // Additional sleep mode logic as needed
  }

  // Collect error codes from different modules and update system error code
  //Get_all_error_code(); // This function can be used to collect and return all error codes from different modules and update the system error code accordingly
  // Show all Errors in 5  sec cykle

  if (SYS.Error_Code != ERROR_NONE)
  {
    if (millis() - SYS.Last_Run > 5000)
    {                                         // If 5 seconds have passed since error occurred
      SYS.Error_Code = ERROR_NONE;            // Clear error code after displaying for 5 seconds only  for testing
      display.set_error_code(SYS.Error_Code); // Update display to clear error message
      display.Set_Current_Screen(SCREEN_MAIN);
      display.force_update(); // Force update to show main screen immediately after clearing error}
    }
      else
    {
      SYS.sleep_triggered = false; // wakup on error
      display.Set_Current_Screen(SCREEN_ERROR);
      display.force_update();                               // Force update to show error screen immediately
      display.set_error_code(SYS.Error_Code, "KOM ERROR!"); // Set error code and message for display
    }

    // reste error code after displaying
  }

  // Reakt on system state changes

  // Update the display content
  // if Screen has changed or if content has changed or if we are in error mode
  // and error code has changed or if we are in sleep mode and sleep mode is triggered


  if (display.Get_Current_Screen() != SYS.actuallscreen ) // neuer Screen 
  { 
     display.force_update(); // Force update to show new screen immediately
     
     SYS.time_stamp = millis(); // Update last run time when display is updated
 // Update display if content has changed or if we are in error mode or if sleep mode is triggered
    
    // reset update  conditions
    protokoll.reset_Content_Changed(); // Reset content changed flag after updating display
    SYS.actuallscreen = display.Get_Current_Screen(); // Update current screen variable after updating display  
  }
  else
  { // New Data received 
     if ((protokoll.Get_Content_Changed() ==true)
    &&( protokoll.Get_Protokoll_Error() == Data_Valid)) // Wait for the  complet Stream 
 {  display.force_update(); // Force update to show new screen immediately
    SYS.time_stamp = millis(); // Update last run time when display is updated
    protokoll.reset_Content_Changed();
  }

  }
    
  
  display.update(); // Update display (e.g., refresh screen, handle animations, etc.)
}

// mamas  little helpers 
// todo  implement this function to change screen based on button input and current screen

void Set_next_Screen(ScreenId id, uint8_t button_value)
{
 uint8_t       index=0; 


  if (button_value >= RESET_REQUEST)
  {
    // both buttons held for reset request
    display.Set_Current_Screen(SCREEN_ERROR);
    SYS.State = MODE_RESET;
    SYS.Error_Code = ERROR_NONE; // Clear error code on reset
   return;
  }
    index=id ;
    if ((button_value & mask_Button_B) == BUTTON_B_PRESSED)
    {
      index=index+1;

    }
     if ((button_value & mask_Button_A) == BUTTON_A_PRESSED)
    {
       index=index-1; 
    }

  // if we flash we will show the Learn screen
  // Dominant condition
 if (strcmp(protokoll.getData(IDX_STATUS),"OTA") == 0)
  {
              index = SCREEN_LEARN;
            }

 //we have to take care that the index is always between 0 and NUM_SCREENS-1
    if (index >= NUM_MENUE_SCREENS)
    {
      index = SCREEN_WELCOME+1; // Wrap around to the first screen left out welcome screen because it is only shown at the beginning and not in the normal loop

    }
    else if (index < SCREEN_WELCOME+1 )
    {
      index = NUM_MENUE_SCREENS - 1; // Wrap around to the last screen
    }   

 display.Set_Current_Screen(index);
// Add more cases as needed for other screens and button interactions
}
/*************************************************************************************/
/// @brief Set LED status based on current screen index and sleep mode

void Set_LEDS(ScreenId idx)
{ // Set LED status based on current screen index and sleep mode
  if (SYS.sleep_triggered == true)
  {
    buttons.SetLED(BUTTON_A, LED_OFF);
    buttons.SetLED(BUTTON_B, LED_OFF);
  }
  else
  {// Set LED status based on current screen index
    switch (idx)
    {
    case SCREEN_WELCOME+1:
      buttons.SetLED(BUTTON_A, LED_OFF);
      buttons.SetLED(BUTTON_B, LED_ON);
      break;
  case     NUM_MENUE_SCREENS-1: // Number of screens in the menu, used for cycling through screens
      buttons.SetLED(BUTTON_A, LED_ON);
      buttons.SetLED(BUTTON_B, LED_OFF);
      break;
    default:
      buttons.SetLED(BUTTON_A, LED_ON);
      buttons.SetLED(BUTTON_B, LED_ON);
      break;
    }
  }
}
/*************************************************************************************/

/// @brief Sets the content for a specific screen index
/// @param idx The screen index to set content for
void Set_Content(ScreenId idx)
{ 
  
  switch (idx)
  {
  case SCREEN_MAIN:
    // Track daten
    // Voltage  format should be <XX.XX V> same for current <XX.XX A> and status <status code> and service <service code>
    //SCREEN_MAIN, false,"Track ","Voltage : ","Current :","Status :","Mode :"
    display.Set_Current_content(protokoll.getData(IDX_TRK_VOLTAGE),protokoll.getData(IDX_TRK_CURRENT), protokoll.getData(IDX_STATUS),  protokoll.getData(IDX_NONE), "  ");
    break;
  case SCREEN_NETWORK:
    // network daten  " IP ","mDNS ","SSID ","RSSI "
    display.Set_Current_content(protokoll.getData(IDX_IP), protokoll.getData(IDX_JSON_MDNS), protokoll.getData(IDX_WIFI_SSID), protokoll.getData(IDX_WIFI_PWR), "  ");
    break;
  case SCREEN_LEARN:
    //  Not implemented yet
    //Show OTA 

    break;
  case SCREEN_UNI:
    // free screen  with free text
        display.Set_Current_content( protokoll.getData(IDX_ID), "test 2", "test 3", "test 4", "test 5");
    break;

  default:

    break;
  }
}
/***************************************************************************************/
void Get_all_error_code()
{
  // This function can be used to collect and return all error codes from different modules
// fehler der einzelene module sammeln und in SYS.Error_Code setzen

  SYS.Error_Code = ERROR_NONE; // Reset error code before collecting
  /*  DISPLAY macht wenig  Sinn weil es ja dan ach nicht angezeit werden kann  aber hier könnte man
   prüfen ob der Aktuelle screen gültig wäre */
  if (display.Get_Current_Screen() == SCREEN_INVALID)
  {
    SYS.Error_Code = ERROR_DISPLAY;
  }

  if (protokoll.Get_Protokoll_Error() != Data_Valid)
  {
    SYS.Error_Code = ERROR_PROTOKOLL;
  }
 // buttons haben keinen Fehlercode
 // Fehler die  von der S3  gemeldet werden (json daten) 
  //Todo:  je nach dem wie die Fehler von der S3  gemeldet werden

  return;
  // Return or use the combined error code as needed
}