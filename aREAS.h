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

    AntController* antController;

    public:    


    aREAS_Handler(){
        antController = nullptr;
    }

    aREAS_Handler(AntController* a){
        antController = a;
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
        const int maxParLen = 32;
        Serial.print("param substring:");
        Serial.println(param.substring(0,3));
        if (param.substring(0,3)=="ant"){
            String value = param.substring(4,maxParLen);
            Serial.print("ant param: ");
            Serial.println(value);
            return antController->setExclusiveFromString(value);
        }
            
        if (param.substring(0,3)=="mul"){
            String value = param.substring(4,maxParLen);
            Serial.print("mul param: ");
            Serial.println(value);
            return antController->setMultiFtomString(value);
        }        
    }
};

#endif //AREAS_LIB