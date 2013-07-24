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
  FILE *fp[2];
  unsigned char word[2][MAX_LEN];
  for (i = 0; i < 2; i++)
    {
      fp[i] = fopen (argv[i + 1], "r");
    }
  while (scanf ("%s %s", word[0], word[1]) != EOF)
    {
      printf ("%-20s %s\n", word[0], word[1]);
    }
}
