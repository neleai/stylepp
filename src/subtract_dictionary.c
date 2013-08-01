/* Copyright 2013 by Ondřej Bílka <neleai@seznam.cz> */

/*
   Prints list of words that appear in first sorted file but not second.
 */
#include "common.h"

#define MAX_LEN 10000


FILE *fp[2];
unsigned char word[2][MAX_LEN];
char lines[2][MAX_LEN];
int
read_line (int i)
{
  if (!fgets (lines[i], MAX_LEN, fp[i]))
    return 1;
  int j = 0;
  while (lines[i][j] && !isspace (lines[i][j]))
    {
      word[i][j] = lines[i][j]; j++;
    }
  word[i][j] = 0;
  return 0;
}
int
main (int argc, char **argv)
{
  int i;
  for (i = 0; i < 2; i++)
    {
      fp[i] = fopen (argv[i + 1], "r");
      read_line (i);
    }
  while (1)
    {
      if (strcmp (word[0], word[1]) < 0)
	{
	  printf ("%s", lines[0]);
	  if (read_line (0))
	    return 0;
	}
      else if (strcmp (word[0], word[1]) > 0)
	{
	  if (read_line (1))
	    for (i = 0; i < MAX_LEN; i++)
	      word[1][i] = 255;
	}
      else
	{
	  if (read_line (0))
	    return 0;
	  if (read_line (1))
	    return 0;
	}
    }
}
