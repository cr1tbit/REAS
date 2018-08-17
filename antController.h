#include <aREST.h>


#define MAX_ANT_NO 16

#if defined(__AVR_ATmega328P__)
    #define AREAS_NANO
#elif defined (ESP8266)
    #define AREAS_ESP
#endif


class AntController {
    static const int antCount = MAX_ANT_NO;

    bool ant[antCount];
    bool _enableFlag;

public:

    AntController(){
        setAllOff();
    }

    void setAllOff(){
        for (int i=0;i<antCount;i++){
            ant[i] = false;
        }
        setOutput();
    }

    int setExclusive(int antNo){
        if((antNo>0)&&antNo<antCount){
            setAllOff();
            ant[antNo] = true;
            setOutput();
            return 0;
        }
        else{
            return -1;
        }
    }

    int setExclusiveFromString(String param){
        int antNo = param.toInt();
        return setExclusive(antNo);
    }

    int setSingle(int antNo){
        if((antNo>0)&&antNo<antCount){
            ant[antNo] = true;
            setOutput();
            return 0;
        }
        else{
            return -1;
        }
    }
    
    int unsetSingle(int antNo){
        if((antNo>0)&&antNo<antCount){
            ant[antNo] = false;
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

    int setOutput(){
        Serial.print("setting ant values:");
        for(int i=0;i<antCount;i++){
            Serial.print(ant[i]);
            Serial.print(" ");
        }
        Serial.println(" EOL");
        return 0;
    }
};

