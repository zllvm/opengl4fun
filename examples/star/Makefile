BIN = star.exe 
CC = g++
FLAGS = -Wall -pedantic
INC = -I ../../../common/include
LOC_LIB = -L ../../../common/lib
SYS_LIB = -lglfw3 -lGL -lX11 -lXxf86vm -lXrandr -lpthread -lXi -lm -ldl
SRC = glad.c star.cpp 

all:
	@echo
	@echo ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	@echo ~~~~~~~~~~~~~~~~~~~~~~~~~~~Building GNU/LINUX 64-bit~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	@echo ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	@echo
	${CC} ${FLAGS} -o ${BIN} ${SRC} ${INC} ${LOC_LIB} ${SYS_LIB}

