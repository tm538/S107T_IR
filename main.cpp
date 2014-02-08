#include "S107T_IR.h"
#include "rtos.h"
#include "mbed.h"
#include "MODSERIAL.h"

S107T_IR IR1(p5);
DigitalOut led1(LED1);
DigitalOut led2(LED2);
 
MODSERIAL pc(USBTX, USBRX); // tx, rx

char buf[100];

volatile int yaw; 
volatile int pitch;
volatile int throttle; 
volatile int channel;

volatile bool newline_detected = false;

void rxCallback(MODSERIAL_IRQ_INFO *q);
void commandThread(void const *args);
void parse();

int main() {
    pc.baud(9600);
    //pc.printf("throttle test 2!\n");
    pc.attach(&rxCallback, MODSERIAL::RxIrq);
    
    Thread thread(commandThread);
    thread.set_priority(osPriorityHigh);
    
    yaw = 63; // no yaw
    pitch = 63; //no pitch
    throttle = 0; //full throttle
    channel = 1;
    
    while(1) {
        
        //IR1.sendCommand(yaw, pitch, throttle, channel);
        //wait_ms(115);
                
        if(newline_detected) {
            parse();
            newline_detected = false;
        }
    }
}

void parse() {
    
    char i[3];
    i[0] = buf[0];
    i[1] = buf[1];
    i[2] = buf[2];
    
    throttle = atoi(i);
    
    i[0] = buf[4];
    i[1] = buf[5];
    i[2] = buf[6];
    
    pitch = atoi(i);
    
    i[0] = buf[8];
    i[1] = buf[9];
    i[2] = buf[10];
    
    yaw = atoi(i);
    
    pc.printf("Throttle: %d, Pitch: %d, Yaw: %d !\n", throttle, pitch, yaw);
}

void commandThread(void const *args) {
    while(1) {
        if(throttle > 0) {
            IR1.sendCommand(yaw, pitch, throttle, channel);
        }    
        /**pc.printf("thr = <%d>\n", throttle);
        pc.printf("yaw = <%d>\n", yaw);
        pc.printf("pitch = <%d>\n", pitch);       
        pc.printf("channel = <%d>\r\n", channel);**/
        Thread::wait(115);
    }
}

void rxCallback(MODSERIAL_IRQ_INFO *q) {
    MODSERIAL *serial = q->serial;
    if ( serial->rxGetLastChar() == '\r') {
        
        int nNum = pc.move(buf, 100, '\r');
        buf[nNum] = '\0';
        newline_detected = true;       
    }
}