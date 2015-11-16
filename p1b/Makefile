all: meme

clean:
	rm process.o meme meme.o
	
tarball: makefile meme.c process.c process.h lap.h README
	tar -czf 612500889.tar.gz makefile *.c *.h README 

meme: meme.o process.o
	gcc -ansi -pedantic -o meme meme.o
	
meme.o: meme.c lap.h
	gcc -ansi -pedantic -c meme.c
	
process.o: process.c process.h
	gcc -ansi -pedantic -c process.c

valgrind: meme
	valgrind --leak-check=full ./meme memes.meme insane.act

run: meme
	./meme memes.meme insane.act

compare : compare.c
	gcc -ansi -g -o compare compare.c -lm


gradememe : compare meme gradememe0 gradememe1 gradememe2  gradememe3 gradememe4  gradememe5  gradememe6  gradememe7  gradememe8  gradememe9  

# 3 standard cases are for 60 points
gradememe0: compare meme
	@echo MEME 00: First Example
	-time ./meme memes.mem insaneproject.act
	-./compare insanity_project.simp gradememe0.simp 20
	@echo
	@echo
	
gradememe1: compare meme
	@echo MEME 01: Second Example
	-time ./meme memes.mem confessproject.act
	-./compare confess_project.simp gradememe1.simp 20
	@echo
	@echo
	
gradememe2: compare meme
	@echo MEME 02: Overflow example
	-time ./meme memes.mem outofbounds.act
	-./compare outofbounds.simp gradememe2.simp 20
	@echo
	@echo

# 3 alternate cases for 60 points
gradememe3: compare meme
	@echo MEME 03: Alternate MEME file, Alternate Font 
	-time ./meme memes2.mem doge.act
	-./compare dogememeout.simp gradememe3.simp 20
	@echo
	@echo

gradememe4: compare meme
	@echo MEME 04: Alternate MEME file, lots of text
	-time ./meme memes2.mem fulloftext.act
	-./compare fulloftext.simp gradememe4.simp 20
	@echo
	@echo

gradememe5: compare meme
	@echo MEME 05: Alternate MEME file, Regular Font, ACT file order 
	-time ./meme memes2.mem zoidberg.act
	-./compare zoidbergmeme.simp gradememe5.simp 20
	@echo
	@echo
	

# 4 special cases, for 30 points total
gradememe6: compare meme
	@echo MEME 06: No arguments. 5 points for no failure only!!!
	-time ./meme
	@echo
	@echo
	
gradememe7: compare meme
	@echo MEME 07: File missing. 5 points for no failure only!!!
	-time ./meme memes.mem THISFILEDOESNOTEXIST
	@echo
	@echo

gradememe8: compare meme
	@echo MEME 08: Font file missing within MEM file. 5 points for no failure only!!!
	-time ./meme failmeme.mem insaneproject.act
	@echo
	@echo

gradememe9: compare meme
	@echo MEME 09: Character is missing from font. 5 points for no failure.
	-time ./meme memes.mem missing.act
	-./compare missing.simp gradememe9.simp 2
	@echo Up to 2 bonus points for agreeing with expected values ignore missing characters.
	@echo
	@echo



memcheck : compare memcheckcrop memcheckbw memcheckcolorshift memcheckoverlay

memcheckbw : memcheckbw0 memcheckbw1 memcheckbw2 memcheckbw3

memcheckbw0: bw
	@echo BW memcheck 00: Standard Example
	-valgrind --leak-check=full --show-reachable=yes ./bw  attached.simp bw_attached.simp 
	-./compare bw_attached.simp memcheckbw0.simp 3
	@echo
	@echo

memcheckbw1: bw
	@echo BW memcheck 01: Doge Example
	-valgrind --leak-check=full --show-reachable=yes ./bw doge.simp bw_doge.simp 
	-./compare bw_doge.simp memcheckbw1.simp 3
	@echo
	@echo

memcheckbw2: bw
	@echo BW memcheck 02: Confession Example
	-valgrind --leak-check=full --show-reachable=yes ./bw confession.simp bw_confess.simp 
	-./compare bw_confess.simp memcheckbw2.simp 3
	@echo
	@echo

memcheckbw3: bw
	@echo BW memcheck 03:
	-valgrind --leak-check=full --show-reachable=yes ./bw keanu.simp keanu_bw.simp
	-./compare keanu_bw.simp memcheckbw3.simp 3
	@echo
	@echo

memcheckcolorshift : memcheckcolor0 memcheckcolor1 

memcheckcolor0: colorshift
	@echo COLORSHIFT memcheck 00: Standard Example
	-valgrind --leak-check=full --show-reachable=yes ./colorshift keanu.simp keanurgb.simp RGB 
	-./compare keanurgb.simp memcheckcolor0a.simp 3
	@echo
	-valgrind --leak-check=full --show-reachable=yes ./colorshift keanu.simp keanurbg.simp RBG 
	-./compare keanurbg.simp memcheckcolor0b.simp 3
	@echo
	@echo
	
memcheckcolor1: colorshift
	@echo COLORSHIFT memcheck 01: Alternate Patterns
	-valgrind --leak-check=full --show-reachable=yes ./colorshift doge.simp doge_gbr.simp BR 
	-./compare doge_gbr.simp memcheckcolor1a.simp 3
	@echo
	-valgrind --leak-check=full --show-reachable=yes ./colorshift doge.simp doge_bgr.simp BG 
	-./compare doge_bgr.simp memcheckcolor1b.simp 3
	@echo

memcheckcrop : memcheckcrop0 memcheckcrop1 memcheckcrop2 memcheckcrop3

memcheckcrop0: crop
	@echo Crop memcheck 00: Standard Example
	-valgrind --leak-check=full --show-reachable=yes ./crop insanity.simp c_insanity.simp 150 112 220 290  
	-./compare c_insanity.simp memcheckcrop0.simp 4
	@echo
	@echo
	
memcheckcrop1: crop
	@echo Crop memcheck 01: 1-by-1
	-valgrind --leak-check=full --show-reachable=yes ./crop attached.simp attached_one.simp 100 300 1 1
	-./compare  attached_one.simp memcheckcrop1.simp 3
	@echo
	@echo
	
memcheckcrop2: crop
	@echo Crop memcheck 02: BIG
	-valgrind --leak-check=full --show-reachable=yes ./crop confession.simp confess_crop.simp 600 400 100 100
	-./compare confess_crop.simp memcheckcrop2.simp 3
	@echo
	@echo
	
memcheckcrop3: crop
	@echo Crop memcheck 03: Tall
	-valgrind --leak-check=full --show-reachable=yes ./crop keanu.simp keanu_skinny.simp 100 0 5 300  
	-./compare keanu_skinny.simp memcheckcrop3.simp 3
	@echo
	@echo


memcheckoverlay: memcheckoverlay0 memcheckoverlay1 memcheckoverlay2 memcheckoverlay3 
	
memcheckoverlay0: overlay
	@echo overlay memcheck 00: Standard example 
	-valgrind --leak-check=full --show-reachable=yes ./overlay  insanity.simp doge.simp insanedoge.simp 150 120 
	-./compare insanedoge.simp memcheckoverlay0.simp 4
	@echo
	@echo
	
memcheckoverlay1: overlay
	@echo overlay memcheck 01: Attached Insane
	-valgrind --leak-check=full --show-reachable=yes ./overlay insanity.simp attached_head.simp insaneattached.simp 110 110
	-./compare insaneattached.simp memcheckoverlay1.simp 3
	@echo
	@echo
	
memcheckoverlay2: overlay
	@echo overlay memcheck 02: Attached Confession Big
	-valgrind --leak-check=full --show-reachable=yes ./overlay confession.simp attached_head.simp attachconfess.simp 400 150
	-./compare attachconfess.simp memcheckoverlay2.simp 3
	@echo
	@echo
	
memcheckoverlay3: overlay
	@echo overlay memcheck 03: Doge Attached
	-valgrind --leak-check=full --show-reachable=yes ./overlay attached.simp doge.simp dogeattach.simp 240 100
	-./compare dogeattach.simp memcheckoverlay3.simp 3
	@echo
	@echo
	
