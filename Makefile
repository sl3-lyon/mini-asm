CC=clang++
EXEC=mini-asm
FLAGS=-std=c++1y -Wall -pedantic -Wextra -Werror
SRC=src/*.cc

all: debug
	
debug:
	@$(CC) $(FLAGS) -g $(SRC) -o $(EXEC)

release:
	@$(CC) $(FLAG) -O2 -DNDEBUG $(SRC) -o $(EXEC)

clean:
	@rm -rf src/*.o src/.*.h.swp src/.*.cc.swp $(EXEC)
