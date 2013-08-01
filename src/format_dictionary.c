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
  while (scanf ("%s %s", word[0], word[1]) != EOF)
    {
      printf ("%-20s %s\n", word[0], word[1]);
    }
  return 0;
}
