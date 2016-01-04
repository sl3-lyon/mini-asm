CC=clang++
EXEC=mini-asm
FLAGS=-std=c++14 -Wall -pedantic -Wextra -Werror
SRC=src/*.cc

all:
	@$(CC) $(FLAGS) $(SRC) -o $(EXEC)
