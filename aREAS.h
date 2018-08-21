#include <UIPEthernet.h>
#include <antController.h>

#ifndef AREAS_LIB
#define AREAS_LIB

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
class aREAS_Handler{

    AntController antController;

    public:    


    aREAS_Handler(uint8_t antennaCount):antController(antennaCount){
        volatile int test = 0;
    }


    void handleClient(EthernetClient client){
        Serial.println("new client\n\n");
        // an http request ends with a blank line
        boolean currentLineIsBlank = true;
        String param = "";
        bool isRequest = false;

        int charCounter = 0;
        const int charCounterMax = 64;//max request length
        client.println("HTTP/1.1 200 OK");
        client.println("Content-Type: text/plain");
        client.println("Access-Control-Allow-Origin: *");
        client.println(); 
        while (client.connected()) {
            if (client.available()) {
                char c = client.read();
                if (charCounter++ > charCounterMax){
                    client.print("Request too long (over ");
                    client.print(charCounterMax);
                    client.println("characters)");
                    break;
                }
                Serial.write(c);

                // if you've gotten to the end of the line (received a newline
                // character) and the line is blank, the http request has ended,
                // so you can send a reply
                if ((c == ' ')&&(isRequest)) {
                    // send a standard http response header        
                    client.println(param);
                    if(handleParam(param)==0){
                        client.println("param OK");
                    }
                    else{
                        client.println("invalid param");
                    }
                    break;
                }
                
                if(isRequest){
                    param+=c;
                }

                if(c == '?'){
                    isRequest = true;
                }

                if(c=='\n'){
                    client.println("Request not valid (no '?' found)\n"
                                    "valid syntax: <IPADDR>/?fun=parameters");
                    break;
                }
            }
        }
        client.stop();
        Serial.println("EOT");
    }

    int handleParam(String param){
        //valid syntax of an param:
        //  param:XYZ=value...
        //char no:012345678...
        const int maxParLen = 32;
        const int parNameLen = 3;

        //get "XYZ"
        String parName = param.substring(0,parNameLen);
        //check if under 3rd char is "="
        Serial.print("param name:");
        Serial.println((String)parName);
        if(param.charAt(parNameLen) != '='){
            Serial.println("'equals' not on valid pos");
            return -1;
        }
        //get chars from 4...maxParLen 
        //(or less, .substring() handles less than max chars)
        String parVal = param.substring(4,maxParLen);
        Serial.print("param val: ");
        Serial.println(parVal);

        if (parName=="ant"){
            Serial.println("ant called...");
            return antController.setExclusiveFromString(parVal);
        }
            
        if (parName=="mul"){
            Serial.println("mul called...");
            return antController.setMultiFtomString(parVal);
        }        
    }
};

#endif //AREAS_LIB