
/**
 * This example shows how to attach custom
 * functions to your REAS controller.
 * 
 * 1.Write your custom functions - they 
 *    need to:
 *    a) Take single String variable as parameter
 *    b) Return int value. 0 = OK, not 0 = Error
 * 
 * 2.initialize REAS_Handler with less than
 *    16 outputs so there's some GPIO left 
 *    for custom functions.
 * 
 * 3.Attach up to 10 custom functions via
 *    attachCallback() method:
 *    a) First parameter is 3-character length
 *      String you want your function to be 
 *      invoked with
 *    b) Second one is pointer to your custom 
 *      function. You can get it by preceding its
 *      name with "&" and ommiting "()"
 * 
 * 4.Enjoy REAS controller executing your own
 *    functions!
 * 
 * 
 * In this example we use REAS controller to
 * controll multiple antennas via Slow serial
 * and filter bank via a couple of GPIOs.
 * 
 * We also see some functions for verbose setup,
 * like 40M, UHF... whatever you'd like!
 * 
 */


#include <UIPEthernet.h>
#include <REAS.h>

#define ANT_40M_NO 1
#define ANT_UHF_NO 2

/** You might want to give unique MAC to every
 * REAS controller in the network */
char mac[] = {
    0xDD, 0xDD, 0xDD, 0xDD, 0x15, 0x25
};
//TODO: setting up custom IP address
IPAddress ip(192, 168, 2, 200);

EthernetServer server(80);

REAS_Handler aHandler(8);
/** custom functions **/

String setFilterBankFromString(String s){
  int fltrNo = s.toInt();
  if ((fltrNo>0)&&(fltrNo<4)){
    Serial.print("activating fltr no ");
    Serial.println(fltrNo);
    //set some GPIO
  } 
  else{
    Serial.print("wrong param ");
    Serial.println(fltrNo);

  }
}

String set40M(String s){
  /** string may be ignored, command
   * "/?40M" (without "=sth") will trigger 
   * it anyway. **/
  Serial.print("Activating antena for the 40M band");
  /** an user may access antController object
   * in order to create his custom functions.
   * however it's probably better to give antenna 
   * their verbose name on API client. **/
  return (String)aHandler.antController.setExclusive(ANT_40M_NO);//No of 40M ant.
}

String setUHF(String s){
  Serial.print("Activating antena for the UHF band");
  return (String)aHandler.antController.setExclusive(ANT_UHF_NO);//No of UHF ant.
}


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

  aHandler.attachCallback("flt", &setFilterBankFromString);
  aHandler.attachCallback("40M", &set40M);
  aHandler.attachCallback("UHF", &setUHF);
}

void loop() {
  // listen for incoming clients
  EthernetClient client = server.available();
  if(client) aHandler.handleClient(&client);
}
