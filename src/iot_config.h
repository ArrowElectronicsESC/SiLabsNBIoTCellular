//ECB Defines for Azure Cloud


//#define HTTP_VIEW_SERVER //Used to direct to Eric's private server for testing

#ifdef HTTP_VIEW_SERVER
	#define HOSTNAME "bepplerfamily.com"
	#define S_HOSTNAME "https://bepplerfamily.com"
	#define	HTTP_PORT 80
#else
	#define HOSTNAME "GGDigi1.azure-devices.net" //IOT Hub name is the first part of this address
	//#define S_HOSTNAME "https://GGDigi1.azure-devices.net" //No longer needed (I think...)
	#define	HTTP_PORT 443 //Must use this port and SSP for Azure
#endif
//Expires 1/1/2020
#define SASTOKEN "SharedAccessSignature sr=GGDigi1.azure-devices.net%2Fdevices%2Fggdevid1&sig=KLsM0xyKL3yHZmKECZ%2FTvXynxFkXjOuTY37xbTg6mHI%3D&se=1592749201"

//Device name in IOT HUB
#define DEVICECLIENT "ggdevid1"

// carrier profile att, verizon, no profile, or auto detect
#define CARRIERPROFILE	XBEE_CONFIG_CARRIER_ATT				// xbee_config.h enum

//Minimum time between updates (mS)- Will throttle the Cell TX - Set to 0 to remove
#define MIN_CONN_IVL 1000 //One second delay
//Maximum updated pushed to cloud - Prevents another crazy data bill....
#define DEV_DATA_LIMIT 5
