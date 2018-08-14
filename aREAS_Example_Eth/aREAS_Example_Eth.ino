//#include <SPI.h>
#include <UIPEthernet.h>

#include <aREAS.h>

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(192, 168, 2, 200);


EthernetServer server(80);

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }


  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
}


void handleParam(String param){
  const int maxParLen = 32;
  if (param.substring(0,3)=="ant"){
    String value = param.substring(4,maxParLen);
    Serial.println(value);
    int antNo = aREAS::stringToAntNo(value);
    aREAS::setOneAnt(antNo);
  }
    
  if (param.substring(0,3)=="mul"){
    aREAS::Ants ants;
    String value = param.substring(4,maxParLen);
    Serial.println(value);
    aREAS::stringToMulAnt(&ants, value);
    aREAS::setMulAnt(ants);
  }
    
}

void loop() {
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client\n\n");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    String param = "";
    bool isRequest = false;
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/plain");
    client.println(); 
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        
        
        
        if ((c == ' ')&&(isRequest)) {
          // send a standard http response header        
          client.println(param);
          handleParam(param);
          break;
        }
        
        if(isRequest){
          param+=c;
        }

        if(c == '?'){
          isRequest = true;
        }

        if(c=='\n'){
          client.println("zjebało sie");
          break;
        }
      }
    
    }
    
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }
}
