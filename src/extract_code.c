/* Copyright 2013 by Ondřej Bílka <neleai@seznam.cz> */


/*
   We extract words that are contained in comments but not in actual source.
 */

char **names;

#include <string.h>

#define HASH_TYPE int
#include "common.h"
#include "hash.h"

#include "comments.h"

#ifndef EXTRACT_COMMENT
  # define EXTRACT_COMMENT 0
#endif
int
main (int argc, char **argv)
{
  test_indent_off (argv[1]);
  int lang = find_language (argv[1]);


  int i, j, k, len;
  int incomment = 0, incomment2 = 0, insquote = 0, indquote = 0;
  char *_buffer = malloc (100000000), *buffer = _buffer + 1;
  char *buffer2 = malloc (100000000);
  char *input = malloc (100000000), *ip = input;
  char *output = malloc (100000000), *op = output;
  _buffer[0] = ' ';
  names = malloc (100000000);
  int names_no = 1;

  // Treat everything in plain files as comment.
  if (lang == LANG_PLAIN)
    incomment = 1;

  while (fgets (buffer, 10000000, stdin))
    {
      for (i = 0, j = 0; buffer[i]; )
	{
	  if (EXTRACT_COMMENT && (incomment || incomment2) && isseparator (buffer + i - 1) && isalpha (buffer[i]))
	    if (add_word (word (buffer + i), names_no))
	      {
		names[names_no++] = strdup (word (buffer + i));
		printf ("%s\n", word (buffer + i));
	      }
	  if (incomment)
	    {
	      if (end_comment (lang, buffer + i))
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
	      if (!EXTRACT_COMMENT && !isalpha (buffer[i - 1]) && isalpha (buffer[i]))
		if (add_word (word (buffer + i), names_no))
		  {
		    names[names_no++] = strdup (word (buffer + i));
		    printf ("%s\n", word (buffer + i));
		  }
	      if (start_comment (lang, buffer + i))
		incomment = 1;
	      if (start_line_comment (lang, buffer + i))
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
