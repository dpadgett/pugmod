# Microsoft Developer Studio Project File - Name="JK2cgame" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=JK2cgame - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "JK2cgame.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "JK2cgame.mak" CFG="JK2cgame - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "JK2cgame - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "JK2cgame - Win32 Final" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "JK2cgame - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "JK2cgame - Win32 Debug(SH)" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "JK2cgame - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\../Release"
# PROP BASE Intermediate_Dir ".\../Release/JK2cgame"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\../Release"
# PROP Intermediate_Dir ".\../Release/JK2cgame"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /ML /I ".." /I "./../game" /Zi /W4 /Og /Ob2 /Oi /Op /Ot /G6 /GA /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "MISSIONPACK" /D "_JK2" /D "CGAME" /GF /Gm /Gy /YX /Fp".\../Release/JK2cgame/JK2_cgame.pch" /Fo".\../Release/JK2cgame/" /Fd".\../Release/JK2cgame/" /c /GX 
# ADD CPP /nologo /ML /I ".." /I "./../game" /Zi /W4 /Og /Ob2 /Oi /Op /Ot /G6 /GA /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "MISSIONPACK" /D "_JK2" /D "CGAME" /GF /Gm /Gy /YX /Fp".\../Release/JK2cgame/JK2_cgame.pch" /Fo".\../Release/JK2cgame/" /Fd".\../Release/JK2cgame/" /c /GX 
# ADD BASE MTL /nologo /D"NDebug" /mktyplib203 /tlb".\..\Release\JK2_cgame.tlb" /win32 
# ADD MTL /nologo /D"NDebug" /mktyplib203 /tlb".\..\Release\JK2_cgame.tlb" /win32 
# ADD BASE RSC /l 1033 /d "NDebug" 
# ADD RSC /l 1033 /d "NDebug" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo 
# ADD BSC32 /nologo 
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib odbc32.lib odbccp32.lib /nologo /dll /out:"..\Release\cgamex86.dll" /incremental:no /def:".\JK2_cgame.def" /debug /pdb:".\..\Release\cgamex86.pdb" /pdbtype:sept /map:"..\Release\cgamex86.map" /subsystem:windows /opt:ref /opt:icf /release /base:"0x30000000" /implib:".\../Release/cgamex86.lib" /MACHINE:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib odbc32.lib odbccp32.lib /nologo /dll /out:"..\Release\cgamex86.dll" /incremental:no /def:".\JK2_cgame.def" /debug /pdb:".\..\Release\cgamex86.pdb" /pdbtype:sept /map:"..\Release\cgamex86.map" /subsystem:windows /opt:ref /opt:icf /release /base:"0x30000000" /implib:".\../Release/cgamex86.lib" /MACHINE:I386

!ELSEIF  "$(CFG)" == "JK2cgame - Win32 Final"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\../Final"
# PROP BASE Intermediate_Dir ".\../Final/JK2cgame"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\../Final"
# PROP Intermediate_Dir ".\../Final/JK2cgame"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /ML /I ".." /I "./../game" /W4 /Og /Ob2 /Oi /Op /Ot /G6 /GA /D "NDEBUG" /D "_WINDOWS" /D "MISSIONPACK" /D "WIN32" /D "CGAME" /D "_JK2" /D "FINAL_BUILD" /GF /Gy /YX /Fp".\../Final/JK2cgame/JK2_cgame.pch" /Fo".\../Final/JK2cgame/" /Fd".\../Final/JK2cgame/" /c /GX 
# ADD CPP /nologo /ML /I ".." /I "./../game" /W4 /Og /Ob2 /Oi /Op /Ot /G6 /GA /D "NDEBUG" /D "_WINDOWS" /D "MISSIONPACK" /D "WIN32" /D "CGAME" /D "_JK2" /D "FINAL_BUILD" /GF /Gy /YX /Fp".\../Final/JK2cgame/JK2_cgame.pch" /Fo".\../Final/JK2cgame/" /Fd".\../Final/JK2cgame/" /c /GX 
# ADD BASE MTL /nologo /D"NDebug" /mktyplib203 /tlb".\..\Final\JK2_cgame.tlb" /win32 
# ADD MTL /nologo /D"NDebug" /mktyplib203 /tlb".\..\Final\JK2_cgame.tlb" /win32 
# ADD BASE RSC /l 1033 /d "NDebug" 
# ADD RSC /l 1033 /d "NDebug" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo 
# ADD BSC32 /nologo 
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib odbc32.lib odbccp32.lib /nologo /dll /out:"..\Final\cgamex86.dll" /incremental:no /def:".\JK2_cgame.def" /pdbtype:sept /map:".\..\Final\cgamex86.map" /subsystem:windows /opt:ref /opt:icf /release /base:"0x30000000" /implib:".\../Final/cgamex86.lib" /MACHINE:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib odbc32.lib odbccp32.lib /nologo /dll /out:"..\Final\cgamex86.dll" /incremental:no /def:".\JK2_cgame.def" /pdbtype:sept /map:".\..\Final\cgamex86.map" /subsystem:windows /opt:ref /opt:icf /release /base:"0x30000000" /implib:".\../Final/cgamex86.lib" /MACHINE:I386

!ELSEIF  "$(CFG)" == "JK2cgame - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\../Debug"
# PROP BASE Intermediate_Dir ".\../Debug/JK2cgame"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\../Debug"
# PROP Intermediate_Dir ".\../Debug/JK2cgame"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MLd /I ".." /I "./../game" /ZI /W3 /Od /G6 /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "MISSIONPACK" /D "CGAME" /D "_JK2" /YX /Fp".\../Debug/JK2cgame/JK2_cgame.pch" /Fo".\../Debug/JK2cgame/" /Fd".\../Debug/JK2cgame/" /FR /GZ /c /GX 
# ADD CPP /nologo /MLd /I ".." /I "./../game" /ZI /W3 /Od /G6 /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "MISSIONPACK" /D "CGAME" /D "_JK2" /YX /Fp".\../Debug/JK2cgame/JK2_cgame.pch" /Fo".\../Debug/JK2cgame/" /Fd".\../Debug/JK2cgame/" /FR /GZ /c /GX 
# ADD BASE MTL /nologo /D"_Debug" /mktyplib203 /tlb".\..\Debug\JK2_cgame.tlb" /win32 
# ADD MTL /nologo /D"_Debug" /mktyplib203 /tlb".\..\Debug\JK2_cgame.tlb" /win32 
# ADD BASE RSC /l 1033 /d "_Debug" 
# ADD RSC /l 1033 /d "_Debug" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo 
# ADD BSC32 /nologo 
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib odbc32.lib odbccp32.lib /nologo /dll /out:"..\Debug\cgamex86.dll" /incremental:yes /def:".\JK2_cgame.def" /debug /pdb:".\..\Debug\cgamex86.pdb" /pdbtype:sept /map:".\..\Debug\cgamex86.map" /subsystem:windows /base:"0x30000000" /implib:".\../Debug/cgamex86.lib" /MACHINE:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib odbc32.lib odbccp32.lib /nologo /dll /out:"..\Debug\cgamex86.dll" /incremental:yes /def:".\JK2_cgame.def" /debug /pdb:".\..\Debug\cgamex86.pdb" /pdbtype:sept /map:".\..\Debug\cgamex86.map" /subsystem:windows /base:"0x30000000" /implib:".\../Debug/cgamex86.lib" /MACHINE:I386

!ELSEIF  "$(CFG)" == "JK2cgame - Win32 Debug(SH)"

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
# ADD BASE CPP /nologo /MLd /I ".." /I "./../game" /ZI /W3 /Od /G6 /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "MISSIONPACK" /D "CGAME" /D "_JK2" /D "JK2AWARDS" /YX /Fp".\../Debug/JK2cgame/JK2_cgame.pch" /Fo".\../Debug/JK2cgame/" /Fd".\../Debug/JK2cgame/" /FR /GZ /c /GX 
# ADD CPP /nologo /MLd /I ".." /I "./../game" /ZI /W3 /Od /G6 /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "MISSIONPACK" /D "CGAME" /D "_JK2" /D "JK2AWARDS" /YX /Fp".\../Debug/JK2cgame/JK2_cgame.pch" /Fo".\../Debug/JK2cgame/" /Fd".\../Debug/JK2cgame/" /FR /GZ /c /GX 
# ADD BASE MTL /nologo /D"_Debug" /mktyplib203 /tlb".\..\Debug\JK2_cgame.tlb" /win32 
# ADD MTL /nologo /D"_Debug" /mktyplib203 /tlb".\..\Debug\JK2_cgame.tlb" /win32 
# ADD BASE RSC /l 1033 /d "_Debug" 
# ADD RSC /l 1033 /d "_Debug" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo 
# ADD BSC32 /nologo 
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib odbc32.lib odbccp32.lib /nologo /dll /out:"..\Debug\cgamex86.dll" /incremental:yes /def:".\JK2_cgame.def" /debug /pdb:".\..\Debug\cgamex86.pdb" /pdbtype:sept /map:".\..\Debug\JK2cgame\cgamex86.map" /subsystem:windows /base:"0x30000000" /implib:".\../Debug/cgamex86.lib" /MACHINE:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib odbc32.lib odbccp32.lib /nologo /dll /out:"..\Debug\cgamex86.dll" /incremental:yes /def:".\JK2_cgame.def" /debug /pdb:".\..\Debug\cgamex86.pdb" /pdbtype:sept /map:".\..\Debug\JK2cgame\cgamex86.map" /subsystem:windows /base:"0x30000000" /implib:".\../Debug/cgamex86.lib" /MACHINE:I386

!ENDIF

# Begin Target

# Name "JK2cgame - Win32 Release"
# Name "JK2cgame - Win32 Final"
# Name "JK2cgame - Win32 Debug"
# Name "JK2cgame - Win32 Debug(SH)"
# Begin Group "Source Files"

# PROP Default_Filter "c"
# Begin Source File

SOURCE=..\game\AnimalNPC.c
# End Source File
# Begin Source File

SOURCE=..\game\FighterNPC.c
# End Source File
# Begin Source File

SOURCE=..\game\SpeederNPC.c
# End Source File
# Begin Source File

SOURCE=..\game\WalkerNPC.c
# End Source File
# Begin Source File

SOURCE=..\game\bg_g2_utils.c
# End Source File
# Begin Source File

SOURCE=..\game\bg_lib.c

!IF  "$(CFG)" == "JK2cgame - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "JK2cgame - Win32 Final"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "JK2cgame - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "JK2cgame - Win32 Debug(SH)"

# PROP Exclude_From_Build 1

!ENDIF

# End Source File
# Begin Source File

SOURCE=..\game\bg_misc.c
# End Source File
# Begin Source File

SOURCE=..\game\bg_panimate.c
# End Source File
# Begin Source File

SOURCE=..\game\bg_pmove.c
# End Source File
# Begin Source File

SOURCE=..\game\bg_saber.c
# End Source File
# Begin Source File

SOURCE=..\game\bg_saberLoad.c
# End Source File
# Begin Source File

SOURCE=..\game\bg_saga.c
# End Source File
# Begin Source File

SOURCE=..\game\bg_slidemove.c
# End Source File
# Begin Source File

SOURCE=..\game\bg_vehicleLoad.c
# End Source File
# Begin Source File

SOURCE=..\game\bg_weapons.c
# End Source File
# Begin Source File

SOURCE=.\cg_consolecmds.c
# End Source File
# Begin Source File

SOURCE=.\cg_draw.c
# End Source File
# Begin Source File

SOURCE=.\cg_drawtools.c
# End Source File
# Begin Source File

SOURCE=.\cg_effects.c
# End Source File
# Begin Source File

SOURCE=.\cg_ents.c
# End Source File
# Begin Source File

SOURCE=.\cg_event.c
# End Source File
# Begin Source File

SOURCE=.\cg_info.c
# End Source File
# Begin Source File

SOURCE=.\cg_light.c
# End Source File
# Begin Source File

SOURCE=.\cg_localents.c
# End Source File
# Begin Source File

SOURCE=.\cg_main.c
# End Source File
# Begin Source File

SOURCE=.\cg_marks.c
# End Source File
# Begin Source File

SOURCE=.\cg_newDraw.c
# End Source File
# Begin Source File

SOURCE=.\cg_players.c
# End Source File
# Begin Source File

SOURCE=.\cg_playerstate.c
# End Source File
# Begin Source File

SOURCE=.\cg_predict.c
# End Source File
# Begin Source File

SOURCE=.\cg_saga.c
# End Source File
# Begin Source File

SOURCE=.\cg_scoreboard.c
# End Source File
# Begin Source File

SOURCE=.\cg_servercmds.c
# End Source File
# Begin Source File

SOURCE=.\cg_snapshot.c
# End Source File
# Begin Source File

SOURCE=.\cg_strap.c
# End Source File
# Begin Source File

SOURCE=.\cg_syscalls.c
# End Source File
# Begin Source File

SOURCE=.\cg_turret.c
# End Source File
# Begin Source File

SOURCE=.\cg_view.c
# End Source File
# Begin Source File

SOURCE=.\cg_weaponinit.c
# End Source File
# Begin Source File

SOURCE=.\cg_weapons.c
# End Source File
# Begin Source File

SOURCE=.\fx_blaster.c
# End Source File
# Begin Source File

SOURCE=.\fx_bowcaster.c
# End Source File
# Begin Source File

SOURCE=.\fx_bryarpistol.c
# End Source File
# Begin Source File

SOURCE=.\fx_demp2.c
# End Source File
# Begin Source File

SOURCE=.\fx_disruptor.c
# End Source File
# Begin Source File

SOURCE=.\fx_flechette.c
# End Source File
# Begin Source File

SOURCE=.\fx_force.c
# End Source File
# Begin Source File

SOURCE=.\fx_heavyrepeater.c
# End Source File
# Begin Source File

SOURCE=.\fx_rocketlauncher.c
# End Source File
# Begin Source File

SOURCE=..\game\q_math.c
# End Source File
# Begin Source File

SOURCE=..\game\q_shared.c
# End Source File
# Begin Source File

SOURCE=..\ui\ui_shared.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h"
# Begin Source File

SOURCE=..\ghoul2\G2.h
# End Source File
# Begin Source File

SOURCE=.\JK2_cgame.def
# End Source File
# Begin Source File

SOURCE=..\game\anims.h
# End Source File
# Begin Source File

SOURCE=.\animtable.h
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

SOURCE=.\cg_lights.h
# End Source File
# Begin Source File

SOURCE=.\cg_local.h
# End Source File
# Begin Source File

SOURCE=.\cg_public.h
# End Source File
# Begin Source File

SOURCE=..\qcommon\disablewarnings.h
# End Source File
# Begin Source File

SOURCE=.\fx_local.h
# End Source File
# Begin Source File

SOURCE=holocronicons.h
# End Source File
# Begin Source File

SOURCE=..\ui\keycodes.h
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

SOURCE=.\tr_types.h
# End Source File
# Begin Source File

SOURCE=..\ui\ui_shared.h
# End Source File
# End Group
# Begin Group "win32"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\win32\JK2cgame.rc
# End Source File
# End Group
# End Target
# End Project

