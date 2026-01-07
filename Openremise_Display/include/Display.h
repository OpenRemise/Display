#ifndef DISPLAY_H
#define DISPLAY_H

// Define the OpenRemiseDisplay class
class OpenRemiseDisplay {

    public:
    uint8_t begin();
    uint8_t update();
    uint8_t clear();

    void ShowError();
    void ShowWelcome();
    void ShowLearnMode();
    void ShowNormalMode(); 
      
private:
   
    bool initialized_display;
}; 
#endif // DISPLAY