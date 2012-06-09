#include "tvk02061.h"


main()
{
int 	rj, i1,i,ii;

	hECE0206=open("/dev/ece0206_0",0);

	if(hECE0206==-1)
	{
   		printf("open error1 \n")	;
		return;
	}

	printf("\nhECE0206 = 0x%0x\n",hECE0206);

	//********************
	ioctl(hECE0206
		,ECE02061_GET_SERIAL_NUMBER
		,Data);
	
   	printf("serial # %s\n",Data)	;

	ioctl(hECE0206
		,ECE02061_GET_PRODUCT_NAME
		,Data);
	
   	printf("PRODUCT_NAME:  %s\n",Data)	;

	ioctl(hECE0206
		,ECE02061_GET_MANUFACTURER_NAME
		,Data);
	
   	printf("MANUFACTURER_NAME: %s\n",Data)	;

	
	//********************


	printf("\nOZU test\n");

	if( tstOZU())
	{
		printf("OZU test ERROR\n");
		close ( hECE0206 );
		return ;
	}
	printf("OK\n");

	//********************

	printf("\nOutput Channel (SO) Test \n");
	
	if( outputChannelTest())
	{
		printf("OutputChannelTest ERROR\n");
		close ( hECE0206 );
		return ;
	}
	printf("\r            \r");
	printf("OK\n");

	//********************

	printf("\nInput Channels (SI) Test \n");
	
	if( inputChannelTest())
	{
		printf("InputChannelTest ERROR\n");
		close ( hECE0206 );
		return ;
	}

	printf("\r            \r");
	printf("OK\n");
 
	close ( hECE0206 );
	
	
}
