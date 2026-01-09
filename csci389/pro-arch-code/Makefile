FLAGS = -Wall -Wextra -Werror -std=c99 -g

CC = clang

test: stage_test.c base.o stages.o assembler.o
	$(CC) $(FLAGS) -o test stage_test.c base.o stages.o assembler.o

base.o: base.c base.h
	$(CC) $(FLAGS) -c base.c

stages.o: stages.c stages.h base.h
	$(CC) $(FLAGS) -c stages.c

assembler.o: assembler.c assembler.h instructions.h
	$(CC) $(FLAGS) -c assembler.c

.PHONY: clean

clean:
	rm test base.o stages.o assembler.o
