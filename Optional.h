#pragma  once
#include "wcomm_dll.h"
typedef struct _option
{
    BOOL    m_bShowDataHead;
	BOOL    m_bSaveToLog;
	int     m_iLogItemCount;

	BOOL    m_bSysAutoPoll;
	int     m_iServerPort;
	int     m_iPollTimeInterval;
	int     m_iOffLineTime;

	BOOL    m_bShowHead;
	BOOL    m_bHexShow;

	int		m_iProtocalType;//0 UDP 1 TCP
}OPTION,*LPOPTION;

typedef struct _settingParam
{
	uint8 nType;
	CString paramName;
	CString value;
	int paramLen;		//1 2 4  other -1 represent string   if it is 5 ipAddress is used
	DWORD ipAddress;	//just use for ip Address
}SETTINGPARAM,*LPSETTINGPARAM;