all: compile run

compile:

		gcc -I ./include/ -o ./lib/input.o -c ./src/input.c
		gcc -I ./include/ -o ./lib/promt.o -c ./src/promt.c
		gcc -I ./include/ -o ./lib/parse.o -c ./src/parse.c
		gcc -I ./include/ -o ./lib/process.o -c ./src/process.c
		gcc -I ./include/ -o ./bin/shell ./lib/input.o ./lib/process.o ./lib/parse.o ./lib/promt.o ./src/shell.c

run:
			./bin/shell
