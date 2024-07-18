all: vcube

vcube: vcube.o smpl.o rand.o cisj.o
	$(LINK.c) -o $@ -Bstatic vcube.o smpl.o rand.o cisj.o -lm

smpl.o: smpl.c smpl.h
	$(COMPILE.c)  -g smpl.c

nbndD12.o: vcube.c smpl.h
	$(COMPILE.c) -g  vcube.c

rand.o: rand.c
	$(COMPILE.c) -g rand.c

cisj.o: cisj.c cisj.h
	$(COMPILE.c) -g cisj.c

clean:
	$(RM) *.o vcube relat saida