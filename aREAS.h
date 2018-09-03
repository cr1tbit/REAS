#include <UIPEthernet.h>
#include <antController.h>

#ifndef AREAS_LIB
#define AREAS_LIB

#define AREAS_MAX_PAR_LEN 3
#define AREAS_MAX_FUN_NO 10

/*
//int randomSeed;//for random MAC generation
//TODO: ranomizing MAC address
char mac[] = {
    0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
//TODO: setting up custom IP address
IPAddress ip(192, 168, 2, 200);

EthernetServer server(80);

AntController antController;

//TODO: DHCP functionality
*/

int freeRam () {
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}

class aREAS_Handler{

    typedef struct{
        String name;
        int (*fPtr)(String);
    } simplefunMap;
    
    simplefunMap fMap[AREAS_MAX_FUN_NO];
    public:    
    AntController antController;

    aREAS_Handler(uint8_t antennaCount):antController(antennaCount){
        clearFunMap();
    }

    int runFunctionFromMap(String name, String param){
        for(int i=0;i<AREAS_MAX_FUN_NO; i++){
            if (fMap[i].fPtr != nullptr){
                if(fMap[i].name == name){
                    return fMap[i].fPtr(param);
                }
            }
        }
        return -1;
    }

    void clearFunMap(){
        for(int i=0;i<AREAS_MAX_FUN_NO; i++){
            fMap[i].fPtr = nullptr;
        }
    }

    int attachCallback(String name, int (*callback)(String)){
        name = name.substring(0,AREAS_MAX_PAR_LEN);
        //find empty callback slot
        for (int i = 0;i<AREAS_MAX_FUN_NO; i++){
            if(fMap[i].fPtr == nullptr){
                //attach stuff
                fMap[i].name = name;
                fMap[i].fPtr = callback;
                return 0;
            }
        }
        int test = 1;
        Serial.println(F("Warning - no emplty callback slots!"));
        return -1;
    }

    /*
    int kbwTrace(const char *fmt, ...)
    {
        va_list args;
        va_start(args, fmt);
        return printf(fmt, args);
    }*/


    void writeResponse(EthernetClient* client, const char *response, boolean isValid = 1){
        client->print(F("HTTP/1.1 "));
        if (isValid){
            client->println(F("200 OK"));
        }
        else{
            client->println(F("404 NOT FOUND"));
        }
        client->println(F("Content-Type: text/plain"));
        /** allow internal network javascript access **/
        client->println(F("Access-Control-Allow-Origin: *"));
        client->println();
        /** Begin data return **/
        client->println(response);
    }

    void handleClient(EthernetClient* client){
        Serial.println(F("new client\n\n"));
        // an http request ends with a blank line
        boolean currentLineIsBlank = true;
        String param = "";
        bool isRequest = false;

        int charCounter = 0;
        const int charCounterMax = 64;//max request length

        while (client->connected()) {
            if (client->available()) {
                char c = client->read();
                if (charCounter++ > charCounterMax){
                    String errMsg = "Request too long (over ";
                    errMsg += charCounterMax;
                    errMsg += "characters)";
                    writeResponse(client,errMsg.c_str(),false);
                    break;
                }
                Serial.write(c);

                // if you've gotten to the end of the line (received a newline
                // character) and the line is blank, the http request has ended,
                // so you can send a reply
                if ((c == ' ')&&(isRequest)) {
                    // send a standard http response header        
                    client->println(param);
                    if(handleParam(param)==0){
                        writeResponse(client,"param OK");
                    }
                    else{
                        writeResponse(client,"invalid param!",false);
                    }
                    break;
                }
                
                if(isRequest){
                    param+=c;
                }

                /** start parsing parameter after "?" is found. **/
                if(c == '?'){
                    isRequest = true;
                }

                /** detecting endOfLine ('\n') marks the end of interesting stuff in request. **/
                if(c=='\n'){
                    writeResponse(client,
                                "Request not valid (no '?' found)\n"
                                "valid syntax: <IPADDR>/?fun=parameters",
                                false);
                    break;
                }
            }
        }
        Serial.println(F("free ram: "));
        Serial.print(freeRam());
        Serial.println(F(" bytes. EOT"));
        client->stop();
    }

    int handleParam(String param){
        //valid syntax of an param:
        //  param:|X|Y|Z|=|v|a|l|u|e|...
        //char no:|0|1|2|3|4|5|6|7|8|...
        const int maxParLen = 32;
        const int parNameLen = AREAS_MAX_PAR_LEN;

        //get "XYZ"
        String parName = param.substring(0,parNameLen);
        
        Serial.print(F("param name:"));
        Serial.println(parName);
        //check if under 3rd char is "="
        /*if(param.charAt(parNameLen) != '='){
            Serial.println(F("'equals' not on valid pos"));
            return -1;
        }*/
        //get chars from 4...maxParLen 
        //(or less, .substring() handles less than max chars)
        String parVal = param.substring(4,maxParLen);
        Serial.print(F("param val: "));
        Serial.println(parVal);

        //try methods embedded in aREAS object:
        if (parName=="sta"){
            Serial.println(F("status OK."));
            return 0;
        }

        if (parName=="ant"){
            Serial.println(F("ant called..."));
            return antController.setExclusiveFromString(parVal);
        }
            
        if (parName=="mul"){
            Serial.println(F("mul called..."));
            return antController.setMultiFtomString(parVal);
        }
        //try to find function in function pointer array
        return runFunctionFromMap(parName,parVal);
    }
};

#endif //AREAS_LIB