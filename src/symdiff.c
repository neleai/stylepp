/*
   Prints list of words that appear in first sorted file but not second.
 */
#include <string.h>
#include <stdio.h>
#define MAX_LEN 100
int
main (int argc, char **argv)
{
  int i;
  FILE *fp[2];
  unsigned char word[2][MAX_LEN];
  for (i = 0; i < 2; i++)
    {
      fp[i] = fopen (argv[i + 1], "r");
      fscanf (fp[i], "%s", word[i]);
    }
  while (1)
    {
      if (strcmp (word[0], word[1]) < 0)
	{
	  printf ("%s\n", word[0]);
	  if (fscanf (fp[0], "%s", word[0]) == EOF)
	    return 0;
	}
      else if (strcmp (word[0], word[1]) > 0)
	{
	  if (fscanf (fp[1], "%s", word[1]) == EOF)
	    for (i = 0; i < MAX_LEN; i++)
	      word[1][i] = 255;
	}
      else
	{
	  if (fscanf (fp[0], "%s", word[0]) == EOF)
	    return 0;
	}
    }
}
