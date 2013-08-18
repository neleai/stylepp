/* Copyright 2013 by Ondřej Bílka <neleai@seznam.cz> */

/*
   Create misspelling correction dictionary from file
   with misspellings and file with corrections.
 */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define MAX_LEN 100
#define ELEMS 100000000
char
strcmp2 (char **a, char **b)
{
  return strcmp (*a, *b);
}
int
main (int argc, char **argv)
{
  char **words = malloc (ELEMS * sizeof (char *));
  char *buffer = malloc (100000000);
  int words_no = 0;
  int i, j;
  while (fgets (buffer, MAX_LEN, stdin))
    {
      words[words_no++] = strdup (buffer);
    }
  words[words_no] = malloc (1);
  qsort (words, words_no, sizeof (char *), (__compar_fn_t) strcmp2);
  for (i = 0; i < words_no; i++)
    {
      if (strcmp (words[i], words[i + 1]))
	{
	  printf ("%s", words[i]);
	}
    }
  return 0;
}
