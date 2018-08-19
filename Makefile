all: platformer

clean:
	rm -f platformer cutils.o

platformer: platformer.c cutils.o 
	gcc platformer.c cutils.o -o platformer

debug: platformer.c cutils.o
	gcc -g platformer.c cutils.o -o platformer

cutils.o: cutils.c cutils.h
	gcc -c cutils.c -o cutils.o