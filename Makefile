all: src/*.c
	for I in $?; do\
	  $(CC) -Wall -Wno-unused -Wno-pointer-sign -g  -Isrc $$I -o bin/stylepp_`basename $$I .c`;\
	done
	touch all
	@echo
	@echo "To use stylepp commands without typing full path add:"
	@echo ".  $(PWD)/stylepp_source"
	@echo "to your .bashrc"
