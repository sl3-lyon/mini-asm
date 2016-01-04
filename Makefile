CC=clang++
EXEC=mini-asm
FLAGS=-std=c++1y -Wall -pedantic -Wextra -Werror
SRC=src/*.cc

all:
	@$(CC) $(FLAGS) $(SRC) -o $(EXEC)
