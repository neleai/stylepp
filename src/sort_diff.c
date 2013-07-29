/* Copyright 2013 by Ondřej Bílka <neleai@seznam.cz> */

/* A tool to strip diffs */
#include "common.h"
typedef struct
{
  char *header, *sort_by, *body;
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
  return strcasecmp (a->sort_by, b->sort_by);
}
int
main (int argc, char **argv)
{
  FILE *stream = stdin;
  if (argc > 1)
    stream = fopen (argv[1], "r");
  int i, j, k;
  char buffer[1000000];
  diff_s diff;
  char header[10000];
  char body[1000000];
  char *bodyp = body;
  char firstm = 0, firstps[1000];
  char firstp = 0, firstms[1000];
  while (fgets (buffer, 100000, stream))
    {
      if (!cmp (buffer, "diff --git"))
	{
	  strcpy (header, buffer);
	  for (i = 0; i < 3; i++)
	    {
	      fgets (buffer, 100000, stream);
	      strcat (header, buffer);
	    }
	}
      else if (!cmp (buffer, "@@ "))
	{
	  diffs[diffs_no].body = strdup (body);
	  diffs_no++;
	  diffs[diffs_no].header = strdup (header);
	  diffs[diffs_no].sort_by = strdup ("\127");
	  strcpy (body, buffer);
	  bodyp = body;
	  firstp = 0;
	  firstm = 0;
	}
      else
	{
	  if (!firstp && buffer[0] == '+')
	    {
	      firstp = 1;
	      for (i = 1; buffer[i] && buffer[i] == firstms[i]; i++)
		;
	      while (i > 0 && isword (buffer[i - 1]))
		i--;

	      diffs[diffs_no].sort_by = strdup (buffer + i);
	    }
	  if (!firstm && buffer[0] == '-')
	    {
	      firstm = 1;
	      strcpy (firstms, buffer);
	    }

	  strcat (bodyp, buffer);
	  bodyp += strlen (bodyp);
	}
    }
  if (!diffs_no)
    return 1;
  qsort (diffs + 1, diffs_no - 1, sizeof (diff_s), (__compar_fn_t) diffcmp);
  for (i = 1; i < diffs_no - 1; i++)
    {
      printf ("%s%s", diffs[i].header, diffs[i].body);
    }

  return 1;
}
