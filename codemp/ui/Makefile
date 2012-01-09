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
CFLAGS = -nologo -O2 -DFINAL_BUILD -DNDEBUG -DMISSIONPACK -DUI_EXPORTS -D_JK2 -D_WINDOWS -DWIN32
TARGET = uix86.dll

OFILES = ..\win32\ui.res ..\game\bg_misc.o ..\game\bg_saga.o ..\game\bg_vehicleLoad.o ..\game\bg_weapons.o ..\game\q_math.o ..\game\q_shared.o ui_atoms.o ui_force.o ui_gameinfo.o ui_main.o ui_saber.o ui_shared.o ui_syscalls.o

all: $(TARGET)

$(TARGET): $(OFILES)
	$(LINK) -nologo -MACHINE:X86 -DLL -BASE:0x20000000 -MAP:.\..\Final\uix86.map -out:$(TARGET) -DEF:ui.def $(OFILES) winmm.lib user32.lib

.c.o:
	$(CC) $(CFLAGS) -I.\ -I..\ -I..\game -c $*.c -Fo$*.o

game.res : ..\win32\JK2game.rc
	$(RC) $(RCFLAGS) $(RCVARS) -fo $@ $?

clean:
	del $(OFILES)
	del $(TARGET)
