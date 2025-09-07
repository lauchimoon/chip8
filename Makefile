CC=gcc
SRC=main.c chip8.c opcodes.c
OUT=c8
PATH_TO_RAYLIB ?= /home/lauren/software/raylib/src/
LDLIBS=-L$(PATH_TO_RAYLIB) -lraylib -lm -ldl -lpthread -lGL
INCLUDE=-I$(PATH_TO_RAYLIB)

default:
	$(CC) -o $(OUT) $(SRC) $(LDLIBS) $(INCLUDE)
