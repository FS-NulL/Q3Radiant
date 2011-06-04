# Microsoft Developer Studio Project File - Name="Radiant" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Radiant - Win32 DebugTTimo
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Radiant.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Radiant.mak" CFG="Radiant - Win32 DebugTTimo"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Radiant - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Radiant - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "Radiant - Win32 DebugTTimo" (based on "Win32 (x86) Application")
!MESSAGE "Radiant - Win32 ReleaseTTimo" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/source/q3radiant", FEFAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Radiant - Win32 Release"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Radiant___Win32_Release"
# PROP BASE Intermediate_Dir "Radiant___Win32_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=df.exe
# ADD BASE F90 /include:"Radiant___Win32_Release/"
# ADD F90 /include:"Release/"
# ADD BASE CPP /nologo /MT /W3 /GR /GX /Zd /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "QUAKE3" /D "Q3RADIANT" /Yu"stdafx.h" /FD /c
# SUBTRACT BASE CPP /Fr
# ADD CPP /nologo /MT /W3 /GR /GX /Zd /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "QUAKE3" /D "Q3RADIANT" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o /win32 "NUL"
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o /win32 "NUL"
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 winmm.lib jpeg6.lib pak.lib cmdlib.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"libc.lib msvcrtd.lib" /out:"d:\quake3\q3radiant\q3radiant.exe"
# SUBTRACT BASE LINK32 /incremental:yes
# ADD LINK32 jpeg6.lib pak.lib cmdlib.lib winmm.lib shlwapi.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"libc.lib msvcrtd.lib" /out:"D:\Program Files\Quake III Arena\Tools\q3radiant.exe"
# SUBTRACT LINK32 /incremental:yes

!ELSEIF  "$(CFG)" == "Radiant - Win32 Debug"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Radiant___Win32_Debug"
# PROP BASE Intermediate_Dir "Radiant___Win32_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=df.exe
# ADD BASE F90 /include:"Radiant___Win32_Debug/"
# ADD F90 /include:"Debug/"
# ADD BASE CPP /nologo /MTd /W3 /GR /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "QUAKE3" /D "Q3RADIANT" /FR /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MTd /W3 /GR /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "QUAKE3" /D "Q3RADIANT" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o /win32 "NUL"
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o /win32 "NUL"
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 winmm.lib jpeg6.lib pak.lib cmdlib.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"libc.lib msvcrtd.lib" /out:"d:\quake3\q3radiant\q3radiant.exe" /pdbtype:sept
# SUBTRACT BASE LINK32 /nodefaultlib
# ADD LINK32 jpeg6d.lib pakd.lib cmdlibd.lib winmm.lib shlwapi.lib /nologo /subsystem:windows /map /debug /machine:I386 /nodefaultlib:"libc.lib msvcrtd.lib" /out:"D:\Program Files\Quake III Arena\Tools\q3radiant.exe" /pdbtype:sept
# SUBTRACT LINK32 /nodefaultlib

!ELSEIF  "$(CFG)" == "Radiant - Win32 DebugTTimo"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Radiant___Win32_DebugTTimo"
# PROP BASE Intermediate_Dir "Radiant___Win32_DebugTTimo"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Radiant___Win32_DebugTTimo"
# PROP Intermediate_Dir "Radiant___Win32_DebugTTimo"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=df.exe
# ADD BASE F90 /include:"Radiant___Win32_DebugTTimo/"
# ADD F90 /browser /include:"Radiant___Win32_DebugTTimo/"
# ADD BASE CPP /nologo /MTd /W3 /GR /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "QUAKE3" /D "Q3RADIANT" /FR /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MTd /W3 /GR /GX /ZI /Od /I "..\Libs" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "QUAKE3" /D "Q3RADIANT" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o /win32 "NUL"
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o /win32 "NUL"
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 winmm.lib jpeg6d.lib pakd.lib cmdlibd.lib /nologo /subsystem:windows /map /debug /machine:I386 /nodefaultlib:"libc.lib msvcrtd.lib" /out:"D:\Program Files\Quake III Arena\Tools\q3radiant.exe" /pdbtype:sept
# SUBTRACT BASE LINK32 /nodefaultlib
# ADD LINK32 jpeg6d.lib pakd.lib cmdlibd.lib winmm.lib shlwapi.lib /nologo /subsystem:windows /map /debug /machine:I386 /nodefaultlib:"libc.lib msvcrtd.lib" /out:"Radiant___Win32_DebugTTimo\q3radiant.exe" /libpath:"..\Libs"
# SUBTRACT LINK32 /profile /nodefaultlib
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Desc=Copy to dirs...
PostBuild_Cmds=copy Radiant___Win32_DebugTTimo\q3radiant.exe $(QUAKE3RADIANTDIR)	rem copy Radiant___Win32_DebugTTimo\q3radiant.exe c:\home\Q3RadiantSetup\WorkZone\quake\tools
# End Special Build Tool

!ELSEIF  "$(CFG)" == "Radiant - Win32 ReleaseTTimo"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Radiant___Win32_ReleaseTTimo0"
# PROP BASE Intermediate_Dir "Radiant___Win32_ReleaseTTimo0"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Radiant___Win32_ReleaseTTimo"
# PROP Intermediate_Dir "Radiant___Win32_ReleaseTTimo"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=df.exe
# ADD BASE F90 /include:"Radiant___Win32_ReleaseTTimo0/"
# ADD F90 /include:"Radiant___Win32_ReleaseTTimo/"
# ADD BASE CPP /nologo /MT /W3 /GR /GX /Zd /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "QUAKE3" /D "Q3RADIANT" /Yu"stdafx.h" /FD /c
# SUBTRACT BASE CPP /Fr
# ADD CPP /nologo /MT /W3 /GR /GX /Zd /O2 /I "..\Libs" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "QUAKE3" /D "Q3RADIANT" /FA /Fr /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o /win32 "NUL"
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o /win32 "NUL"
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 winmm.lib jpeg6.lib pak.lib cmdlib.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"libc.lib msvcrtd.lib" /out:"D:\Program Files\Quake III Arena\Tools\q3radiant.exe"
# SUBTRACT BASE LINK32 /incremental:yes
# ADD LINK32 jpeg6.lib pak.lib cmdlib.lib winmm.lib shlwapi.lib /nologo /subsystem:windows /map /debug /machine:I386 /nodefaultlib:"libc.lib msvcrtd.lib" /out:"Radiant___Win32_ReleaseTTimo\q3radiant.exe" /libpath:"..\Libs"
# SUBTRACT LINK32 /profile
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Desc=Copy to dir...
PostBuild_Cmds=copy Radiant___Win32_ReleaseTTimo\q3radiant.exe $(QUAKE3RADIANTDIR)\Q3Radiant_202.exe	copy Radiant___Win32_ReleaseTTimo\q3radiant.exe c:\home\Q3RadiantSetup\WorkZone\quake\tools\Q3Radiant_202.exe	copy Radiant___Win32_ReleaseTTimo\q3radiant.map $(QUAKE3RADIANTDIR)	rem copy Radiant___Win32_ReleaseTTimo\q3radiant.exe e:\home\quake3\tools\Q3Radiant_202b3.exe
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "Radiant - Win32 Release"
# Name "Radiant - Win32 Debug"
# Name "Radiant - Win32 DebugTTimo"
# Name "Radiant - Win32 ReleaseTTimo"
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
# Begin Group "Brush Primitives"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\brush_primit.cpp
# End Source File
# End Group
# Begin Group "interfaces"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\GLInterface.cpp
# End Source File
# Begin Source File

SOURCE=.\IEpairs.cpp
# End Source File
# Begin Source File

SOURCE=.\IShaders.cpp
# End Source File
# Begin Source File

SOURCE=.\Messaging.cpp
# End Source File
# Begin Source File

SOURCE=.\PluginEntities.cpp
# End Source File
# Begin Source File

SOURCE=.\SelectedFace.cpp
# End Source File
# Begin Source File

SOURCE=.\SurfacePlugin.cpp
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
# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
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

SOURCE=.\GroupDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\LightPrompt.cpp
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

SOURCE=.\NameDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\NewProjDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PatchDensityDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PatchDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\PlugIn.cpp

!IF  "$(CFG)" == "Radiant - Win32 Release"

# ADD BASE CPP /Od
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /Od
# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "Radiant - Win32 Debug"

!ELSEIF  "$(CFG)" == "Radiant - Win32 DebugTTimo"

!ELSEIF  "$(CFG)" == "Radiant - Win32 ReleaseTTimo"

# ADD BASE CPP /Od
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /Od
# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\PlugInManager.cpp

!IF  "$(CFG)" == "Radiant - Win32 Release"

# ADD BASE CPP /Od
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /Od
# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "Radiant - Win32 Debug"

!ELSEIF  "$(CFG)" == "Radiant - Win32 DebugTTimo"

!ELSEIF  "$(CFG)" == "Radiant - Win32 ReleaseTTimo"

# ADD BASE CPP /Od
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /Od
# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\PMESH.CPP
# End Source File
# Begin Source File

SOURCE=.\PrefsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\qgl_win.c
# SUBTRACT BASE CPP /YX /Yc /Yu
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

SOURCE=.\ShaderInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD BASE CPP /Yc"stdafx.h"
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

SOURCE=.\Undo.cpp
# End Source File
# Begin Source File

SOURCE=.\WaveOpen.cpp
# End Source File
# Begin Source File

SOURCE=.\Winding.cpp
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
# Begin Group "interface headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\IEpairs.h
# End Source File
# Begin Source File

SOURCE=.\igl.h
# End Source File
# Begin Source File

SOURCE=.\IMessaging.h
# End Source File
# Begin Source File

SOURCE=.\IPluginEntities.h
# End Source File
# Begin Source File

SOURCE=.\ISelectedFace.h
# End Source File
# Begin Source File

SOURCE=.\IShaders.h
# End Source File
# Begin Source File

SOURCE=.\isurfaceplugin.h
# End Source File
# End Group
# Begin Group "internal interface headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\EpairsWrapper.h
# End Source File
# Begin Source File

SOURCE=.\Messaging.h
# End Source File
# Begin Source File

SOURCE=.\Shaders.h
# End Source File
# End Group
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

SOURCE=.\FindTextureDlg.h
# End Source File
# Begin Source File

SOURCE=.\GroupBar.h
# End Source File
# Begin Source File

SOURCE=.\GroupDlg.h
# End Source File
# Begin Source File

SOURCE=.\LightPrompt.h
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

SOURCE=.\NameDlg.h
# End Source File
# Begin Source File

SOURCE=.\NewProjDlg.h
# End Source File
# Begin Source File

SOURCE=.\PatchDensityDlg.h
# End Source File
# Begin Source File

SOURCE=.\PatchDialog.h
# End Source File
# Begin Source File

SOURCE=.\PlugIn.h
# End Source File
# Begin Source File

SOURCE=.\PlugInManager.h
# End Source File
# Begin Source File

SOURCE=.\PrefsDlg.h
# End Source File
# Begin Source File

SOURCE=.\qe3.h
# End Source File
# Begin Source File

SOURCE=.\qerplugin.h
# End Source File
# Begin Source File

SOURCE=.\QERTYPES.H
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

SOURCE=.\ShaderInfo.h
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

SOURCE=.\WaveOpen.h
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

SOURCE=.\atuer.bmp
# End Source File
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

SOURCE=.\RES\bmp00003.bmp
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

SOURCE=.\RES\shaderbar.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\shaderdoc.ico
# End Source File
# Begin Source File

SOURCE=.\RES\shaderframe.ico
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
# End Target
# End Project
