// GPRS_SMM.h : main header file for the GPRS_SMM DLL
// Edited by xie hong sheng , 2002-11 ,CopyRight hongdian
#ifndef __COMM_SMS_H
#define __COMM_SMS_H

#ifndef DSCDLLAPI
#define DSCDLLAPI  extern "C"
#endif

#ifndef  HDCALL
#define  HDCALL        __stdcall 
#endif


/*************************************************************/
// Interface to DSC
/*
int  SMMInit(char *DSCIpAddr,int DSCSrvPort,
			 char *SerialCom,DCB *dcb,
			 char *SMSCNumber,char *SMMNumber,int WorkMode);
1,DSCIpAddr:DSC ip address;
2,DSCSrvPort:DSC server port;
3,SerialCom:the serial comm than connect gprs modem;
4,dcb:information that config comm,containing baudrate,bytesize,stopbits,pairty;
5,SMSCNumber:SMSC phone number;
6,SMMNumber:the gprs modem phone number;
7,WorkMode:if 0,the dsc ip and port is not changed,1,changed;

result:
0:    success;
-10:  argument error;
-1:   DSCIpAddr or DSCSrvPort error;
-2:   open serial comm error;
-3:   open socket error;
-4:   create mutex error;
-5:   configSMM error;
-6:   SetTimer error;
*/
DSCDLLAPI int  HDCALL SMMInit(char *,int,char *,DCB *,char *,char *,int);

/*
    if you call SMMInit,then you must call SMMFree to free 
	the resource that got in the SMMInit;
*/
DSCDLLAPI int  HDCALL SMMFree(void);

/*
    int  SMMSetting(char *DSCIpAddr,int DSCSrvPort)
    Set the dsc ip address and dsc server port to smminfo struct;
	DSCIpAddr:dsc ip address;
	DSCSrvPort:dsc server port;
result:
    0:success;
	-1:failed;	
*/
DSCDLLAPI int  HDCALL SMMSetting(char *,int);

/*
    int MakeDTUOnLine(int Changed,char *DTUNumber)
    Changed:0-not changed,1-changed;
    DTUNumber:DTU phone number which you want make on line; 
result:
    0-success;
	-1:failed;
*/
DSCDLLAPI int HDCALL MakeDTUOnLine(int,char *);

/*
    int  SendDataToDTUBySM(char *DTUNumber,char *szData,int nDataLen,int nProtoType)
	send data to DTU by sm,this mode is used if the gprs service doesn't work;
	DTUNumber:the DTU phone number that you want to send data to;
	szData:the data that you want to send to DTU;
	nDataLen:the length of szData;
	nProtoType:data type,0-configure data,1-user data
result:
    0:success;
	-1:failed;
*/
DSCDLLAPI int  HDCALL SendDataToDTUBySM(char *,char *,int,int);

/*
 *Add by SEA on Oct 27,2003
 *Setup Modem type
 *if use CDMA modem,set 1
 *if use GPRS modem,set 0
 *default value 0
 */
DSCDLLAPI void  HDCALL SetModemType(int nModemType);
#endif 
