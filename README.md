# oslabb
Lab work in the course ID2200 Operating systems.

This is a shell that handles all the normal commands of a shell plus out own implementation of the commands "cd", "exit" and "checkEnv". There are two different versions of the code depending on how you compile it.

If you want the shell to use polling then compile it using the following command:

	make

If you want the shell to use a signal handler then compile it using the following command:

	make OPT=-DSIGDET=1

To then run the shell use the following command:

	./program.out