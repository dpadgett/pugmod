# Microsoft Developer Studio Project File - Name="ui" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=ui - Win32 Final
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ui.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ui.mak" CFG="ui - Win32 Final"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ui - Win32 Final" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ui - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ui - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ui - Win32 Debug(SH)" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ui - Win32 Final"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\../Final"
# PROP BASE Intermediate_Dir ".\../Final/ui"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\../Final"
# PROP Intermediate_Dir ".\../Final/ui"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /ML /W4 /Og /Ob2 /Oi /Op /Ot /G6 /GA /D "NDEBUG" /D "_USRDL" /D "_WINDOWS" /D "UI_EXPORTS" /D "MISSIONPACK" /D "WIN32" /D "_JK2" /D "FINAL_BUILD" /GF /Gy /YX /Fp".\../Final/ui/ui.pch" /Fo".\../Final/ui/" /Fd".\../Final/ui/" /c /GX 
# ADD CPP /nologo /ML /W4 /Og /Ob2 /Oi /Op /Ot /G6 /GA /D "NDEBUG" /D "_USRDL" /D "_WINDOWS" /D "UI_EXPORTS" /D "MISSIONPACK" /D "WIN32" /D "_JK2" /D "FINAL_BUILD" /GF /Gy /YX /Fp".\../Final/ui/ui.pch" /Fo".\../Final/ui/" /Fd".\../Final/ui/" /c /GX 
# ADD BASE MTL /nologo /D"NDebug" /mktyplib203 /tlb".\..\Final\ui.tlb" /win32 
# ADD MTL /nologo /D"NDebug" /mktyplib203 /tlb".\..\Final\ui.tlb" /win32 
# ADD BASE RSC /l 1033 /d "NDebug" 
# ADD RSC /l 1033 /d "NDebug" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo 
# ADD BSC32 /nologo 
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib odbc32.lib odbccp32.lib /nologo /dll /out:"..\Final\uix86.dll" /incremental:no /def:".\ui.def" /pdbtype:sept /map:".\..\Final\uix86.map" /subsystem:windows /opt:ref /opt:icf /release /base:"0x40000000" /implib:".\../Final/uix86.lib" /MACHINE:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib odbc32.lib odbccp32.lib /nologo /dll /out:"..\Final\uix86.dll" /incremental:no /def:".\ui.def" /pdbtype:sept /map:".\..\Final\uix86.map" /subsystem:windows /opt:ref /opt:icf /release /base:"0x40000000" /implib:".\../Final/uix86.lib" /MACHINE:I386

!ELSEIF  "$(CFG)" == "ui - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\../Debug"
# PROP BASE Intermediate_Dir ".\../Debug/ui"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\../Debug"
# PROP Intermediate_Dir ".\../Debug/ui"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MLd /ZI /W3 /Od /G6 /D "_DEBUG" /D "_USRDLL" /D "WIN32" /D "_WINDOWS" /D "UI_EXPORTS" /D "MISSIONPACK" /D "_JK2" /YX /Fp".\../Debug/ui/ui.pch" /Fo".\../Debug/ui/" /Fd".\../Debug/ui/" /FR /GZ /c /GX 
# ADD CPP /nologo /MLd /ZI /W3 /Od /G6 /D "_DEBUG" /D "_USRDLL" /D "WIN32" /D "_WINDOWS" /D "UI_EXPORTS" /D "MISSIONPACK" /D "_JK2" /YX /Fp".\../Debug/ui/ui.pch" /Fo".\../Debug/ui/" /Fd".\../Debug/ui/" /FR /GZ /c /GX 
# ADD BASE MTL /nologo /D"_Debug" /mktyplib203 /tlb".\..\Debug\ui.tlb" /win32 
# ADD MTL /nologo /D"_Debug" /mktyplib203 /tlb".\..\Debug\ui.tlb" /win32 
# ADD BASE RSC /l 1033 /d "_Debug" 
# ADD RSC /l 1033 /d "_Debug" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo 
# ADD BSC32 /nologo 
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib odbc32.lib odbccp32.lib /nologo /dll /out:"..\Debug\uix86.dll" /incremental:yes /def:".\ui.def" /debug /pdb:".\..\Debug\uix86.pdb" /pdbtype:sept /map:".\..\Debug\uix86.map" /subsystem:windows /base:"0x40000000" /implib:".\../Debug/uix86.lib" /MACHINE:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib odbc32.lib odbccp32.lib /nologo /dll /out:"..\Debug\uix86.dll" /incremental:yes /def:".\ui.def" /debug /pdb:".\..\Debug\uix86.pdb" /pdbtype:sept /map:".\..\Debug\uix86.map" /subsystem:windows /base:"0x40000000" /implib:".\../Debug/uix86.lib" /MACHINE:I386

!ELSEIF  "$(CFG)" == "ui - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\../Release"
# PROP BASE Intermediate_Dir ".\../Release/ui"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\../Release"
# PROP Intermediate_Dir ".\../Release/ui"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /ML /Zi /W4 /Og /Ob2 /Oi /Op /Ot /G6 /GA /D "NDEBUG" /D "_USRDL" /D "WIN32" /D "_WINDOWS" /D "UI_EXPORTS" /D "MISSIONPACK" /D "_JK2" /GF /Gm /Gy /YX /Fp".\../Release/ui/ui.pch" /Fo".\../Release/ui/" /Fd".\../Release/ui/" /c /GX 
# ADD CPP /nologo /ML /Zi /W4 /Og /Ob2 /Oi /Op /Ot /G6 /GA /D "NDEBUG" /D "_USRDL" /D "WIN32" /D "_WINDOWS" /D "UI_EXPORTS" /D "MISSIONPACK" /D "_JK2" /GF /Gm /Gy /YX /Fp".\../Release/ui/ui.pch" /Fo".\../Release/ui/" /Fd".\../Release/ui/" /c /GX 
# ADD BASE MTL /nologo /D"NDebug" /mktyplib203 /tlb".\..\Release\ui.tlb" /win32 
# ADD MTL /nologo /D"NDebug" /mktyplib203 /tlb".\..\Release\ui.tlb" /win32 
# ADD BASE RSC /l 1033 /d "NDebug" 
# ADD RSC /l 1033 /d "NDebug" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo 
# ADD BSC32 /nologo 
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib odbc32.lib odbccp32.lib /nologo /dll /out:"..\Release\uix86.dll" /incremental:no /def:".\ui.def" /debug /pdb:".\..\Release\uix86.pdb" /pdbtype:sept /map:".\..\Release\uix86.map" /subsystem:windows /opt:ref /opt:icf /release /base:"0x40000000" /implib:".\../Release/uix86.lib" /MACHINE:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib odbc32.lib odbccp32.lib /nologo /dll /out:"..\Release\uix86.dll" /incremental:no /def:".\ui.def" /debug /pdb:".\..\Release\uix86.pdb" /pdbtype:sept /map:".\..\Release\uix86.map" /subsystem:windows /opt:ref /opt:icf /release /base:"0x40000000" /implib:".\../Release/uix86.lib" /MACHINE:I386

!ELSEIF  "$(CFG)" == "ui - Win32 Debug(SH)"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug(SH)"
# PROP BASE Intermediate_Dir "Debug(SH)"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug(SH)"
# PROP Intermediate_Dir "Debug(SH)"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /ZI /W3 /Od /G6 /D "_DEBUG" /D "_USRDLL" /D "WIN32" /D "_WINDOWS" /D "UI_EXPORTS" /D "MISSIONPACK" /D "_JK2" /YX /Fp".\../Debug/ui/ui.pch" /Fo".\../Debug/ui/" /Fd".\../Debug/ui/" /FR /GZ /c /GX 
# ADD CPP /nologo /MTd /ZI /W3 /Od /G6 /D "_DEBUG" /D "_USRDLL" /D "WIN32" /D "_WINDOWS" /D "UI_EXPORTS" /D "MISSIONPACK" /D "_JK2" /YX /Fp".\../Debug/ui/ui.pch" /Fo".\../Debug/ui/" /Fd".\../Debug/ui/" /FR /GZ /c /GX 
# ADD BASE MTL /nologo /D"_Debug" /mktyplib203 /tlb".\..\Debug\ui.tlb" /win32 
# ADD MTL /nologo /D"_Debug" /mktyplib203 /tlb".\..\Debug\ui.tlb" /win32 
# ADD BASE RSC /l 1033 /d "_Debug" 
# ADD RSC /l 1033 /d "_Debug" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo 
# ADD BSC32 /nologo 
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib odbc32.lib odbccp32.lib /nologo /dll /out:"..\Debug\uix86.dll" /incremental:yes /def:".\ui.def" /debug /pdb:".\..\Debug\uix86.pdb" /pdbtype:sept /map:".\..\Debug\uix86.map" /subsystem:windows /base:"0x40000000" /implib:".\../Debug/uix86.lib" /MACHINE:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib odbc32.lib odbccp32.lib /nologo /dll /out:"..\Debug\uix86.dll" /incremental:yes /def:".\ui.def" /debug /pdb:".\..\Debug\uix86.pdb" /pdbtype:sept /map:".\..\Debug\uix86.map" /subsystem:windows /base:"0x40000000" /implib:".\../Debug/uix86.lib" /MACHINE:I386

!ENDIF

# Begin Target

# Name "ui - Win32 Final"
# Name "ui - Win32 Debug"
# Name "ui - Win32 Release"
# Name "ui - Win32 Debug(SH)"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\game\bg_lib.c

!IF  "$(CFG)" == "ui - Win32 Final"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ui - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ui - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ui - Win32 Debug(SH)"

# PROP Exclude_From_Build 1

!ENDIF

# End Source File
# Begin Source File

SOURCE=..\game\bg_misc.c
# End Source File
# Begin Source File

SOURCE=..\game\bg_saga.c
# End Source File
# Begin Source File

SOURCE=..\game\bg_vehicleLoad.c
# End Source File
# Begin Source File

SOURCE=..\game\bg_weapons.c
# End Source File
# Begin Source File

SOURCE=..\game\q_math.c
# End Source File
# Begin Source File

SOURCE=..\game\q_shared.c
# End Source File
# Begin Source File

SOURCE=.\ui_atoms.c
# End Source File
# Begin Source File

SOURCE=.\ui_force.c
# End Source File
# Begin Source File

SOURCE=.\ui_gameinfo.c
# End Source File
# Begin Source File

SOURCE=.\ui_main.c
# End Source File
# Begin Source File

SOURCE=ui_saber.c
# End Source File
# Begin Source File

SOURCE=.\ui_shared.c
# End Source File
# Begin Source File

SOURCE=.\ui_syscalls.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\game\anims.h
# End Source File
# Begin Source File

SOURCE=..\game\bg_local.h
# End Source File
# Begin Source File

SOURCE=..\game\bg_public.h
# End Source File
# Begin Source File

SOURCE=..\game\bg_saga.h
# End Source File
# Begin Source File

SOURCE=..\game\bg_strap.h
# End Source File
# Begin Source File

SOURCE=..\game\bg_weapons.h
# End Source File
# Begin Source File

SOURCE=..\qcommon\disablewarnings.h
# End Source File
# Begin Source File

SOURCE=..\qcommon\game_version.h
# End Source File
# Begin Source File

SOURCE=.\keycodes.h
# End Source File
# Begin Source File

SOURCE=..\..\ui\menudef.h
# End Source File
# Begin Source File

SOURCE=..\game\q_shared.h
# End Source File
# Begin Source File

SOURCE=..\qcommon\qfiles.h
# End Source File
# Begin Source File

SOURCE=..\game\surfaceflags.h
# End Source File
# Begin Source File

SOURCE=..\qcommon\tags.h
# End Source File
# Begin Source File

SOURCE=..\cgame\tr_types.h
# End Source File
# Begin Source File

SOURCE=.\ui.def
# End Source File
# Begin Source File

SOURCE=.\ui_force.h
# End Source File
# Begin Source File

SOURCE=.\ui_local.h
# End Source File
# Begin Source File

SOURCE=.\ui_public.h
# End Source File
# Begin Source File

SOURCE=.\ui_shared.h
# End Source File
# End Group
# Begin Group "win32"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\win32\ui.rc
# End Source File
# End Group
# End Target
# End Project

