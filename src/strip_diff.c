/* Copyright 2013 by Ondřej Bílka <neleai@seznam.cz> */

/* A tool to strip diffs */
#include "common.h"
typedef struct
{
  char *lines[2];
  int different_from, different_to;
} diff_s;

diff_s diffs[1000000]; int diffs_no = 0;

int
diffcmp (diff_s *a, diff_s *b)
{
  return strcasecmp (a->lines[1] + 40, b->lines[1] + 40);
}
int
main (int argc, char **argv)
{
  int i, j;
  char _buffer[4][100000];
  char *buffer[4];
  for (j = 0; j < 4; j++)
    {
      for (i = 0; i < 40; i++)
	_buffer[j][i] = ' ';
      buffer[j] = _buffer[j] + 40;
    }
  int skip = 0;
  diff_s diff;
  while (fgets (buffer[3], 100000, stdin))
    {
      /* Find occurences of
         [^+-]line
         -[^-]line
         +line
         [^+-]line
       */
      if (!one_from (buffer[0][0], "+-") && buffer[1][0] == '-' && buffer[1][1] != '-' && buffer[2][0] == '+' && !one_from (buffer[3][0], "+-"))
	{
	  for (i = 0; buffer[1][i]; i++)
	    if (buffer[1][i] == '\t')
	      buffer[1][i] = ' ';
	  for (i = 0; buffer[2][i]; i++)
	    if (buffer[2][i] == '\t')
	      buffer[2][i] = ' ';

	  for (i = 1; buffer[1][i] == buffer[2][i] && buffer[2][i]; i++)
	    ;
	  while (isalpha (buffer[1][i - 1]))
	    i--;
	  diff.different_from = i;

	  for (i = 1; buffer[1][strlen (buffer[1]) - i] == buffer[2][strlen (buffer[2]) - i] && i < strlen (buffer[1]) && i < strlen (buffer[2]); i++)
	    ;
	  diff.different_to = strlen (buffer[2]) - i;
	  diff.lines[0] = strdup (_buffer[1] + diff.different_from);
	  diff.lines[1] = strdup (_buffer[2] + diff.different_from);

	  /* Fill context with previous line. */
	  if (diff.different_from < 40)
	    {
	      int rep = 40 - diff.different_from;
	      diff.lines[0][rep - 3] = diff.lines[1][rep - 3] = ' ';
	      diff.lines[0][rep - 2] = diff.lines[1][rep - 2] = '\\';
	      diff.lines[0][rep - 1] = diff.lines[1][rep - 1] = 'n';
	      for (i = 3; i < rep && i < strlen (buffer[0]); i++)
		diff.lines[0][rep - i - 1] = diff.lines[1][rep - i - 1] = buffer[0][strlen (buffer[0]) - i + 1];
	    }
	  /* TODO context with next line. */
	  diffs[diffs_no++] = diff;
	  buffer[1][0] = ' ';
	  buffer[2][0] = ' ';
	}
      if (one_from (buffer[0][0], "+-") && !one_from (buffer[0][1], "+-"))
	printf ("%s", buffer[0]);
      for (i = 0; i < 3; i++)
	strcpy (buffer[i], buffer[i + 1]);
    }
  qsort (diffs, diffs_no, sizeof (diff_s), diffcmp);
  for (i = 0; i < diffs_no; i++)
    {
      printf ("%s%s", diffs[i].lines[0], diffs[i].lines[1]);
      for (j = 0; j < 40; j++)
	printf (" ");
      printf ("^");
      for (j = 1; j < diffs[i].different_to - diffs[i].different_from; j++)
	printf (" ");

      printf ("^\n");
    }

  return 1;
}
