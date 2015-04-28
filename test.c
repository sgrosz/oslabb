#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


/* program som gör:  ls -la | less  */

#define fel(x) {fprintf(stderr,"Fel i %s på rad %d\n",__FILE__, __LINE__); perror(x); }

int main()
{
  int pipa[2];    /* här lagras decriptornumren för pipen */
  pid_t pid;
  int status;


  if(pipe(pipa) == -1) /* skapa pipen */
    {
      fel("Could not pipe");
      exit(1);
    }

  pid = fork(); /* skapa en barnprocess */

  if(pid == -1)
    {
      fel("Could not fork");
      exit(1);
    }
  else if(pid == 0) /* barnprocessen */
    {
      if(dup2(pipa[1], 1) == -1) /* sätt barnprocessens "stdout" att gå in i pipe:n */
	{
	  fel("could not dup2");
	  exit(1);
	}
      if(close(pipa[0]) == -1) /* stäng pipe:ns läsände */
	{
	  fel("could not close");
	  exit(1);
	}

      execlp("ls", "ls", "-la", 0); /* starta ls */
      
      fel("could not start ls");
      exit(1);
    }
  else /* förälder */
    {
      pid = fork(); /* skapa en barnprocess */

      if(pid == -1)
	{
	  fel("Could not fork");
	  exit(1);
	}
      else if(pid == 0) /* barnprocessen */
	{
	  if(dup2(pipa[0], 0) == -1) /* sätt barnprocessens "stdin" att gå från i pipe:n */
	    {
	      fel("could not dup2");
	      exit(1);
	    }
	  if(close(pipa[1]) == -1) /* stäng pipe:ns skrivände */
	    {
	      fel("could not close");
	      exit(1);
	    }

	  execlp("more", "more", 0); /* starta less */
      
	  fel("could not start less");
	  exit(1);
	}
    }

  /* stäng ändarna på pipen -  som inte föräldern använder - annars hänger barnen då de tror att föräldern kan skriva på pipen */

  if(close(pipa[0]) == -1 || close(pipa[1]) == -1)
     fel("could not close");

   printf("do bla bla bla1");

  /* Vänta på barnprocesserna, HÄR BÖR MAN KOLLA STATUS!!!!!! */
  wait(&status);
  wait(&status);

  printf("do bla bla bla");
}