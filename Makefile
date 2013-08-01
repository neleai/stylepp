all: src/*.c
	for I in $?; do\
	  $(CC) -Wall -Wno-unused -Wno-pointer-sign -g  -Isrc $$I -o bin/stylepp_`basename $$I .c`;\
	done
	touch all
