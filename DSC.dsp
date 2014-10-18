# Microsoft Developer Studio Project File - Name="DSC" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=DSC - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "DSC.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "DSC.mak" CFG="DSC - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "DSC - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "DSC - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/发布程序/DSC/函数开发包/DEMO代码/DSC_源代码", MRBAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "DSC - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /Fr /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 ws2_32.lib /nologo /subsystem:windows /machine:I386 /out:"Release/DSC 中心程序.exe"
# SUBTRACT LINK32 /incremental:yes /debug

!ELSEIF  "$(CFG)" == "DSC - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ws2_32.lib /nologo /subsystem:windows /debug /machine:I386 /out:"Debug/DSC 中心程序.exe" /pdbtype:sept
# SUBTRACT LINK32 /incremental:no

!ENDIF 

# Begin Target

# Name "DSC - Win32 Release"
# Name "DSC - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AddUserDlg1.cpp
# End Source File
# Begin Source File

SOURCE=.\BottomEditView.cpp
# End Source File
# Begin Source File

SOURCE=.\DateBase.cpp
# End Source File
# Begin Source File

SOURCE=.\DSC.cpp
# End Source File
# Begin Source File

SOURCE=.\DSC.rc
# End Source File
# Begin Source File

SOURCE=.\DSCDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\DSCView.cpp
# End Source File
# Begin Source File

SOURCE=.\LeftView.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\MenuBar.cpp
# End Source File
# Begin Source File

SOURCE=.\MyToolBar.cpp
# End Source File
# Begin Source File

SOURCE=.\RemoteDtuSettingDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\RemoteDtuSettingPanel1.cpp
# End Source File
# Begin Source File

SOURCE=.\RemoteDtuSettingPanel2.cpp
# End Source File
# Begin Source File

SOURCE=.\RemoteDtuSettingPanel3.cpp
# End Source File
# Begin Source File

SOURCE=.\RemoteDtuSettingPanel4.cpp
# End Source File
# Begin Source File

SOURCE=.\SendDataDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SendSettingParamDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SettingDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SqlSettingDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SubclassWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\WordThread.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AddUserDlg1.h
# End Source File
# Begin Source File

SOURCE=.\BottomEditView.h
# End Source File
# Begin Source File

SOURCE=.\DateBase.h
# End Source File
# Begin Source File

SOURCE=.\DSC.h
# End Source File
# Begin Source File

SOURCE=.\DSCDoc.h
# End Source File
# Begin Source File

SOURCE=.\DSCView.h
# End Source File
# Begin Source File

SOURCE=.\GPRS_SMM.h
# End Source File
# Begin Source File

SOURCE=.\LeftView.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\MenuBar.h
# End Source File
# Begin Source File

SOURCE=.\MyToolBar.h
# End Source File
# Begin Source File

SOURCE=.\Optional.h
# End Source File
# Begin Source File

SOURCE=.\RemoteDtuSettingDlg.h
# End Source File
# Begin Source File

SOURCE=.\RemoteDtuSettingPanel1.h
# End Source File
# Begin Source File

SOURCE=.\RemoteDtuSettingPanel2.h
# End Source File
# Begin Source File

SOURCE=.\RemoteDtuSettingPanel3.h
# End Source File
# Begin Source File

SOURCE=.\RemoteDtuSettingPanel4.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SendDataDlg.h
# End Source File
# Begin Source File

SOURCE=.\SendSettingParamDlg.h
# End Source File
# Begin Source File

SOURCE=.\SettingDlg.h
# End Source File
# Begin Source File

SOURCE=.\SqlSettingDlg.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\SubclassWnd.h
# End Source File
# Begin Source File

SOURCE=.\wcomm_dll.h
# End Source File
# Begin Source File

SOURCE=.\WordThread.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\128_19.ico
# End Source File
# Begin Source File

SOURCE=.\res\app.ico
# End Source File
# Begin Source File

SOURCE=.\res\app_icon.ico
# End Source File
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\coldbar.BMP
# End Source File
# Begin Source File

SOURCE=.\res\coldbar22.BMP
# End Source File
# Begin Source File

SOURCE=.\res\coldbar26.BMP
# End Source File
# Begin Source File

SOURCE=.\res\DSC.ico
# End Source File
# Begin Source File

SOURCE=.\res\DSC.rc2
# End Source File
# Begin Source File

SOURCE=.\res\DSCDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\hotbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\hotbar22.BMP
# End Source File
# Begin Source File

SOURCE=.\res\hotbar26.BMP
# End Source File
# Begin Source File

SOURCE=.\res\ico00001.ico
# End Source File
# Begin Source File

SOURCE=.\res\ICON1.ICO
# End Source File
# Begin Source File

SOURCE=.\res\logo.bmp
# End Source File
# Begin Source File

SOURCE=.\res\norra.bmp
# End Source File
# Begin Source File

SOURCE=.\res\setting.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\treebmp.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=.\GPRS_SMM.lib
# End Source File
# Begin Source File

SOURCE=.\wcomm_dll.lib
# End Source File
# End Target
# End Project
