
#define CHANNEL_PUSK	SI_pusk( siChanNumber, siMode, siParity, siFreq);\
			intervalMs(300);\
			SI_clear_array(siChanNumber);\
			SO_pusk(soErr_en, soParity, soFreq, soArrayDim, soDelay, soArrayNumber);
//			intervalMs(300);

#define myDEBUG		1	

#define INPUTCHANNELNUMBER		1	


///////////////////////////////////////////////////////////////////////
//
//  
//
//////////////////////////////////////////////////////////////////////
static int  outputChannelTest()
{
	DWORD	minPeriodTemplate;
	DWORD	maxPeriodTemplate;

	DWORD tick1;
	int array_number;
	
	unsigned int	ii;
	int f;

	UCHAR  siChanNumber = INPUTCHANNELNUMBER;	
	UCHAR  siMode;	
	UCHAR  siParity;
	UCHAR  siFreq;	

	UCHAR  soErr_en;
	UCHAR  soParity;
	UCHAR  soFreq;	
	UCHAR  soArrayDim;
	UCHAR  soDelay;	
	UCHAR  soArrayNumber;

	UCHAR ArrayDimVariantsInd;
	UCHAR ArrayNumberVariantsInd;	

	ULONG   param [256];	
	
	ioctl ( hECE0206, ECE02061_SET_LONG_MODE);				



	//	
	for( ii = 0; ii<256; ii++)
	{
		param[ii] = 0x70000000 + (ii<<16) + (((~ii)<<8)&0xff00) + ii;
	}

	BUF256x32_write( param);

	siParity = 1;
	soParity = 1;
	soErr_en = 0;


	printf(" SINGLE OUTPUT:\n");
	soArrayNumber = 1;
	soDelay = 0;

	for( siMode = 1; siMode <=1; siMode--)
	{
		printf("   %s\n",siMode ? "Self-checking mode" : "Operating mode (with stub)");

		for( soFreq = 0; soFreq <=2; soFreq++)
		{
			siFreq = soFreq ? 0 : 1;

			frequency_printf(siFreq, soFreq);

			if (soFreq)
			{
				minPeriodTemplate = (720/soFreq)-10;
				maxPeriodTemplate = (720/soFreq)+10;
			}
			else
			{
				minPeriodTemplate = 2800;
				maxPeriodTemplate = 2960;
			}

			// 

			UCHAR	ArrayDimVariants[4] = {1,128,255,0};	

			for( ArrayDimVariantsInd = 0; ArrayDimVariantsInd <4; ArrayDimVariantsInd++)
			{
				soArrayDim = ArrayDimVariants[ArrayDimVariantsInd];

				unsigned int soArrayDim_int = soArrayDim ? soArrayDim : 256;
				
				printf("          SO Array Dimension = %3d\n",soArrayDim_int);
			
				CHANNEL_PUSK;

				intervalMs(1500);

				if(inputParamCodeCheck( siChanNumber
							, soArrayDim_int
							, param))
				{
					SI_stop(siChanNumber);
					return 1;
				}

				if(test_period( siChanNumber
							,soArrayDim_int
							,minPeriodTemplate
							,maxPeriodTemplate))
				{
					SI_stop(siChanNumber);
					return 1;
				}

				SI_stop(siChanNumber);

			}//soArrayDim
		}//soFreq 
	}// siMode = 1/0


	UCHAR	ArrayNumberVariants[2] = {2,5};

	for( ArrayNumberVariantsInd = 0; ArrayNumberVariantsInd <2; ArrayNumberVariantsInd++)
	{
		soArrayNumber = ArrayNumberVariants[ArrayNumberVariantsInd];
		printf(" MULTIPLE OUTPUT:  SO Array Number = %3d\n",soArrayNumber);

		for( siMode = 1; siMode <=1; siMode--)
		{
			printf("   %s\n",siMode ? "Self-checking mode" : "Operating mode (with stub)");

			for( soFreq = 0; soFreq <=2; soFreq++)
			{
				siFreq = soFreq ? 0 : 1;

				frequency_printf(siFreq, soFreq);

				soArrayDim = 1;
				soDelay = 0;
				printf("          SO Array Dimension = 1\n");

				CHANNEL_PUSK;

				startTime = clock();
				startTime1 = startTime;
				while (((startTime1 - startTime)*1000/CLOCKS_PER_SEC)<1000) 
				{
					startTime1 = clock();
					printf("%d \r",(startTime1 - startTime)*1000/CLOCKS_PER_SEC);
				};
				INPUTPARAM inputParam[256];
				read_array_CC(siChanNumber, inputParam );

				array_number = 0;

				for( ii = 0; ii<256; ii++)
					if(param[0] == (inputParam[ii].param&0x7fffffff)) array_number++;

				if(array_number != soArrayNumber)
				{
					printf("          ERROR: input array number = %d\n",array_number);
					#ifdef myDEBUG					
					for( f=0;f<256;f++)
					{
						printf("paramN:%3d  inputParam: %08x timer:%08x error:%02x\n"
							,f
							,inputParam[f].param
							,inputParam[f].timer
							,inputParam[f].error);
					}
					#endif
					SI_stop(siChanNumber);
					return 1;
				}  

				SI_stop(siChanNumber);

				soArrayDim = 0;
				printf("          SO Array Dimension = 256\n");
				soDelay = 0;
				array_number = 0; 
				INPUTPARAM  bufOutput57;

				CHANNEL_PUSK;

				startTime = clock();
				do							// 
				{
					read_parameter_CC(siChanNumber
						,255
						,&bufOutput57);
					startTime1 = clock();
					printf("%d \r",(startTime1 - startTime)*1000/CLOCKS_PER_SEC);
				}while((bufOutput57.param==0)&&(((startTime1 - startTime)*1000/CLOCKS_PER_SEC)<1000));

				if(bufOutput57.param==0)
				{
					printf("          ERROR: first input array timeout \n");
					SI_stop(siChanNumber);
					return 1;
				}
				DWORD  timerTemp0 = bufOutput57.timer;
				array_number++; 
					
				startTime = clock();
				do
				{
					read_parameter_CC(siChanNumber
						,255
						,&bufOutput57);
					if(bufOutput57.timer !=timerTemp0)
					{
						array_number++; 
						timerTemp0 = bufOutput57.timer;
					}

					startTime1 = clock();
					printf("%d \r",(startTime1 - startTime)*1000/CLOCKS_PER_SEC);
				}while (((startTime1 - startTime)*1000/CLOCKS_PER_SEC)<1000*(unsigned long int)soArrayNumber);//

/*			printf("\n2. param: %08x  timer: %08x  error: %02x\n"
				,bufOutput57.param
				,bufOutput57.timer
				,bufOutput57.error);
*/
					if(array_number!=soArrayNumber)
					{
						printf("          ERROR: array_number = %d \n",array_number);
						SI_stop(siChanNumber);
						return 1;
					}

					SI_stop(siChanNumber);

			}//soFreq 
		}// siMode = 1/0
	}


	return 0;
}

