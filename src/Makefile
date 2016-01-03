CC=clang++
EXEC=mini-asm
FLAGS=-std=c++14 -Wall -pedantic -Wextra -Werror
SRC=*.cc

all: debug

debug:
	@echo "Compiling in DEBUG mode"
	@$(CC) $(FLAGS) $(SRC) -o $(EXEC)

release:
	@echo "Compiling in RELEASE mode"
	@$(CC) $(FLAGS) -DNDEBUG -O2 $(SRC) -o $(EXEC)

clean:
	@rm -rf *.cc~ *.h~ .Makefile.swp $(EXEC)
