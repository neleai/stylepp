/* Copyright 2013 by Ondřej Bílka <neleai@seznam.cz> */

/*
   Create misspelling correction dictionary from file
   with misspellings and file with corrections.
 */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define MAX_LEN 100
#define ELEMS 1000000
char
strcmp2 (char **a, char **b)
{
  return strcmp (*a, *b);
}
char *words[ELEMS];
int
main (int argc, char **argv)
{
  int words_no = 0;
  int i, j;
  for (i = 0; i < ELEMS; i++)
    words[i] = malloc (MAX_LEN);
  while (fgets (words[words_no], MAX_LEN, stdin))
    words_no++;
  qsort (words, words_no, sizeof (char *), strcmp2);
  for (i = 0; i < words_no; i++)
    {
      if (strcmp (words[i], words[i + 1]))
	{
	  printf ("%s", words[i]);
	}
    }
  return 0;
}
