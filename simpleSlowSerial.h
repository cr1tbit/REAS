

#ifndef AREAS_SSSERIAL
#define AREAS_SSSERIAL

#define START_STOP_BIT_1DOT5 //enable 1.5 start and stop bit



class SSSerial;

SSSerial* _s = nullptr;

class SSSerial{
    uint8_t outputPin;
    static const int queueSize = 1; 

    char queue[queueSize];

    uint8_t bitPointer;
    uint16_t timeCounter;
    uint16_t timePeriod;

    bool beginFlag;

    bool startBitFlag, endBitFlag;

    void clearQueue(){
            for(int i=0;i<queueSize;i++){
                queue[i] = '\0';
            }
            bitPointer = 0b10000000;
            startBitFlag = false;
            endBitFlag = false;
        }

        void shiftQueue(){
            for(int i=0;i<queueSize-1;i++){
                queue[i] = queue[i+1];
            }
            queue[queueSize] = '\0';
        }
    
    

public:

    SSSerial(uint8_t pinName){
        beginFlag = false;
        outputPin = pinName;
        pinMode(outputPin,OUTPUT);
        //configure TIMER0, which is already
        //used for milis(), to launch interrupt
        OCR0A = 0xAF;
        TIMSK0 |= _BV(OCIE0A);

        /** Careful - quite hacky part of code:
         * -Timer interrupt may be only attached
         * to a normal function, or static method,
         * but not really to a normal method of a
         * class 
         * This is because a pointer to a non-static
         * method is a special one - it must contain
         * its object's pointer too. Long story short:
         * void(*p)() is NOT compatible with
         * void(className::*p)()
         * -It is worth saying that SIGNAL() function
         * is a special macro that cannot be put inside
         * the method.
         * -One way to resolve this problem is used in
         * TimerOne library - a single instance of 
         * timer1 is created inside the timerOne.h file.
         * This is not very elegant, as every programmer
         * is used to creating this kind of objects in
         * their code. This approach might be confusing.
         * -The other way is not to attach a method
         * pointer to the interrupt routine, but pointer
         * to a specific instance of an object - in
         * this approach you still cannot create multiple
         * SSSerial objects, but a signle one can be 
         * created with ease during the runtime. 
         * -If ever the need for multiple SSSerial 
         * objects may arise, it might be just possible,
         * however surely quite harder. **/
        if(_s == nullptr){
            _s = this;
        }
        else{
            Serial.println(
                "WARNING - second instance of SSSerial is being created!\n"
                "THIS SHOULD NEVER HAPPEN!!");
        }
    }

    void begin(uint16_t milis){
        beginFlag = true;
        clearQueue();
        timePeriod = milis;
        timeCounter = 0;
    }

    void send(char c){
        if(beginFlag){
            clearQueue();
            queue[0] = c;
        }
    }

    /** sending short string not implemented yet. **/
    /*
    void send(char* pc, uint8_t len){
        if(beginFlag){
            clearQueue();
            if (len>queueSize) len = queueSize;
            memcpy(queue,pc,len*sizeof(&pc));
        }
    }
    */

    /** this function will be called every 1ms **/
    void _interrupt(){
        if(timeCounter >0){
            timeCounter--;
        }
        else{
            timeCounter = timePeriod;
            //if char isn't empty
            if(queue[0] != '\0'){
                //if start bit wasn't sent yet
                if (startBitFlag == false){
                    startBitFlag = true;
                    #ifdef START_STOP_BIT_1DOT5
                    timeCounter += (timeCounter>>1);
                    #endif
                    digitalWrite(outputPin, 1);
                }
                else {
                    //set output by bit
                    digitalWrite(outputPin,
                        queue[0] & bitPointer);
                    //shift bitMask
                    bitPointer >>= 1;
                    //when bit pointer is shifted to '0'
                    if (bitPointer == 0x00){
                        //reset bitPointer
                        bitPointer = 0b10000000;
                        //load new char to queue
                        shiftQueue();
                    }
                } 
            }
            else{
                if(endBitFlag == false){
                    endBitFlag = true;
                    #ifdef START_STOP_BIT_1DOT5
                    timeCounter += (timeCounter>>1);
                    #endif
                    digitalWrite(outputPin, 1);
                }
                else{
                    digitalWrite(outputPin,0);
                }
            }
        }
    }
};

SIGNAL(TIMER0_COMPA_vect){
    if(_s != nullptr){
        _s->_interrupt();
    }
}

#endif //AREAS_SSSERIAL