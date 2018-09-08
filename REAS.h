#include <UIPEthernet.h>
#include <antController.h>

#ifndef REAS_LIB
#define REAS_LIB

#define REAS_MAX_PAR_LEN 3
#define REAS_MAX_FUN_NO 10

/** this function comes from arduino help site **/
int freeRam () {
  extern int __heap_start, *__brkval; 
  String v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}

/** Main object used to handle client requests **/
class REAS_Handler{

    typedef struct{
        String name;
        String (*fPtr)(String);
    } simplefunMap;
    
    simplefunMap fMap[REAS_MAX_FUN_NO];
    public:    
    AntController antController;

    REAS_Handler(uint8_t antennaCount):antController(antennaCount){
        clearFunMap();
    }

    String runFunctionFromMap(String name, String param){
        for(int i=0;i<REAS_MAX_FUN_NO; i++){
            if (fMap[i].fPtr != nullptr){
                if(fMap[i].name == name){
                    return "200 "+ fMap[i].fPtr(param);
                }
            }
        }
        return "404 Function " + name + "not found.";
    }

    void clearFunMap(){
        for(int i=0;i<REAS_MAX_FUN_NO; i++){
            fMap[i].fPtr = nullptr;
        }
    }

    int attachCallback(String name, String (*callback)(String)){
        name = name.substring(0,REAS_MAX_PAR_LEN);
        //find empty callback slot
        for (int i = 0;i<REAS_MAX_FUN_NO; i++){
            if(fMap[i].fPtr == nullptr){
                //attach stuff
                fMap[i].name = name;
                fMap[i].fPtr = callback;
                return 0;
            }
        }
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


    void writeResponse(EthernetClient* client, String response, boolean isValid = 1){
        /** check if first 3 chars from response
         * string contain an status number **/ 

        int16_t statusNumber = response.substring(0,3).toInt();
        if (statusNumber != 0){
            if (statusNumber != 200 ){
                //probably 404
                isValid = false;
            }
        }
        
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
        Serial.println(F("New client:"));
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
                    writeResponse(client,errMsg,false);
                    break;
                }
                Serial.write(c);

                // if you've gotten to the end of the line (received a newline
                // character) and the line is blank, the http request has ended,
                // so you can send a reply
                if ((c == ' ')&&(isRequest)) {
                    // send a standard http response header        
                    
                    writeResponse(
                        client,
                        handleParam(param)
                    );
                    client->println("request parameter: ");
                    client->println(param);

                    /*
                    if(handleParam(param)==0){
                        writeResponse(client,"param OK");
                    }
                    else{
                        writeResponse(client,"invalid param!",false);
                    }
                    */
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
        Serial.println(F(" bytes. EOT\n\n"));
        client->stop();
    }

    String handleParam(String param){
        //valid syntax of an param:
        //  param:|X|Y|Z|=|v|a|l|u|e|...
        //char no:|0|1|2|3|4|5|6|7|8|...
        const int maxParLen = 32;
        const int parNameLen = REAS_MAX_PAR_LEN;

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

        //try methods embedded in REAS object:
        if (parName=="sta"){
            String status = "200 OK\n";
            status += antController.getOutputStatus();

            Serial.println(status);
            return status;
        }

        if (parName=="ant"){
            Serial.println(F("ant called..."));
            return "200 OK\n" + (String)antController.setExclusiveFromString(parVal);
        }
            
        if (parName=="mul"){
            Serial.println(F("mul called..."));
            return "200 OK\n" + (String)antController.setMultiFtomString(parVal);
        }
        //try to find function in function pointer array
        return runFunctionFromMap(parName,parVal);
    }
};

#endif //REAS_LIB