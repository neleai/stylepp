all: src/*.c
	for I in $?; do\
	  gcc -g -Isrc $$I -o bin/stylepp_`basename $$I .c`;\
	done
	touch all
