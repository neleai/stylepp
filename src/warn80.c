/* Copyright 2013 by Ondřej Bílka <neleai@seznam.cz> */

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#define isalnum_(x) (isalnum (x) || x == '_')
int
main (int argc, char **argv)
{
  int line = 1;
  int i, len, nslen, nsi;
  char buffer[100000];
  while (fgets (buffer, 100000, stdin))
    {
      len = 0;
      nsi = 0;
      nslen = 0;
      for (i = 0; buffer[i]; i++)
	{
	  if (buffer[i] == '\t')
	    len = 8 * (len / 8) + 8;
	  else
	    len++;
	}

      if (len > 81)
	fprintf (stderr, "vim +%i %s\n", line, argv[1]);
      line++;
    }


  return 1;
}
