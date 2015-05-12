all:
	gcc -pedantic -Wall $(CFLAGS) -ansi -O4 -o program program.c cd.c checkenv.c error.c