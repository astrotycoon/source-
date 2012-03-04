#####################################################################################
# MBoo Player Makefile for Microsoft NMAKE
# Created by Wilson Wang <wilsonbbk66@gmail.com>
#####################################################################################
OUTDIR=build

all: $(OUTDIR) $(OUTDIR)\Player.exe

$(OUTDIR) :
    if not exist "$(OUTDIR)" mkdir $(OUTDIR)

# Update the resource if necessary
$(OUTDIR)\main.res: main.rc resource.h
    rc /fo $(OUTDIR)\main.res main.rc

# Update the object file if necessary
$(OUTDIR)\main.obj: main.c resource.h
    cl /c /WX /Fo"$(OUTDIR)\\" /Fd"$(OUTDIR)\\" main.c

# Update the executable file if necessary, and if so, add the resource back in.
$(OUTDIR)\Player.exe: $(OUTDIR)\main.obj $(OUTDIR)\main.res
    link -out:$(OUTDIR)\Player.exe $(OUTDIR)\main.obj $(OUTDIR)\main.res gdi32.lib user32.lib

clean:
       del /F $(OUTDIR)\*
