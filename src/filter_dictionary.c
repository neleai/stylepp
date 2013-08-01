/* Copyright 2013 by Ondřej Bílka <neleai@seznam.cz> */

/*
   Filter dictionary by checking if words differ by at most two characters.
 */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define MAX_LEN 10000
int
main (int argc, char **argv)
{
  int i, j;
  unsigned char word[2][MAX_LEN];
  int counts[2][256];
  while (scanf ("%s %s", word[0], word[1]) != EOF)
    {
      for (i = 0; i < 2; i++)
	for (j = 0; j < 256; j++)
	  counts[i][j] = 0;
      for (i = 0; i < 2; i++)
	for (j = 0; word[i][j]; j++)
	  counts[i][word[i][j]]++;
      int diff = 0;
      for (i = 0; i < 256; i++)
	diff += abs (counts[0][i] - counts[1][i]);
      if (diff < 3 && strlen (word[0]) > 2 && strlen (word[1]) > 2)
	printf ("%-20s %s\n", word[0], word[1]);
    }
  return 0;
}
