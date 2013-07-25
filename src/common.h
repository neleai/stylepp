/* Copyright 2013 by Ondřej Bílka <neleai@seznam.cz> */

/*
   Collection of utility functions.
 */

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>

#define isalnum_(x) (isalnum (x) || x == '_')
#define cmp(x, y) memcmp (x, y, strlen (y))
#define one_from(x, y) (x && strchr (y, x))
#define TEST_EXTENSION(file, ext) (!cmp (file + strlen (file) - strlen (ext), ext))

void
test_indent_off (char *arg)
{
  if (!arg)
    return;
  char strbuf[1000]; strcpy (strbuf, arg);
  char strbuf2[1000];
  while (strchr (strbuf, '/') && strbuf[0] && strbuf[1])
    {
      dirname (strbuf);
      strcpy (strbuf2, strbuf);
      strcat (strbuf2, "/.indent.off");
      if (fopen (strbuf2, "r"))
	exit (1);
    }
}
int
write_if_common (char *input, char *ip, char *output, char *op)
{
  if ((ip - input == op - output) && !memcmp (input, output, ip - input))
    return (1);
  else
    {
      printf ("%s", output);
      return (0);
    }
}

int
isword (char c)
{
  return isalpha (c) || c == '\'' || c == '-';
}

int
isseparator (char *c)
{
  return isspace (c[0]) || ((c[0] == '.' || c[0] == ',' || c[0] == ';') && isspace (c[1]));
}
