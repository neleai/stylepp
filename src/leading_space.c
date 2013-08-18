/* Copyright 2013 by Ondřej Bílka <neleai@seznam.cz> */

#include "common.h"

#ifndef SPACEBEFORETAB
# define SPACEBEFORETAB 0
#endif

int
main (int argc, char **argv)
{
  test_indent_off (argv[1]);
  int i, j, k, len;
  int incomment = 0, incomment2 = 0, insquote = 0, indquote = 0;
  char *_buffer = malloc (100000000), *buffer = _buffer + 1;
  char *buffer2 = malloc (100000000);
  char *input = malloc (100000000), *ip = input;
  char *output = malloc (100000000), *op = output;
  _buffer[0] = ' ';
  while (fgets (buffer, 100000, stdin))
    {
      int spacebeforetab = 1 - SPACEBEFORETAB;
      i = 0;
      j = 0;
      len = 0;

#ifndef TRAILING_SPACE
      /* Change leading spaces to tabs followed by space format.  */
      if (!insquote && !indquote) /* Beware of multiline comments.  */
	{
	  while (buffer[i] && isblank (buffer[i]))
	    {
	      if (buffer[i] == ' ')
		len++;
	      if (buffer[i] == '\t')
		len = 8 * (len / 8) + 8;
	      i++;
	      if (!cmp (buffer + i, " \t"))
		spacebeforetab = 1;
	    }

# ifdef LEADING_TAB
	  if (spacebeforetab)
	    {
	      for (k = 0; k < len / 8; k++)
		buffer2[j++] = '\t';
	      for (k = 0; k < len % 8; k++)
		buffer2[j++] = ' ';
	    }
	  else
	    i = 0;
# else
	  for (k = 0; k < len; k++)
	    buffer2[j++] = ' ';
# endif
	}
#endif

      while (buffer[i])
	{
	  if (incomment)
	    {
	      if (!cmp (buffer + i, "*/"))
		incomment = 0;
	    }
	  else if (incomment2)
	    {
	      if (buffer[i] != '\\' && buffer[i + 1] == '\n')
		incomment2 = 0;
	    }
	  else if (insquote)
	    {
	      if (buffer[i] == '\\')
		buffer2[j++] = buffer[i++];
	      else if (buffer[i] == '\'')
		insquote = 0;
	    }
	  else if (indquote)
	    {
	      if (buffer[i] == '\\')
		buffer2[j++] = buffer[i++];
	      else if (buffer[i] == '"')
		indquote = 0;
	    }
	  else
	    {
	      if (!cmp (buffer + i, "/*"))
		incomment = 1;
	      if (!cmp (buffer + i, "//") || buffer[i] == '#')
		incomment2 = 1;

	      if (buffer[i] == '\'')
		insquote = 1;
	      if (buffer[i] == '"')
		indquote = 1;
	    }
	  buffer2[j++] = buffer[i++];
	skip:;
	}

#ifdef TRAILING_SPACE
      /* Remove trailing spaces. */
      j--;
      while (j > 0 && (isblank (buffer2[j - 1]) || buffer2[j - 1] == FORMFEED))
	j--;
      buffer2[j++] = '\n';
#endif

      memcpy (ip, buffer, i); ip += i;
      memcpy (op, buffer2, j); op += j;
    }
  return write_if_common (input, ip, output, op);
}
