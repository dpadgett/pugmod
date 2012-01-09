# Microsoft Developer Studio Project File - Name="JK2game" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=JK2game - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "JK2game.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "JK2game.mak" CFG="JK2game - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "JK2game - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "JK2game - Win32 Final" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "JK2game - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "JK2game - Win32 Debug(SH)" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "JK2game - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\../Release"
# PROP BASE Intermediate_Dir ".\../Release/JK2game"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\../Release"
# PROP Intermediate_Dir ".\../Release/JK2game"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /ML /Zi /W4 /Ob2 /Oi /Op /Ot /G6 /GA /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "MISSIONPACK" /D "QAGAME" /D "_JK2" /GF /Gm /Gy /YX /Fp".\../Release/JK2game/JK2_game.pch" /Fo".\../Release/JK2game/" /Fd".\../Release/JK2game/" /c /GX 
# ADD CPP /nologo /ML /Zi /W4 /Ob2 /Oi /Op /Ot /G6 /GA /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "MISSIONPACK" /D "QAGAME" /D "_JK2" /GF /Gm /Gy /YX /Fp".\../Release/JK2game/JK2_game.pch" /Fo".\../Release/JK2game/" /Fd".\../Release/JK2game/" /c /GX 
# ADD BASE MTL /nologo /D"NDebug" /mktyplib203 /tlb".\..\Release\JK2_game.tlb" /win32 
# ADD MTL /nologo /D"NDebug" /mktyplib203 /tlb".\..\Release\JK2_game.tlb" /win32 
# ADD BASE RSC /l 1033 /d "NDebug" 
# ADD RSC /l 1033 /d "NDebug" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo 
# ADD BSC32 /nologo 
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib /nologo /dll /out:"..\Release\jampgamex86.dll" /incremental:no /def:".\JK2_game.def" /debug /pdb:".\..\Release\jampgamex86.pdb" /pdbtype:sept /map:"..\Release\jampgamex86.map" /subsystem:windows /opt:ref /opt:icf /release /base:"0x20000000" /implib:".\../Release/jampgamex86.lib" /MACHINE:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib /nologo /dll /out:"..\Release\jampgamex86.dll" /incremental:no /def:".\JK2_game.def" /debug /pdb:".\..\Release\jampgamex86.pdb" /pdbtype:sept /map:"..\Release\jampgamex86.map" /subsystem:windows /opt:ref /opt:icf /release /base:"0x20000000" /implib:".\../Release/jampgamex86.lib" /MACHINE:I386

!ELSEIF  "$(CFG)" == "JK2game - Win32 Final"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\../Final"
# PROP BASE Intermediate_Dir ".\../Final/JK2game"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\../Final"
# PROP Intermediate_Dir ".\../Final/JK2game"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /ML /W4 /Ob2 /Oi /Op /Ot /G6 /GA /D "NDEBUG" /D "_WINDOWS" /D "MISSIONPACK" /D "QAGAME" /D "WIN32" /D "_JK2" /D "FINAL_BUILD" /GF /Gy /YX /Fp".\../Final/JK2game/JK2_game.pch" /Fo".\../Final/JK2game/" /Fd".\../Final/JK2game/" /c /GX 
# ADD CPP /nologo /ML /W4 /Ob2 /Oi /Op /Ot /G6 /GA /D "NDEBUG" /D "_WINDOWS" /D "MISSIONPACK" /D "QAGAME" /D "WIN32" /D "_JK2" /D "FINAL_BUILD" /GF /Gy /YX /Fp".\../Final/JK2game/JK2_game.pch" /Fo".\../Final/JK2game/" /Fd".\../Final/JK2game/" /c /GX 
# ADD BASE MTL /nologo /D"NDebug" /mktyplib203 /tlb".\..\Final\JK2_game.tlb" /win32 
# ADD MTL /nologo /D"NDebug" /mktyplib203 /tlb".\..\Final\JK2_game.tlb" /win32 
# ADD BASE RSC /l 1033 /d "NDebug" 
# ADD RSC /l 1033 /d "NDebug" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo 
# ADD BSC32 /nologo 
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib /nologo /dll /out:"..\Final\jampgamex86.dll" /incremental:no /def:".\JK2_game.def" /pdbtype:sept /map:".\..\Final\jampgamex86.map" /mapinfo:lines /subsystem:windows /opt:ref /opt:icf /release /base:"0x20000000" /implib:".\../Final/jampgamex86.lib" /MACHINE:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib /nologo /dll /out:"..\Final\jampgamex86.dll" /incremental:no /def:".\JK2_game.def" /pdbtype:sept /map:".\..\Final\jampgamex86.map" /mapinfo:lines /subsystem:windows /opt:ref /opt:icf /release /base:"0x20000000" /implib:".\../Final/jampgamex86.lib" /MACHINE:I386

!ELSEIF  "$(CFG)" == "JK2game - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\../Debug"
# PROP BASE Intermediate_Dir ".\../Debug/JK2game"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\../Debug"
# PROP Intermediate_Dir ".\../Debug/JK2game"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MLd /ZI /W3 /Od /G6 /D "_DEBUG" /D "BUILDING_REF_GL" /D "Debug" /D "WIN32" /D "_WINDOWS" /D "MISSIONPACK" /D "QAGAME" /D "_JK2" /D "JK2AWARDS" /YX /Fp".\../Debug/JK2game/JK2_game.pch" /Fo".\../Debug/JK2game/" /Fd".\../Debug/JK2game/" /FR /GZ /c /GX 
# ADD CPP /nologo /MLd /ZI /W3 /Od /G6 /D "_DEBUG" /D "BUILDING_REF_GL" /D "Debug" /D "WIN32" /D "_WINDOWS" /D "MISSIONPACK" /D "QAGAME" /D "_JK2" /D "JK2AWARDS" /YX /Fp".\../Debug/JK2game/JK2_game.pch" /Fo".\../Debug/JK2game/" /Fd".\../Debug/JK2game/" /FR /GZ /c /GX 
# ADD BASE MTL /nologo /D"_Debug" /mktyplib203 /tlb".\..\Debug\JK2_game.tlb" /win32 
# ADD MTL /nologo /D"_Debug" /mktyplib203 /tlb".\..\Debug\JK2_game.tlb" /win32 
# ADD BASE RSC /l 1033 /d "_Debug" 
# ADD RSC /l 1033 /d "_Debug" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo 
# ADD BSC32 /nologo 
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib /nologo /dll /out:"..\Debug\jampgamex86.dll" /incremental:yes /def:".\JK2_game.def" /debug /pdb:".\..\Debug\jampgamex86.pdb" /pdbtype:sept /map:"..\Debug\jampgamex86.map" /subsystem:windows /base:"0x20000000" /implib:".\../Debug/jampgamex86.lib" /MACHINE:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib /nologo /dll /out:"..\Debug\jampgamex86.dll" /incremental:yes /def:".\JK2_game.def" /debug /pdb:".\..\Debug\jampgamex86.pdb" /pdbtype:sept /map:"..\Debug\jampgamex86.map" /subsystem:windows /base:"0x20000000" /implib:".\../Debug/jampgamex86.lib" /MACHINE:I386

!ELSEIF  "$(CFG)" == "JK2game - Win32 Debug(SH)"

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
# ADD BASE CPP /nologo /MLd /ZI /W3 /Od /G6 /D "_DEBUG" /D "BUILDING_REF_GL" /D "Debug" /D "WIN32" /D "_WINDOWS" /D "MISSIONPACK" /D "QAGAME" /D "_JK2" /D "JK2AWARDS" /YX /Fp".\../Debug/JK2game/JK2_game.pch" /Fo".\../Debug/JK2game/" /Fd".\../Debug/JK2game/" /FR /GZ /c /GX 
# ADD CPP /nologo /MLd /ZI /W3 /Od /G6 /D "_DEBUG" /D "BUILDING_REF_GL" /D "Debug" /D "WIN32" /D "_WINDOWS" /D "MISSIONPACK" /D "QAGAME" /D "_JK2" /D "JK2AWARDS" /YX /Fp".\../Debug/JK2game/JK2_game.pch" /Fo".\../Debug/JK2game/" /Fd".\../Debug/JK2game/" /FR /GZ /c /GX 
# ADD BASE MTL /nologo /D"_Debug" /mktyplib203 /tlb".\..\Debug\JK2_game.tlb" /win32 
# ADD MTL /nologo /D"_Debug" /mktyplib203 /tlb".\..\Debug\JK2_game.tlb" /win32 
# ADD BASE RSC /l 1033 /d "_Debug" 
# ADD RSC /l 1033 /d "_Debug" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo 
# ADD BSC32 /nologo 
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib /nologo /dll /out:"..\Debug\jampgamex86.dll" /incremental:yes /def:".\JK2_game.def" /debug /pdb:".\..\Debug\jampgamex86.pdb" /pdbtype:sept /map:".\..\Debug\jampgamex86.map" /subsystem:windows /base:"0x20000000" /implib:".\../Debug/jampgamex86.lib" /MACHINE:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib /nologo /dll /out:"..\Debug\jampgamex86.dll" /incremental:yes /def:".\JK2_game.def" /debug /pdb:".\..\Debug\jampgamex86.pdb" /pdbtype:sept /map:".\..\Debug\jampgamex86.map" /subsystem:windows /base:"0x20000000" /implib:".\../Debug/jampgamex86.lib" /MACHINE:I386

!ENDIF

# Begin Target

# Name "JK2game - Win32 Release"
# Name "JK2game - Win32 Final"
# Name "JK2game - Win32 Debug"
# Name "JK2game - Win32 Debug(SH)"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Source File

SOURCE=AnimalNPC.c
# End Source File
# Begin Source File

SOURCE=FighterNPC.c
# End Source File
# Begin Source File

SOURCE=.\NPC.c
# End Source File
# Begin Source File

SOURCE=.\NPC_AI_Atst.c
# End Source File
# Begin Source File

SOURCE=.\NPC_AI_Default.c
# End Source File
# Begin Source File

SOURCE=.\NPC_AI_Droid.c
# End Source File
# Begin Source File

SOURCE=.\NPC_AI_GalakMech.c
# End Source File
# Begin Source File

SOURCE=.\NPC_AI_Grenadier.c
# End Source File
# Begin Source File

SOURCE=.\NPC_AI_Howler.c
# End Source File
# Begin Source File

SOURCE=.\NPC_AI_ImperialProbe.c
# End Source File
# Begin Source File

SOURCE=.\NPC_AI_Interrogator.c
# End Source File
# Begin Source File

SOURCE=.\NPC_AI_Jedi.c
# End Source File
# Begin Source File

SOURCE=.\NPC_AI_Mark1.c
# End Source File
# Begin Source File

SOURCE=.\NPC_AI_Mark2.c
# End Source File
# Begin Source File

SOURCE=.\NPC_AI_MineMonster.c
# End Source File
# Begin Source File

SOURCE=NPC_AI_Rancor.c
# End Source File
# Begin Source File

SOURCE=.\NPC_AI_Remote.c
# End Source File
# Begin Source File

SOURCE=.\NPC_AI_Seeker.c
# End Source File
# Begin Source File

SOURCE=.\NPC_AI_Sentry.c
# End Source File
# Begin Source File

SOURCE=.\NPC_AI_Sniper.c
# End Source File
# Begin Source File

SOURCE=.\NPC_AI_Stormtrooper.c
# End Source File
# Begin Source File

SOURCE=.\NPC_AI_Utils.c
# End Source File
# Begin Source File

SOURCE=NPC_AI_Wampa.c
# End Source File
# Begin Source File

SOURCE=.\NPC_behavior.c
# End Source File
# Begin Source File

SOURCE=.\NPC_combat.c
# End Source File
# Begin Source File

SOURCE=.\NPC_goal.c
# End Source File
# Begin Source File

SOURCE=.\NPC_misc.c
# End Source File
# Begin Source File

SOURCE=.\NPC_move.c
# End Source File
# Begin Source File

SOURCE=.\NPC_reactions.c
# End Source File
# Begin Source File

SOURCE=.\NPC_senses.c
# End Source File
# Begin Source File

SOURCE=.\NPC_sounds.c
# End Source File
# Begin Source File

SOURCE=.\NPC_spawn.c
# End Source File
# Begin Source File

SOURCE=.\NPC_stats.c
# End Source File
# Begin Source File

SOURCE=.\NPC_utils.c
# End Source File
# Begin Source File

SOURCE=SpeederNPC.c
# End Source File
# Begin Source File

SOURCE=WalkerNPC.c
# End Source File
# Begin Source File

SOURCE=.\ai_main.c
# End Source File
# Begin Source File

SOURCE=.\ai_util.c
# End Source File
# Begin Source File

SOURCE=.\ai_wpnav.c
# End Source File
# Begin Source File

SOURCE=bg_g2_utils.c
# End Source File
# Begin Source File

SOURCE=.\bg_lib.c

!IF  "$(CFG)" == "JK2game - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "JK2game - Win32 Final"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "JK2game - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "JK2game - Win32 Debug(SH)"

# PROP Exclude_From_Build 1

!ENDIF

# End Source File
# Begin Source File

SOURCE=.\bg_misc.c
# End Source File
# Begin Source File

SOURCE=.\bg_panimate.c
# End Source File
# Begin Source File

SOURCE=.\bg_pmove.c
# End Source File
# Begin Source File

SOURCE=.\bg_saber.c
# End Source File
# Begin Source File

SOURCE=bg_saberLoad.c
# End Source File
# Begin Source File

SOURCE=bg_saga.c
# End Source File
# Begin Source File

SOURCE=.\bg_slidemove.c
# End Source File
# Begin Source File

SOURCE=bg_vehicleLoad.c
# End Source File
# Begin Source File

SOURCE=.\bg_weapons.c
# End Source File
# Begin Source File

SOURCE=.\g_ICARUScb.c
# End Source File
# Begin Source File

SOURCE=.\g_active.c
# End Source File
# Begin Source File

SOURCE=.\g_arenas.c
# End Source File
# Begin Source File

SOURCE=.\g_bot.c
# End Source File
# Begin Source File

SOURCE=.\g_client.c
# End Source File
# Begin Source File

SOURCE=.\g_cmds.c
# End Source File
# Begin Source File

SOURCE=.\g_combat.c
# End Source File
# Begin Source File

SOURCE=.\g_exphysics.c
# End Source File
# Begin Source File

SOURCE=.\g_items.c
# End Source File
# Begin Source File

SOURCE=.\g_log.c
# End Source File
# Begin Source File

SOURCE=.\g_main.c
# End Source File
# Begin Source File

SOURCE=.\g_mem.c
# End Source File
# Begin Source File

SOURCE=.\g_misc.c
# End Source File
# Begin Source File

SOURCE=.\g_missile.c
# End Source File
# Begin Source File

SOURCE=.\g_mover.c
# End Source File
# Begin Source File

SOURCE=.\g_nav.c
# End Source File
# Begin Source File

SOURCE=.\g_navnew.c
# End Source File
# Begin Source File

SOURCE=.\g_object.c
# End Source File
# Begin Source File

SOURCE=.\g_saga.c
# End Source File
# Begin Source File

SOURCE=.\g_session.c
# End Source File
# Begin Source File

SOURCE=.\g_spawn.c
# End Source File
# Begin Source File

SOURCE=.\g_strap.c
# End Source File
# Begin Source File

SOURCE=.\g_svcmds.c
# End Source File
# Begin Source File

SOURCE=.\g_syscalls.c
# End Source File
# Begin Source File

SOURCE=.\g_target.c
# End Source File
# Begin Source File

SOURCE=.\g_team.c
# End Source File
# Begin Source File

SOURCE=.\g_timer.c
# End Source File
# Begin Source File

SOURCE=.\g_trigger.c
# End Source File
# Begin Source File

SOURCE=g_turret.c
# End Source File
# Begin Source File

SOURCE=g_turret_G2.c
# End Source File
# Begin Source File

SOURCE=.\g_utils.c
# End Source File
# Begin Source File

SOURCE=g_vehicleTurret.c
# End Source File
# Begin Source File

SOURCE=g_vehicles.c
# End Source File
# Begin Source File

SOURCE=.\g_weapon.c
# End Source File
# Begin Source File

SOURCE=.\q_math.c
# End Source File
# Begin Source File

SOURCE=.\q_shared.c
# End Source File
# Begin Source File

SOURCE=tri_coll_test.c
# End Source File
# Begin Source File

SOURCE=.\w_force.c
# End Source File
# Begin Source File

SOURCE=.\w_saber.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=..\ghoul2\G2.h
# End Source File
# Begin Source File

SOURCE=.\JK2_game.def
# End Source File
# Begin Source File

SOURCE=.\ai.h
# End Source File
# Begin Source File

SOURCE=.\ai_main.h
# End Source File
# Begin Source File

SOURCE=.\anims.h
# End Source File
# Begin Source File

SOURCE=..\cgame\animtable.h
# End Source File
# Begin Source File

SOURCE=.\b_local.h
# End Source File
# Begin Source File

SOURCE=.\b_public.h
# End Source File
# Begin Source File

SOURCE=.\be_aas.h
# End Source File
# Begin Source File

SOURCE=.\be_ai_char.h
# End Source File
# Begin Source File

SOURCE=.\be_ai_chat.h
# End Source File
# Begin Source File

SOURCE=.\be_ai_gen.h
# End Source File
# Begin Source File

SOURCE=.\be_ai_goal.h
# End Source File
# Begin Source File

SOURCE=.\be_ai_move.h
# End Source File
# Begin Source File

SOURCE=.\be_ai_weap.h
# End Source File
# Begin Source File

SOURCE=.\be_ea.h
# End Source File
# Begin Source File

SOURCE=.\bg_lib.h
# End Source File
# Begin Source File

SOURCE=.\bg_local.h
# End Source File
# Begin Source File

SOURCE=.\bg_public.h
# End Source File
# Begin Source File

SOURCE=.\bg_saga.h
# End Source File
# Begin Source File

SOURCE=.\bg_strap.h
# End Source File
# Begin Source File

SOURCE=bg_vehicles.h
# End Source File
# Begin Source File

SOURCE=.\bg_weapons.h
# End Source File
# Begin Source File

SOURCE=.\botlib.h
# End Source File
# Begin Source File

SOURCE=..\cgame\cg_local.h
# End Source File
# Begin Source File

SOURCE=..\cgame\cg_public.h
# End Source File
# Begin Source File

SOURCE=.\chars.h
# End Source File
# Begin Source File

SOURCE=..\qcommon\disablewarnings.h
# End Source File
# Begin Source File

SOURCE=.\g_ICARUScb.h
# End Source File
# Begin Source File

SOURCE=g_headers.h
# End Source File
# Begin Source File

SOURCE=.\g_local.h
# End Source File
# Begin Source File

SOURCE=.\g_nav.h
# End Source File
# Begin Source File

SOURCE=.\g_public.h
# End Source File
# Begin Source File

SOURCE=.\g_team.h
# End Source File
# Begin Source File

SOURCE=.\inv.h
# End Source File
# Begin Source File

SOURCE=.\match.h
# End Source File
# Begin Source File

SOURCE=..\..\ui\menudef.h
# End Source File
# Begin Source File

SOURCE=.\npc_headers.h
# End Source File
# Begin Source File

SOURCE=.\q_shared.h
# End Source File
# Begin Source File

SOURCE=.\say.h
# End Source File
# Begin Source File

SOURCE=.\surfaceflags.h
# End Source File
# Begin Source File

SOURCE=.\syn.h
# End Source File
# Begin Source File

SOURCE=..\qcommon\tags.h
# End Source File
# Begin Source File

SOURCE=.\teams.h
# End Source File
# Begin Source File

SOURCE=..\cgame\tr_types.h
# End Source File
# Begin Source File

SOURCE=.\w_saber.h
# End Source File
# Begin Group "ICARUS Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\icarus\Q3_Interface.h
# End Source File
# Begin Source File

SOURCE=..\icarus\interpreter.h
# End Source File
# End Group
# End Group
# Begin Group "win32"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\win32\JK2game.rc
# End Source File
# End Group
# End Target
# End Project

