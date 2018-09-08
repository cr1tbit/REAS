
/**
 * Basic example of the REAS library:
 * 1.initialize REAS_Handler with (16)
 * 2.enjoy control over 16 antenna outputs
 *    via ethernet.
 */


#include <UIPEthernet.h>
#include <REAS.h>

/** You might want to give unique MAC to every
 * REAS controller in the network */
char mac[] = {
    0xDD, 0xDD, 0xDD, 0xDD, 0x15, 0x25
};
//TODO: setting up custom IP address
IPAddress ip(192, 168, 2, 200);

EthernetServer server(80);

REAS_Handler aHandler(16);


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

void loop() {
  // listen for incoming clients
  EthernetClient client = server.available();
  if(client) aHandler.handleClient(client);
}
