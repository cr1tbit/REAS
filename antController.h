#include <simpleSlowSerial.h>

#ifndef AREAS_ANT_CTRL
#define AREAS_ANT_CTRL


#define MAX_ANT_NO 16

#if defined(__AVR_ATmega328P__)
    #define AREAS_NANO
#elif defined (ESP8266)
    #define AREAS_ESP
#endif

#define AREAS_OUTPUT_DEBUG //every output change is logged to serial output
#define AREAS_OUTPUT_UART50HZ //output is sent via slow UART port

class AntController {
    static const int antCount = MAX_ANT_NO;

    bool ant[antCount];
    bool _enableFlag;
    SSSerial serial50hz;
public:

    AntController():serial50hz(3){
        setAllOff();
        serial50hz.begin(50);
    }

    void clearAnt(){
    for (int i=0;i<antCount;i++){
        ant[i] = false;
    }
    }

    void setAllOff(){
        clearAnt();    
        setOutput();
    }

    int setExclusive(int antNo){
        if(antNo == 0){
            clearAnt();
            setOutput();
            return 0;
        }
        if((antNo>0)&&antNo<=antCount){
            clearAnt();
            ant[antNo-1] = true;
            setOutput();
            return 0;
        }
        //ant = 0 or sth weird - ignore
        else{
            return -1;
        }
    }

    int setExclusiveFromString(String param){
        int antNo = param.toInt();
        return setExclusive(antNo);
    }

    int setSingle(int antNo){
        if(antNo == 0){
            setOutput();
            return 0;
        }
        else if((antNo>0)&&antNo<antCount){
            ant[antNo-1] = true;
            setOutput();
            return 0;
        }
        else{
            return -1;
        }
    }
    
    int unsetSingle(int antNo){
        if((antNo>0)&&antNo<antCount){
            ant[antNo-1] = false;
            setOutput();
            return 0;
        }
        else{
            return -1;
        }
    }

    int setMultiFromBoolArray(bool* arrayPtr){
        memcpy(ant, arrayPtr, antCount*sizeof(bool));
        setOutput();
    }

    int setMultiFtomString(String params){
        int paramCount = params.length();
        if (paramCount > antCount)
            paramCount = antCount;
        for (int i = 0; i< paramCount; i++){
            if (params.charAt(i) == '1')
                ant[i] = true;
            else
                ant[i] = false;
        }
        setOutput();
        return 0;
    }

    char antToChar(){
        uint8_t bitmask = 0b00000001;
        uint8_t c = 0;
        for(int i=0;i<8;i++){
            if(ant[i]){
                c += bitmask;
            }
            bitmask <<=1;
        }
        if(c == '\0'){
            return (char)0xFF;
        }
        else{
            return (char)c;
        }
    }

    int setOutput(){
        
        uint8_t bitOut = 0;
        uint8_t bitMask = 1;
        for(int i=0;i<8;i++){
            if(ant[i]){
                bitOut |= bitMask;
            }
            bitMask >>= 1;
        }
        #ifdef AREAS_OUTPUT_UART50HZ
            serial50hz.send(antToChar());
        #endif //AREAS_OUTPUT_UART50HZ

        #ifdef AREAS_OUTPUT_DEBUG
            Serial.print("setting ant values:");
            for(int i=0;i<antCount;i++){
                Serial.print(ant[i]);
                Serial.print(" ");
            }
            Serial.println("EOL");
         #endif //AREAS_OUTPUT_DEBUG
        return 0;
    }

   
};

#endif //AREAS_ANT_CTRL