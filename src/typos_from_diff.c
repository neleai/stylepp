/* Copyright 2013 by Ondřej Bílka <neleai@seznam.cz> */

/* A tool to strip diffs */
#include "common.h"

int
main (int argc, char **argv)
{
  FILE *stream = stdin;
  if (argc > 1)
    stream = fopen (argv[1], "r");
  int i, j, k;
  char _buffer[4][100000];
  char *buffer[4];
  for (j = 0; j < 4; j++)
    {
      for (i = 0; i < 40; i++)
	_buffer[j][i] = ' ';
      buffer[j] = _buffer[j] + 40;
    }
  int skip = 0;
  while (fgets (buffer[3], 100000, stream))
    {
      /* Find occurrences of
         [^+-]line
         -[^-]line
         +line
         [^+-]line
       */
      char *_pbuffer = malloc (100000000), *pbuffer = _pbuffer + 40;
      if (!one_from (buffer[0][0], "+-") && buffer[1][0] == '-' && buffer[1][1] != '-' && buffer[2][0] == '+' && !one_from (buffer[3][0], "+-"))
	{
	  for (i = 1; buffer[1][i] == buffer[2][i] && buffer[2][i]; i++)
	    ;
	  while (isword (buffer[1][i - 1]))
	    i--;
	  int different_from = i;
	  j = i;
	  while (isword (buffer[1][i]))
	    i++;
	  while (isword (buffer[2][j]))
	    j++;
	  if (!strcmp (buffer[1] + i, buffer[2] + j))
	    {
	      buffer[1][i] = 0;
	      buffer[2][j] = 0;
	      printf ("%s %s\n", buffer[1] + different_from, buffer[2] + different_from);
	    }
	}
      for (i = 0; i < 3; i++)
	strcpy (buffer[i], buffer[i + 1]);
    }

  return 0;
}
