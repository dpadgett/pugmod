# Linux Makefile for Jedi Academy MP SDK
# By Patrick Hemmer
# Version 2
# 
# Created Nov 29, 2003
# The Void - http://www.divoid.net
#
# You may set your own optimizations on the CFLAGS line. If you dont know what optimizations are, then just leave it as is.
# Run 'make' to compile the code, once done, you will have a file called 'jampgamei386.so' in the 'game' directory. Copy this file to the 'base' folder of your server or to your mod folder.
# If for some strange reason, you are running on less than a 586 processor, change the i586 to i386 on the CFLAGS line.

#CC = gcc
#LINK = gcc
#CFLAGS = -fPIC -DFINAL_BUILD -DNDEBUG -DMISSIONPACK -DQAGAME -D_JK2 -D__linux__ -march=i586 -O3 
CC = cl
LINK = link
RC = rc
RCFLAGS = -r
RCVARS = -DWIN32 -D_WIN32 -DWINVER=0x0500
CFLAGS = -nologo -O2 -DFINAL_BUILD -DNDEBUG -DMISSIONPACK -DCGAME -D_JK2 -D_WINDOWS -DWIN32
TARGET = cgamex86.dll

OFILES = cgame.res cg_consolecmds.o cg_draw.o cg_drawtools.o cg_effects.o cg_ents.o cg_event.o cg_hdc.o cg_info.o cg_light.o cg_localents.o cg_main.o cg_marks.o cg_newDraw.o cg_players.o cg_playerstate.o cg_predict.o cg_saga.o cg_scoreboard.o cg_servercmds.o cg_snapshot.o cg_strap.o cg_syscalls.o cg_turret.o cg_view.o cg_weaponinit.o cg_weapons.o fx_blaster.o fx_bowcaster.o fx_bryarpistol.o fx_demp2.o fx_disruptor.o fx_flechette.o fx_force.o fx_heavyrepeater.o fx_rocketlauncher.o
GAMEOFILES = AnimalNPC.o bg_g2_utils.o bg_lib.o bg_misc.o bg_panimate.o bg_pmove.o bg_saber.o bg_saberLoad.o bg_saga.o bg_slidemove.o bg_vehicleLoad.o bg_weapons.o FighterNPC.o SpeederNPC.o WalkerNPC.o q_math.o q_shared.o
UIOFILES = ui_shared.o

all: $(TARGET)

$(TARGET): $(OFILES) $(GAMEOFILES) $(UIOFILES)
	$(LINK) -nologo -MACHINE:X86 -DLL -BASE:0x20000000 -MAP:.\..\Final\cgamex86.map -out:$(TARGET) -DEF:JK2_cgame.def $(OFILES) $(GAMEOFILES) $(UIOFILES) winmm.lib user32.lib

$(GAMEOFILES):
	$(CC) $(CFLAGS) -I.\ -I..\ -I..\game -c ..\game\$*.c -Fo$*.o

$(UIOFILES):
	$(CC) $(CFLAGS) -I.\ -I..\ -I..\game -c ..\ui\$*.c -Fo$*.o

cgame.res: ..\win32\JK2cgame.rc
	$(RC) $(RCFLAGS) $(RCVARS) -fo $@ $?

.c.o:
	$(CC) $(CFLAGS) -I.\ -I..\ -I..\game -c $*.c -Fo$*.o

clean:
	del $(OFILES) $(GAMEOFILES) $(UIOFILES)
	del $(TARGET)
