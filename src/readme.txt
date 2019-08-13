Digi XBee Device Cloud example.

This example project uses a Digi XBee module to communicate via LTE CAT-M1. It
transmits temperature data to Azure.


At startup, the EFM32GG11 will attempt to communicate with the XBee module
via UART. The application assumes the XBee is configured with a baudrate of
115200 bps and its API mode set to API Mode Without Escapes. If this initial
communication fails, the behavior of the application can be controlled by
defining or not defining a symbol called XBEE_DEMO_CONFIG at the compiler
command line.

If XBEE_DEMO_CONFIG is defined, the application will reset the XBee and then
attempt to configure it to 115200 bps/API Mode Without Escapes.

If XBEE_DEMO_CONFIG is not defined, the application will halt execution in a
while(1) loop at the point of failure.

If XBEE_DEMO_HOLOGRAM_SIM is defined and XBEE_DEMO_CONFIG, the application will
configure the XBee to work with the Hologram SIM card. This configuration
involves disabling NB-IoT on the modem. This is a slow process that takes over
20 seconds. Once NB-IoT has been disabled on a given XBee, that setting is
saved in non-volatile memory. So, if the user is confident that his XBee is
configured correctly for use with the Hologram SIM card, he can remove this
configuration define in the project settings and the demo will still operate
correctly.

If XBEE_CHANGE_APN is defined, the application will configure the Access Point 
Name (APN) setting of the XBee. The default value in "-". In the case of the 
Hologram SIM card, it is recommended to set the APN to "hologram".

The EFM32 prints logging data either through the on board LCD or prints to the 
serial terminal. To print to LCD, make sure LCDLOG is defined inside of iot_config.h
To print to serial terminal, make sure that define is commented out.

To change carrier profile settings, change the define in iot_config.h CARRIERPROFILE 
to desired network provider (ATT or Verizon).

In order to connect to the cloud, make sure HOSTNAME, SASTOKEN, and DEVICECLIENT are
configured corectly inside iot_config.h. HOSTNAME comes from Azure IoT hub, SASTOKEN
should be a signature token generated inside of your device monitor. DEVICECLIENT is 
the device name inside of your IoT hub.

Board:  Silicon Labs SLSTK3701A Starter Kit attached to BRD8021A Expansion Board
Device: EFM32GG11B820F2048GL192

