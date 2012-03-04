del *.exe
cl /c test.c
cl /c swflib.c
cl /c bitio.c
cl /c mem.c
cl /c ae.c
link test.obj swflib.obj bitio.obj mem.obj
link ae.obj swflib.obj bitio.obj mem.obj
