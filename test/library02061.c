#include <sys/ioctl.h>

void intervalMs(int interval)
{

	startTime = clock();
	do{
	}while((clock()-startTime)*1000/CLOCKS_PER_SEC < interval);

}


//----------------------------------------------------------------------------------------
//
void BUF256x32_write(PULONG paramArray)
{


	for( ch =0; ch<4; ch++)
	{
		bufInput41.paramAddress = ch*64;
		bufInput41.ArrayDim = 64;

		for( i = 0; i<64; i++)
		{
			bufInput41.param [i] = *(paramArray + ch*64 + i);
		}

		retval = ioctl ( hECE0206, ECE02061_WRITE_ARRAY_1, &bufInput41);
		if(retval)
			printf("BUF256x32_write: retval = %d",retval);

		intervalMs(300);

	}



}

//----------------------------------------------------------------------------------------
//

void SI_pusk(UCHAR ChanNumber, UCHAR Mode, UCHAR Parity, UCHAR Freq)
{

	buffer51.bufInput51.ChanNumber = ChanNumber;
	buffer51.bufInput51.Mode = Mode;
	buffer51.bufInput51.Parity = Parity;
	buffer51.bufInput51.Freq = Freq;

	retval = ioctl ( hECE0206, ECE02061_SI_PUSK, &buffer51);
	if(retval)
		printf("SI_pusk: retval = %d",retval);
}

//----------------------------------------------------------------------------------------
//

void SO_pusk(UCHAR Err_en, UCHAR Parity, UCHAR Freq, UCHAR ArrayDim, UCHAR Delay, UCHAR ArrayNumber)
{

	bufInput43.Err_en = Err_en;
	bufInput43.Parity = Parity;
	bufInput43.Freq = Freq;
	bufInput43.ArrayDim = ArrayDim;
	bufInput43.Delay = Delay;
	bufInput43.ArrayNumber = ArrayNumber;

	retval = ioctl ( hECE0206, ECE02061_SO_PUSK1, &bufInput43);
	if(retval)
		printf("SO_pusk: retval = %d",retval);
}

//----------------------------------------------------------------------------------------
//

void SI_clear_array(UCHAR ChanNumber)
{
	buffer53 = ChanNumber;
	retval = ioctl ( hECE0206, ECE02061_CLEAR_SI_ARRAY, &buffer53);
	if(retval)
		printf("SI_clear_array: retval = %d",retval);
}

//----------------------------------------------------------------------------------------
//

void SI_stop(UCHAR ChanNumber)
{

	retval = ioctl ( hECE0206, ECE02061_SI_STOP, &ChanNumber);
	if(retval)
		printf("SI_stop: retval = %d",retval);

}

//----------------------------------------------------------------------------------------
//

void SO_stop(void)
{

	retval = ioctl ( hECE0206, ECE02061_SO_STOP1);
	if(retval)
		printf("SO_stop: retval = %d",retval);

}

//----------------------------------------------------------------------------------------
//

void read_array_CC(UCHAR ChanNumber,INPUTPARAM * bufOutput )
{
	ULONG kop;

	switch(ChanNumber)
	{
	case 1:
		kop = ECE02061_READ_ARRAY_CC1;
		break;
	case 2:
		kop = ECE02061_READ_ARRAY_CC2;
		break;
	case 3:
		kop = ECE02061_READ_ARRAY_CC3;
		break;
	case 4:
		kop = ECE02061_READ_ARRAY_CC4;
		break;
	}


	retval = ioctl ( hECE0206, kop, bufOutput);
	if(retval)
		printf("read_array_CC: retval = %d",retval);
}


//----------------------------------------------------------------------------------------
//

void read_parameter_CC(UCHAR ChanNumber, UCHAR ParamNumber,INPUTPARAM * bufOutput )
{
	ULONG kop;

	buffer55.ParamNumber = ParamNumber;

	switch(ChanNumber)
	{
	case 1:
		kop = ECE02061_READ_PARAM_CC1;
		break;
	case 2:
		kop = ECE02061_READ_PARAM_CC2;
		break;
	case 3:
		kop = ECE02061_READ_PARAM_CC3;
		break;
	case 4:
		kop = ECE02061_READ_PARAM_CC4;
		break;
	}


	retval = ioctl ( hECE0206, kop, &buffer55);
	if(retval)
		printf("read_parameter_CC: retval = %d",retval);

	bufOutput->param = buffer55.ParamCod.param;
	bufOutput->timer = buffer55.ParamCod.timer;
	bufOutput->error = buffer55.ParamCod.error;

}




//----------------------------------------------------------------------------------------
//

void frequency_printf(UCHAR siFreq,UCHAR soFreq)
{

	if(siFreq)
		printf("      frequency: si = Slow");
	else
		printf("      frequency: si = Fast");


	if(soFreq==0)
		printf(" so = 12,5KHz\n");
	else
	{
		if(soFreq==1)
			printf(" so = 50KHz\n");
		else
			printf(" so = 100KHz\n");
	}

}

//------------------------------------------------------------------------------------------------
//

int	inputParamCodeCheck(UCHAR siChanNumber,unsigned int soArrayDim_int, ULONG * outputParam)
{

	INPUTPARAM inputParam[256];
	unsigned int paramN = 0;

	read_array_CC(siChanNumber, inputParam );


	for( ; paramN<soArrayDim_int; paramN++)
		if(
			( (outputParam[paramN] != (inputParam[paramN].param&0x7fffffff))
			  ||(inputParam[paramN].error!=0x0f )
			)
			&&(!inputParam[paramN].pollingError)
		  )
		{
			printf("paramN: %3d  outputParam: %08x  inputParam: %08x  error: %02x  pollingError: %02x\n"
				,paramN
				,outputParam[paramN]
				,inputParam[paramN].param
				,inputParam[paramN].error
				,inputParam[paramN].pollingError);

			#ifdef myDEBUG
			printf("\n");
			for(unsigned int f=0;f<soArrayDim_int;f++)
			{
				buffer47.ParamNumber= f;

				retval = ioctl ( hECE0206, ECE02061_READ_PARAM_1, &buffer47);
				if(retval)
					printf("inputParamCodeCheck: retval = %d",retval);

				printf("paramN:%3d outputParam:%08x inputParam:%08x timer:%08x error:%02x pollingError: %02x\n"
										,f
										,buffer47.param
										,inputParam[f].param
										,inputParam[f].timer
										,inputParam[f].error
										,inputParam[f].pollingError);
			}
			#endif
				return 1;
		}
				return 0;
}


//----------------------------------------------------------------------------------------
//

int test_period(UCHAR siChanNumber
				, unsigned int soArrayDim_int
				, DWORD minPeriodTemplate
				, DWORD maxPeriodTemplate)
{
	INPUTPARAM inputParam[256];
	unsigned int ii;

	read_array_CC(siChanNumber, inputParam );

	DWORD  timerTemp0 = inputParam[0].timer;

	DWORD temp0 = (timerTemp0>>8)*1024+(timerTemp0&0xff)*4;

	for( ii = 1; ii<soArrayDim_int; ii++)
	{
		DWORD  timerTemp1 = inputParam[ii].timer;
		DWORD temp1 = (timerTemp1>>8)*1024+(timerTemp1&0xff)*4;
		DWORD deltatemp = (temp1-temp0);

		if(( deltatemp < minPeriodTemplate)||( deltatemp > maxPeriodTemplate))
		{
			printf("ERROR:paramN:%3d   input parameter period = %d PeriodTemplate = %d - %d\n"
				,ii
				,deltatemp
				,minPeriodTemplate
				,maxPeriodTemplate);

			#ifdef myDEBUG
			for(unsigned int f=0;f<soArrayDim_int;f++)
			{
				printf("paramN:%3d  inputParam: %08x timer:%08x error:%02x pollingError:%02x\n"
					,f
					,inputParam[f].param
					,inputParam[f].timer
					,inputParam[f].error
					,inputParam[f].pollingError);
			}
			#endif
			return 1;
		}
		temp0 = temp1;
	}
	return 0;

}
