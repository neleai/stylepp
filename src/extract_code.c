/* Copyright 2013 by Ondřej Bílka <neleai@seznam.cz> */


/*
   We extract words that are contained in comments but not in actual source.
 */

/*
   TODO  use trie.
 */

#include <string.h>

#define HASH_TYPE char
#include "hash.h"
#include "common.h"
int
main (int argc, char **argv)
{
  test_indent_off (argv[1]);
  if (TEST_EXTENSION (argv[1], ".txt") ||
      TEST_EXTENSION (argv[1], ".htm") ||
      TEST_EXTENSION (argv[1], ".html") ||
      TEST_EXTENSION (argv[1], ".texi"))
    return 0;
  int i, j, k, len;
  int incomment = 0, incomment2 = 0, insquote = 0, indquote = 0;
  char _buffer[100000], *buffer = _buffer + 1;
  char buffer2[100000];
  char *input = malloc (100000000), *ip = input;
  char *output = malloc (100000000), *op = output;
  _buffer[0] = ' ';
  while (fgets (buffer, 100000, stdin))
    {
      for (i = 0, j = 0; buffer[i]; )
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
	      if (!isalpha (buffer[i - 1]) && isalpha (buffer[i]))
		if (add_word (word (buffer + i), 1))
		  printf ("%s\n", word (buffer + i));

	      if (!cmp (buffer + i, "/*"))
		incomment = 1;
	      if (!cmp (buffer + i, "//"))
		incomment2 = 1;

	      if (buffer[i] == '\'')
		insquote = 1;
	      if (buffer[i] == '"')
		indquote = 1;
	    }
	  buffer2[j++] = buffer[i++];
	skip:;
	}
      memcpy (ip, buffer, i); ip += i;
      memcpy (op, buffer2, j); op += j;
    }

  return 0;
}
