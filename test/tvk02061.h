
#ifndef __TVK02061_H__
#define __TVK02061_H__
#include "../ECE0206lin_ioctl.h"


#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#define    UCHAR     unsigned char
#define    USHORT     unsigned short int
#define    ULONG     unsigned long int
#define    PULONG     unsigned long int *
#define    DWORD     long int
#define    BOOL     bool
#pragma pack(1)

    int  hECE0206;
    int ch;
    int i;
    unsigned char Data[20];
    clock_t startTime, startTime1;
    int retval;

    typedef struct
     {
    DWORD  param;
    DWORD  timer;
    UCHAR  error;
    UCHAR  pollingError;
    }INPUTPARAM;


    struct
    {
        UCHAR   paramAddress;
        UCHAR   ArrayDim;
        ULONG   param [64];
    } bufInput41;


    struct
    {
        UCHAR  Err_en;
        UCHAR  Parity;
        UCHAR  Freq;
        UCHAR  ArrayDim;
        UCHAR  Delay;
        UCHAR  ArrayNumber;
    } bufInput43;


    union buffer47_
    {
        UCHAR ParamNumber;    //номер параметра (ячейки) в буфере (0..255)
        ULONG     param;
    } buffer47;

    struct  _bufInput51
    {
        UCHAR  ChanNumber;    //номер канала (1..4)
        UCHAR  Mode;        // = 0  - рабочий режим; = 1 - режим самоконтроля
        UCHAR  Parity;        //= 0  - без контроля нечетности; = 1 - контроль нечетности
        UCHAR  Freq;        //=0 – прием на частотах 36-100КГц; = 1 – прием на частотах 11-14,5 КГц
    } ;
    union
    {
        struct _bufInput51    bufInput51 ;
        UCHAR  bufOutput51;
    } buffer51;

    UCHAR buffer53;        //номер канала (1..4)

    union
    {
        UCHAR ParamNumber;
        INPUTPARAM   ParamCod;
    } buffer55;


#include "library02061.c"
#include "OZUtest.c"
#include "SO_test.c"
#include "SI_test.c"



#endif // __TVK02061_H__
