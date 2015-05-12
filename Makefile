all:
	gcc -pedantic -Wall $(OPT) -ansi -O4 -o program.out program.c cd.c checkenv.c helper.c