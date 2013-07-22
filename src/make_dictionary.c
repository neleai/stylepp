/* Copyright 2013 by Ondřej Bílka <neleai@seznam.cz> */

/*
   Create misspelling correction dictionary from file
   with misspellings and file with corrections.
 */
#include <string.h>
#include <stdio.h>
#define MAX_LEN 100
int
main (int argc, char **argv)
{
  int i, j;
  FILE *fp[2];
  unsigned char word[2][MAX_LEN];
  for (i = 0; i < 2; i++)
    {
      fp[i] = fopen (argv[i + 1], "r");
    }
  while (1)
    {
      for (i = 0; i < 2; i++)
	{
	  if (!fgets (word[i], MAX_LEN, fp[i]))
	    return 0;
	  for (j = 0; word[i][j]; j++)
	    {
	      if (word[i][j] == ' ')
		word[i][j] = '_';
	      if (word[i][j] == '\n')
		word[i][j] = '\0';
	    }
	}
      if (strcmp (word[0], word[1]))
	{
	  printf ("%-20s %s\n", word[0], word[1]);
	}
    }
}
