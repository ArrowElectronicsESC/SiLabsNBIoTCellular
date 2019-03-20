//ECB Defines for Azure Cloud


//#define HTTP_VIEW_SERVER //Used to direct to Eric's private server for testing

#ifdef HTTP_VIEW_SERVER
	#define HOSTNAME "bepplerfamily.com"
	#define S_HOSTNAME "https://bepplerfamily.com"
	#define	HTTP_PORT 80
#else
	#define HOSTNAME "ECB-Arrow-Dev.azure-devices.net" //IOT Hub name is the first part of this address
	#define S_HOSTNAME "https://ECB-Arrow-Dev.azure-devices.net" //No longer needed (I think...)
	#define	HTTP_PORT 443 //Must use this port and SSP for Azure
#endif
//Expires 1/1/2020
#define SASTOKEN "SharedAccessSignature sr=ECB-Arrow-Dev.azure-devices.net&sig=N%2BkkNRr7gXV%2Fq0rF04tuKx58h8I8i14G31R1OXQv%2BfA%3D&se=1584216455&skn=iothubowner"
//Device name in IOT HUB
#define DEVICECLIENT "ECB_DEV_0"

//Minimum time between updates (mS)- Will throttle the Cell TX - Set to 0 to remove
#define MIN_CONN_IVL 1000 //One second delay
//Maximum updated pushed to cloud - Prevents another crazy data bill....
#define DEV_DATA_LIMIT 10
