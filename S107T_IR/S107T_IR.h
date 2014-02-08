#ifndef MBED_S107T_IR_H
#define MBED_S107T_IR_H

#include "mbed.h"

class S107T_IR {
public:
    S107T_IR(PinName pin);
    void sendCommand(int yaw, int pitch, int throttle, int channel);

private:
    DigitalOut _pin;
    Timer t;
    
    void tx_us(int us);    
    void tx_ms(int ms); 
    void sendHeader();   
    void sendFooter();
    void sendOne();
    void sendZero();
};

#endif