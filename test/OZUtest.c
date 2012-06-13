static int tstOZU(void)
{
    ULONG   param [256];


    int i = 256;
    do{
        i--;
        param[i] = 0x55555555;
        i--;
        param[i] = 0xaaaaaaaa;
    }while(i);

    BUF256x32_write( param);





    for(i=0; i<256; i++)
    {
        buffer47.ParamNumber= i;

        unsigned int templ = ( i&1)? 0x55555555 : 0xaaaaaaaa;

        ioctl ( hECE0206, ECE02061_READ_PARAM_1, &buffer47);

        if(buffer47.param!= templ)
        {
            printf("1.ECE02061_READ_PARAM_1 error: i=%03d  0x%08x (0x%08x)\n"
                                    ,i
                                    ,buffer47.param
                                    ,templ);
            return 1;
        }
    }



    i = 256;
    do{
        i--;
        param[i] = 0xaaaaaaaa;
        i--;
        param[i] = 0x55555555;
    }while(i);

    BUF256x32_write( param);


    for(i=0; i<256; i++)
    {
        buffer47.ParamNumber= i;

        unsigned int templ = ( i&1)? 0xaaaaaaaa : 0x55555555;

        ioctl ( hECE0206, ECE02061_READ_PARAM_1, &buffer47);

        if(buffer47.param!= templ)
        {
            printf("2.ECE02061_READ_PARAM_1 error: i=%03d  0x%08x (0x%08x)\n"
                                    ,i
                                    ,buffer47.param
                                    ,templ);
            return 1;
        }
    }


    return 0;
}
