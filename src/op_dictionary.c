/* Copyright 2013 by Ondřej Bílka <neleai@seznam.cz> */

/*
   Given two dictionaries this prints list of lines

   --subtract
   --intersect
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
  int intersect = 0;
  int subtract = 0;
  if (!strcmp (argv[1], "--subtract"))
    subtract = 1;
  if (!strcmp (argv[1], "--intersect"))
    intersect = 1;
  if (!subtract && !intersect)
    {
      printf ("Given operation and two dictionaries it will print lines\n"
	      "of first dictionary such that:\n"
	      "stylepp_op_dictionary --intersect dictionary1 dictionary2\n"
	      "  First word of line occurs both in dictionary1 and dictionary2.\n"
	      "stylepp_op_dictionary --subtract dictionary1 dictionary2\n"
	      "  First word of line occurs in dictionary1 but not in dictionary2.\n");
      return 1;
    }
  int i;
  for (i = 0; i < 2; i++)
    {
      fp[i] = fopen (argv[i + 2], "r");
      read_line (i);
    }
  while (1)
    {
      if (strcmp (word[0], word[1]) < 0)
	{
	  if (subtract)
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
	  if (intersect)
	    printf ("%s", lines[0]);

	  if (read_line (0))
	    return 0;
	  if (read_line (1))
	    return 0;
	}
    }
}
