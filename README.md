aREAS - aREST External Antenna Switch

This project aims to create simple and effecient way to control
antenna switches via TCP/IP network, either via Ethernet cable or WiFi network.

The project is based on aREST library to provide HTTP REST API.

What is REST API? It's the way to send request via http protocol, the same
that's used in your browser adress bar. This video sums it up pretty well:
https://www.youtube.com/watch?v=7YcW25PHnAA

In order to simplify the hardware part, only two platforms will be served:

-Arduino nano (with atmeg328p) + standard Ethernet shield
-ESP8266 WiFi-enabled chip with optional I2C I/O Extender for 9+antennas

The project will implement 3 basic configurations:

The query for switching between single antennas:
aREAS1.local/setAntenna?ant={1-16}

The query for turning on multiple antenna configurations:
aREAS1.local/multiAntennas?ants=xxxxxxxxxxxxxxxx //x = 0 or 1

The query for custom commands
aREAS1.local/command?comm={command} //for example "UHF" or "40M"

The device may be served directly from the browser window, but
generally it's meant to be controlled by an external application.

TODO list:
    Arduino:
    [ ] create working Ethernet example
    [ ] populate aREAS.h and aREAS_hal.h files
    [ ] create working WiFi example
    [ ] adapt aREAS.h and aREAS_hal.h to work with both Ethernet and WiFi

    Testing/clients:
    [ ] create python tests
    [ ] create simple html5+js client
    
    Hardware:
    [ ] include schematics
    [ ] synchronize aREAS_hal.h with schematics
    [ ] include motherboard file
    [ ] create daughterboard for ESP8266 with pin layout identical 
        to arduino nano
    
    Stuff:
    [ ] installation instructions
    [ ] deal with license stuff

Copyright lololol Jakub Sadowski under coffeeware license


