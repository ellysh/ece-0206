

///////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////
static int   inputChannelTest()
{
	
	ULONG	param[256];
	DWORD tick1;

	unsigned int	ii;
  	for( ii = 0; ii<256; ii++)
	{
		param[ii] = 0x70000000 + (ii<<16) + (((~ii)<<8)&0xff00) + ii;
	}

	BUF256x32_write( param);

	UCHAR  soErr_en = 0;
	UCHAR  soParity = 1;
	UCHAR  soFreq;	
	UCHAR  soArrayDim = 0;
	UCHAR  soDelay = 0;	
	UCHAR  soArrayNumber = 0;	

	UCHAR  siChanNumber;	
	UCHAR  siMode;	
	UCHAR  siParity = 1;	
	UCHAR  siFreq;	
	UCHAR siChanNumber1;

	
	ioctl ( hECE0206, ECE02061_SET_SHORT_MODE);				

	for( siChanNumber =1; siChanNumber<=4; siChanNumber++)
	{
		printf("Channel # %d\n",siChanNumber);

		//	

		for( siMode = 1; siMode <=1; siMode--)
		{
			printf("   %s\n",siMode ? "Self-checking mode" : "Operating mode (with stub)");

			// 

			for( soFreq = 0; soFreq <=2; soFreq++)
			{
				siFreq = soFreq ? 0 : 1;

				frequency_printf(siFreq, soFreq);

				// 
				for( siChanNumber1 =1; siChanNumber1<=4; siChanNumber1++)
				{
					SI_pusk( siChanNumber1, siMode, siParity, siFreq);
					intervalMs(100);
					SI_clear_array(siChanNumber1);
				}

				// 
				SO_pusk(soErr_en, soParity, soFreq, soArrayDim, soDelay, soArrayNumber);

					startTime = clock();
					startTime1 = startTime;
					while (((startTime1 - startTime)*1000/CLOCKS_PER_SEC)<1000) 
					{
						startTime1 = clock();
						printf("%d \r",(startTime1 - startTime)*1000/CLOCKS_PER_SEC);
					};


				if(inputParamCodeCheck( siChanNumber
						,256
						,param))
				{
					SO_stop();
					SI_stop(1);
					SI_stop(2);
					SI_stop(3);
					SI_stop(4);
					return 1;
				}

				SO_stop();
				SI_stop(1);
				SI_stop(2);
				SI_stop(3);
				SI_stop(4);

			}//soFreq 
		}// siMode = 1/0
	}//for( siChanNumber =1; siChanNumber<=4; siChanNumber++)

	return 0;
}
