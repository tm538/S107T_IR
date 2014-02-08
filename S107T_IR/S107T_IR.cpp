#include "S107T_IR.h"
#include "mbed.h"

S107T_IR::S107T_IR(PinName pin) : _pin(pin) {  // _pin(pin) means pass pin to the DigitalOut constructor
    _pin = 0;                                        // default the output to 0
}

void S107T_IR::sendCommand(int yaw, int pitch, int throttle, int channel) {
    
    int binvals[8] = {1,2,4,8,16,32,64,128};
 
    sendHeader();
    
    /** Yaw byte 0
    *  0–62 is a right turn, 63 is centre (default) value, 64–127 is a left turn
    *  first bit 0
    **/
    
    sendZero();
    
    for(int i = 6; i >= 0; i--) {
        if(yaw >= binvals[i] ) {
            yaw -= binvals[i]; 
            sendOne();
        } else {
            sendZero();   
        }  
    }
    
    /** Pitch byte 1
    *  0–62 is pitch forward, 63 is centre (default) value, 64–127 is pitch backwards
    *  first bit 0
    **/
    
    sendZero();
    
    for(int i = 6; i >= 0; i--) {
        if(pitch >= binvals[i] ) {
            pitch -= binvals[i]; 
            sendOne();
        } else {
            sendZero();   
        }  
    }
    
    /** Channel **/ 
    
    sendOne();
    
    /** Throttle
    *  Byte 2 (throttle) has a range of 0–127 
    **/
    
    for(int i = 6; i >= 0; i--) {
        if(throttle >= binvals[i] ) {
            throttle -= binvals[i]; 
            sendOne();
        } else {
            sendZero();   
        }  
    }
    
    sendFooter();
}

void S107T_IR::tx_us(int us) {

    t.start();
    _pin = 0;
    
    while ( t.read_us() < us ) {
    
        _pin = 1;
        wait_us(13);
        _pin = 0;
        wait_us(13);
    }
    
    t.stop();
    t.reset();
    _pin = 0;
}

void S107T_IR::tx_ms(int ms) {        
    tx_us(1000 * ms);    
}

/** header is 2ms high then 2ms low then high again for 380 us **/
void S107T_IR::sendHeader() {    
    tx_ms(2);
    wait_ms(2);
}

/** footer is 300us high then >2ms low **/
void S107T_IR::sendFooter() {    
    tx_us(300);
    wait_us(2005);    
}

void S107T_IR::sendOne() {
    tx_us(300);
    wait_us(700);
}

void S107T_IR::sendZero() {    
    tx_us(300);
    wait_us(300);
}