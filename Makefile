all: src/*.c
	for I in $?; do\
	  gcc -Isrc $$I -o bin/stylepp_`basename $$I .c`;\
	done
	touch all
