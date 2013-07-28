/* Copyright 2013 by Ondřej Bílka <neleai@seznam.cz> */

/* A tool to strip diffs */
#include "common.h"
typedef struct
{
  char *lines[3];
  int different_from, different_to;
} diff_s;

diff_s diffs[1000000]; int diffs_no = 0;


char *
strmove (char *dest, char *src)
{
  return memmove (dest, src, strlen (src) + 1);
}

int
diffcmp (diff_s *a, diff_s *b)
{
  return strcasecmp (a->lines[1] + 40, b->lines[1] + 40);
}
int
main (int argc, char **argv)
{
  FILE *stream = stdin;
  if (argc > 1)
    stream = fopen (argv[1], "r");
  int i, j, k;
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
  while (fgets (buffer[3], 100000, stream))
    {
      /* Find occurrences of
         [^+-]line
         -[^-]line
         +line
         [^+-]line
       */
      char _pbuffer[10000], *pbuffer = _pbuffer + 40;
      if (!one_from (buffer[0][0], "+-") && buffer[1][0] == '-' && buffer[1][1] != '-' && buffer[2][0] == '+' && !one_from (buffer[3][0], "+-"))
	{
	  /* Strip leading spaces */
	  for (i = 1; isspace (buffer[1][i]) && isspace (buffer[2][i]) && buffer[1][i] == buffer[2][i]; i++)
	    ;
	  for (j = 1; j <= 2; j++)
	    strmove (buffer[j] + 1, buffer[j] + i);

	  for (i = 1; buffer[1][i] == buffer[2][i] && buffer[2][i]; i++)
	    ;
	  int dif2 = i;
	  while (isword (buffer[1][i - 1]))
	    i--;
	  diff.different_from = i;

	  for (i = 1; buffer[1][strlen (buffer[1]) - i] == buffer[2][strlen (buffer[2]) - i] && i < strlen (buffer[1]) && i < strlen (buffer[2]); i++)
	    ;
	  diff.different_to = strlen (buffer[2]) - i;

	  /* Replace tabs to keep alignment. */
	  for (j = 0; buffer[0][j]; j++)
	    if (buffer[0][j] == '\t')
	      buffer[0][j] = ' ';
	  for (j = 0; j < diff.different_from; j++)
	    if (buffer[1][j] == '\t')
	      buffer[1][j] = ' ';
	  for (j = diff.different_to + strlen (buffer[1]) - strlen (buffer[2]); buffer[1][j]; j++)
	    if (buffer[1][j] == '\t')
	      buffer[1][j] = ' ';
	  for (j = 0; j < diff.different_from; j++)
	    if (buffer[2][j] == '\t')
	      buffer[2][j] = ' ';
	  for (j = diff.different_to; buffer[2][j]; j++)
	    if (buffer[2][j] == '\t')
	      buffer[2][j] = ' ';
	  for (j = 0; buffer[3][j]; j++)
	    if (buffer[3][j] == '\t')
	      buffer[3][j] = ' ';

	  /* TODO remove spaces at start of lines.*/


	  for (k = 0; k < 3; k++)
	    diff.lines[k] = malloc (10000);

	  /* Place change marks */
	  for (i = 0; i < 10000; i++)
	    _pbuffer[i] = ' ';
	  pbuffer[40 + dif2] = '^';
	  pbuffer[40 + diff.different_to] = '^';
	  pbuffer[79 + diff.different_from] = '\n';
	  pbuffer[80 + diff.different_from] = 0;

	  strcpy (diff.lines[0], _buffer[1] + diff.different_from);
	  strcpy (diff.lines[1], _buffer[2] + diff.different_from);
	  strcpy (diff.lines[2], pbuffer + diff.different_from);

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
	  for (i = 0; i < 2; i++)
	    {
	      diff.lines[i][strlen (diff.lines[i]) - 1] = 0;
	      strcat (diff.lines[i], " \\n ");
	      for (j = 0; isspace (buffer[3][j]); j++)
		;
	      strcat (diff.lines[i], buffer[3] + j);
	      diff.lines[i][strlen (diff.lines[i]) - 1] = 0;
	      for (j = strlen (diff.lines[i]); j < 80; j++)
		diff.lines[i][j] = ' ';
	      diff.lines[i][80] = '\n';
	      diff.lines[i][81] = 0;
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
      printf ("%s%s%s", diffs[i].lines[0], diffs[i].lines[1], diffs[i].lines[2]);
    }

  return 1;
}
