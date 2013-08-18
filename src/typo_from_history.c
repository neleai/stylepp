/* Copyright 2013 by Ondřej Bílka <neleai@seznam.cz> */



#include "common.h"

int
main (int argc, char **argv)
{
  int line = 1;
  int i, len, nslen, nsi;
  char *buffer = malloc (100000000);
  while (fgets (buffer, 100000, stdin))
    {
      len = 0;
      nsi = 0;
      nslen = 0;
      char *word = malloc (100000000), *repl = malloc (100000000);
      for (i = 0; buffer[i]; i++)
	{
	  if (!cmp (buffer + i, "[-"))
	    {
	      int j = i;
	      j += 2;
	      while (isspace (buffer[j]))
		j++;
	      int k = 0;
	      while (isalpha (buffer[j]))
		word[k++] = buffer[j++];
	      word[k] = 0;
	      while (isspace (buffer[j]))
		j++;
	      if (cmp (buffer + j, "-]{+"))
		goto next;
	      j += 4;
	      while (isspace (buffer[j]))
		j++;
	      k = 0;
	      while (isalpha (buffer[j]))
		repl[k++] = buffer[j++];
	      repl[k] = 0;
	      while (isspace (buffer[j]))
		j++;
	      if (cmp (buffer + j, "+}"))
		goto next;
	      j += 2;
	      while (isspace (buffer[j]))
		j++;
	      if (!isalpha (buffer[j]))
		goto next;
	      printf ("%s %s\n", word, repl);
	    }
	next:;
	}
    }


  return 1;
}
