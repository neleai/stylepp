/* Copyright 2013 by Ondřej Bílka <neleai@seznam.cz> */

/*
   Format misspell dictionary.
 */
#include <string.h>
#include <stdio.h>
#define MAX_LEN 10000
int
main (int argc, char **argv)
{
  int i, j;
  unsigned char word[2][MAX_LEN];
  unsigned char lastword[2][MAX_LEN];
  int dupe = 1;
  while (scanf ("%s %s", word[0], word[1]) != EOF)
    {
      if (!strcmp (word[0], lastword[0]))
	dupe = 1;
      else
	{
	  if (!dupe)
	    printf ("%-20s %s\n", lastword[0], lastword[1]);
	  strcpy (lastword[0], word[0]);
	  strcpy (lastword[1], word[1]);
	  dupe = 0;
	}
    }
  if (!dupe)
    printf ("%-20s %s\n", lastword[0], lastword[1]);
  return 0;
}
