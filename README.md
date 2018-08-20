# aREAS - a REST-API External Antenna Switch

This project aims to create simple and efficient way to control
antenna switches via TCP/IP network, either via Ethernet cable or WiFi network.

Depending on configuration commands recieved via internet can be 
executed as GPIO actions, or may be sent via different medium, like
very slow serial output sent using the same medium as RF signal.

~~The project is based on aREST library to provide HTTP REST API.~~

_Sadly aREST library doesn't work with the cheapo ethernet sheld from
aliexpress, so a library compatible with standard ethernet.h is used,
and only basic http request handling is supported._ 


## Hardware platform
In order to simplify the hardware part, only two platforms will be served:

-Arduino nano (with atmeg328p) + cheap aliexpress nano ethernet shield  
-[TODO] ESP8266 WiFi-enabled chip with optional I2C I/O Extender for 9+ antennas

## Implemented API
What is REST API? It's the way to send request via http protocol, the same
that's used in your browser adress bar. This video sums it up pretty well:
https://www.youtube.com/watch?v=7YcW25PHnAA

The project will implement 3 basic configurations:

The query for switching between single antennas:
aREAS1.local/setAntenna?ant={0-16}  //0 - off, 1;16 - ant number

The query for turning on multiple antenna configurations:
aREAS1.local/multiAntennas?mul=xxxxxxxxxxxxxxxx //x = 0 or 1, 1;16 x'es

[TODO]The query for custom commands
aREAS1.local/?com={command} //for example "UHF" or "40M"

The device may be served directly from the browser window, but
generally it's meant to be controlled by an external application.

## TODO list:
Arduino:
- [x] create working Ethernet example
- [x] populate aREAS.h and antController.h files
- [ ] DHCP functionality
- [ ] create working WiFi example
- [ ] adapt aREAS.h and antController.h to work with both Ethernet and WiFi

Testing/clients:
- [ ] create python tests
- [ ] create simple html5+js client

Hardware:
- [x] create SimpleSlowSerial library for sending data with baudrates
    under 300bps (preferably 50bps) parallel to RF signals;
- [ ] include schematics
- [ ] synchronize antController.h with schematics
- [ ] include motherboard file
- [ ] create daughterboard for ESP8266 with pin layout identical 
    to arduino nano

Stuff:
- [ ] installation instructions
- [ ] deal with license stuff
- [ ] inviting to cooperation when project actually looks decent 

Copyright lololol Jakub Sadowski coffeeware placeholder license
