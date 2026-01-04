#ifndef DISPLAY_H
#define DISPLAY_H

// Define the OpenRemiseDisplay class
class OpenRemiseDisplay {
public:
    void begin();
    void update();
    void clear();
 
private:
   
    bool initialized_display;
}; 
#endif // DISPLAY