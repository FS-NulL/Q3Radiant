# Microsoft Developer Studio Project File - Name="Radiant" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 5.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Radiant - Win32 Q3Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Radiant.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Radiant.mak" CFG="Radiant - Win32 Q3Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Radiant - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Radiant - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "Radiant - Win32 Q3Release" (based on "Win32 (x86) Application")
!MESSAGE "Radiant - Win32 Q3Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Radiant - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GR /GX /Zd /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o NUL /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o NUL /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386 /nodefaultlib:"libc.lib msvcrtd.lib"
# SUBTRACT LINK32 /debug

!ELSEIF  "$(CFG)" == "Radiant - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MTd /W3 /Gm /GR /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o NUL /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o NUL /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"libc.lib msvcrtd.lib" /pdbtype:sept
# SUBTRACT LINK32 /nodefaultlib

!ELSEIF  "$(CFG)" == "Radiant - Win32 Q3Release"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Radiant1"
# PROP BASE Intermediate_Dir "Radiant1"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Q3Release"
# PROP Intermediate_Dir "Q3Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GR /GX /Zd /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GR /GX /Zd /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "QUAKE3" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o NUL /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o NUL /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 opengl32.lib glu32.lib /nologo /subsystem:windows /machine:I386 /nodefaultlib:"libc.lib msvcrtd.lib"
# SUBTRACT BASE LINK32 /debug
# ADD LINK32 /nologo /subsystem:windows /machine:I386 /nodefaultlib:"libc.lib msvcrtd.lib" /out:"f:\program files\qeradiant\q3radiant.exe"
# SUBTRACT LINK32 /debug

!ELSEIF  "$(CFG)" == "Radiant - Win32 Q3Debug"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Radiant2"
# PROP BASE Intermediate_Dir "Radiant2"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Q3Debug"
# PROP Intermediate_Dir "Q3Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GR /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MTd /W3 /Gm /GR /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "QUAKE3" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o NUL /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o NUL /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 opengl32.lib glu32.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"libc.lib msvcrtd.lib" /pdbtype:sept
# SUBTRACT BASE LINK32 /nodefaultlib
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"libc.lib msvcrtd.lib" /out:"f:\program files\qeradiant\q3radiant.exe" /pdbtype:sept
# SUBTRACT LINK32 /nodefaultlib

!ENDIF 

# Begin Target

# Name "Radiant - Win32 Release"
# Name "Radiant - Win32 Debug"
# Name "Radiant - Win32 Q3Release"
# Name "Radiant - Win32 Q3Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "QE4 Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Bmp.cpp
# End Source File
# Begin Source File

SOURCE=.\brush.cpp
# End Source File
# Begin Source File

SOURCE=.\cmdlib.cpp
# End Source File
# Begin Source File

SOURCE=.\csg.cpp
# End Source File
# Begin Source File

SOURCE=.\drag.cpp
# End Source File
# Begin Source File

SOURCE=.\eclass.cpp
# End Source File
# Begin Source File

SOURCE=.\entity.cpp
# End Source File
# Begin Source File

SOURCE=.\lbmlib.cpp
# End Source File
# Begin Source File

SOURCE=.\map.cpp
# End Source File
# Begin Source File

SOURCE=.\mathlib.cpp
# End Source File
# Begin Source File

SOURCE=.\mru.cpp
# End Source File
# Begin Source File

SOURCE=.\parse.cpp
# End Source File
# Begin Source File

SOURCE=.\points.cpp
# End Source File
# Begin Source File

SOURCE=.\qe3.cpp
# End Source File
# Begin Source File

SOURCE=.\select.cpp
# End Source File
# Begin Source File

SOURCE=.\vertsel.cpp
# End Source File
# Begin Source File

SOURCE=.\win_dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\win_ent.cpp
# End Source File
# Begin Source File

SOURCE=.\win_main.cpp
# End Source File
# Begin Source File

SOURCE=.\win_qe3.cpp
# End Source File
# Begin Source File

SOURCE=.\z.cpp
# End Source File
# End Group
# Begin Group "From others"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\fnmatch.cpp
# End Source File
# Begin Source File

SOURCE=.\inc.cpp
# End Source File
# End Group
# Begin Group "Quake 3"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Quake3\cbrush.cpp

!IF  "$(CFG)" == "Radiant - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Radiant - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Radiant - Win32 Q3Release"

!ELSEIF  "$(CFG)" == "Radiant - Win32 Q3Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Quake3\pmesh.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\BrushScript.cpp
# End Source File
# Begin Source File

SOURCE=.\BSInput.cpp
# End Source File
# Begin Source File

SOURCE=.\CamWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\CapDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\cbrushstub.cpp

!IF  "$(CFG)" == "Radiant - Win32 Release"

!ELSEIF  "$(CFG)" == "Radiant - Win32 Debug"

!ELSEIF  "$(CFG)" == "Radiant - Win32 Q3Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Radiant - Win32 Q3Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CommandsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DialogInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\DialogTextures.cpp
# End Source File
# Begin Source File

SOURCE=.\DialogThick.cpp
# End Source File
# Begin Source File

SOURCE=.\EditWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\EntityListDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\FindTextureDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\LstToolBar.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\MapInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\NewProjDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PatchDensityDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PrefsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\qgl_win.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\radbsp.cpp
# End Source File
# Begin Source File

SOURCE=.\RADEditWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\Radiant.cpp
# End Source File
# Begin Source File

SOURCE=.\Radiant.rc

!IF  "$(CFG)" == "Radiant - Win32 Release"

!ELSEIF  "$(CFG)" == "Radiant - Win32 Debug"

!ELSEIF  "$(CFG)" == "Radiant - Win32 Q3Release"

!ELSEIF  "$(CFG)" == "Radiant - Win32 Q3Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\RotateDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ScaleDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\ScriptDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SurfaceDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TexEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\TextureBar.cpp
# End Source File
# Begin Source File

SOURCE=.\TextureLayout.cpp
# End Source File
# Begin Source File

SOURCE=.\TexWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\XYWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\ZWnd.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\3DFXCamWnd.h
# End Source File
# Begin Source File

SOURCE=.\BSInput.h
# End Source File
# Begin Source File

SOURCE=.\CamWnd.h
# End Source File
# Begin Source File

SOURCE=.\CapDialog.h
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.h
# End Source File
# Begin Source File

SOURCE=.\CommandsDlg.h
# End Source File
# Begin Source File

SOURCE=.\DialogInfo.h
# End Source File
# Begin Source File

SOURCE=.\DialogTextures.h
# End Source File
# Begin Source File

SOURCE=.\DialogThick.h
# End Source File
# Begin Source File

SOURCE=.\EditWnd.h
# End Source File
# Begin Source File

SOURCE=.\EntityListDlg.h
# End Source File
# Begin Source File

SOURCE=.\LstToolBar.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\MapInfo.h
# End Source File
# Begin Source File

SOURCE=.\NewProjDlg.h
# End Source File
# Begin Source File

SOURCE=.\PatchDensityDlg.h
# End Source File
# Begin Source File

SOURCE=.\PrefsDlg.h
# End Source File
# Begin Source File

SOURCE=.\qe3.h
# End Source File
# Begin Source File

SOURCE=.\RADEditWnd.h
# End Source File
# Begin Source File

SOURCE=.\Radiant.h
# End Source File
# Begin Source File

SOURCE=.\RadiantDoc.h
# End Source File
# Begin Source File

SOURCE=.\RadiantView.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\RotateDlg.h
# End Source File
# Begin Source File

SOURCE=.\ScaleDialog.h
# End Source File
# Begin Source File

SOURCE=.\ScriptDlg.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\SurfaceDlg.h
# End Source File
# Begin Source File

SOURCE=.\TexEdit.h
# End Source File
# Begin Source File

SOURCE=.\TextureBar.h
# End Source File
# Begin Source File

SOURCE=.\TextureLayout.h
# End Source File
# Begin Source File

SOURCE=.\TexWnd.h
# End Source File
# Begin Source File

SOURCE=.\XYWnd.h
# End Source File
# Begin Source File

SOURCE=.\ZWnd.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\bevel.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00002.bmp
# End Source File
# Begin Source File

SOURCE=.\res\endcap.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ibevel.bmp
# End Source File
# Begin Source File

SOURCE=.\res\iendcap.bmp
# End Source File
# Begin Source File

SOURCE=.\res\logo_sm3dfx.bmp
# End Source File
# Begin Source File

SOURCE=.\q.bmp
# End Source File
# Begin Source File

SOURCE=.\res\q.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Radiant.ico
# End Source File
# Begin Source File

SOURCE=.\res\Radiant.rc2
# End Source File
# Begin Source File

SOURCE=.\res\RadiantDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar1.bmp
# End Source File
# Begin Source File

SOURCE=.\toolbar1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\viewdefa.bmp
# End Source File
# Begin Source File

SOURCE=.\res\viewoppo.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
