#include <UIPEthernet.h>
#include <aREAS_hal.h>

#define MAX_ANT_NO 16

#if defined(__AVR_ATmega328P__)
    #define AREAS_NANO
#elif defined (ESP8266)
    #define AREAS_ESP
#endif

namespace aREAS {

    typedef struct{
        bool ant[MAX_ANT_NO];
    } Ants;



    void turnAllOff(Ants ants){
        for (int i=0;i<MAX_ANT_NO; i++){
            ants.ant[i] = false;
        }
    }


    void setOneAnt(int antNo){
        return;
    }

    void setMulAnt(Ants ants){
        return;
    }

    int stringToAntNo(String str){
        int antNo = str.toInt();
        if ((antNo >0) && (antNo <= 16)){
            return antNo;
        }
        return -1;
    }
    
    int stringToMulAnt(Ants* ants,String params){
        for (int i = 0; i< params.length(); i++){
            if (params.charAt(i) == 1)
                ants->ant[i] = true;
            else
                ants->ant[i] = false;
        }
        return 0;
    }
}