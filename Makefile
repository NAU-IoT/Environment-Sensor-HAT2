# compiler
CC		= gcc
# compiler flags
FLGS	= -std=c18 -march=native -g -Wall -Wextra -pedantic -Wno-unused-result -Wparentheses -Wsign-compare
# libaries we need to link against
LIBS	= -lm
# source files
SRC		= waveshare.c src/util.c src/bme280.c
# compiled binary for the waveshare board
BIN 	= WS_bin

# compile core utils
compile:
	${CC} ${FLGS} ${SRC} -o ${BIN}

# valgrind binary for checking memory leaks
VG		= valgrind
# valgrind flags
VGFLGS	= --leak-check=full --show-leak-kinds=all --track-origins=yes --tool=memcheck

check-mem:
	${VG} ${VGFLGS} ./${BIN}	

all:
	${bme280}

clean:
	# remove binary
	rm -f ${BIN}


