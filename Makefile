all: src/*.c
	for I in $?; do\
	  gcc -g -O3 -Isrc $$I -o bin/stylepp_`basename $$I .c`;\
	done
	touch all
