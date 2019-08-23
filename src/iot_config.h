#ifndef IOT_CONFIG_H
#define IOT_CONFIG_H

//#define LCDLOG 			// establishes LCD or Terminal logging

#define HOSTNAME "GGDigi1.azure-devices.net" //IOT Hub name is the first part of this address


#define SASTOKEN "SharedAccessSignature sr=GGDigi1.azure-devices.net%2Fdevices%2Fggdevid1&sig=KLsM0xyKL3yHZmKECZ%2FTvXynxFkXjOuTY37xbTg6mHI%3D&se=1592749201"

//Device name in IOT HUB
#define DEVICECLIENT "ggdevid1"

// carrier profile att, verizon, no profile, or auto detect
#define CARRIERPROFILE	XBEE_CONFIG_CARRIER_ATT				// xbee_config.h enum

#define XBEE_DEFAULT_APN "m2m.com.attz"

#define	HTTP_PORT 443 //Must use this port and SSP for Azure

//Minimum time between updates (mS)- Will throttle the Cell TX - Set to 0 to remove
#define MIN_CONN_IVL 1000 //One second delay
//Maximum updated pushed to cloud - Prevents another crazy data bill....
#define DEV_DATA_LIMIT 1000

#endif // IOT_CONFIG_H
