all: meme process

clean:
	rm process process.o meme meme.o
	
tarball: makefile meme.c process.c process.h lap.h README
	tar -czf 612500889.tar.gz makefile *.c *.h README 

meme: meme.o
	gcc -ansi -pedantic -o meme meme.c
	
meme.o: meme.c lap.h
	gcc -ansi -pedantic -c meme.o
	
process: process.c
	gcc -ansi -pedantic -o process process.c
	
process.o: process.c process.h
	gcc -ansi -pedantic -c process.o