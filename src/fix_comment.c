/* Copyright 2013 by Ondřej Bílka <neleai@seznam.cz> */

/*
   Fix misspellings in comments. Reads list of word replacement lines from
   dictionary and replaces all these occurences in comments.
 */
#include "common.h"
int name_number;
char names[10000][100], replacements[10000][100];
int
main (int argc, char **argv)
{
  // START_TEST

  int i, j, k, len;
  int incomment = 0, incomment2 = 0, insquote = 0, indquote = 0;
  char _buffer[100000], *buffer = _buffer + 1;
  char buffer2[100000];
  char *input = malloc (100000000), *ip = input;
  char *output = malloc (100000000), *op = output;
  _buffer[0] = ' ';

  /* Read a dictionary */
  FILE *dictionary = fopen ("dictionary", "r");
  name_number = 0;
  while (fscanf (dictionary, "%s %s", names[name_number], replacements[name_number]) != EOF)
    name_number++;
  /* Verify thaat dictionary consists only of letters and '.  */
  for (i = 0; i < name_number; i++)
    {
      for (j = 0; names[i][j]; j++){
        if (names[i][j]=='_') 
           names[i][j]=' ';
	else if (!isalpha (names[i][j]) && names[i][j] != '\'')
	  abort ();
      }
      for (j = 0; replacements[i][j]; j++){
        if (replacements[i][j]=='_') 
           replacements[i][j]=' ';
	else if (!isalpha (replacements[i][j]) && replacements[i][j] != '\'')
	  abort ();
      }
    }



  while (fgets (buffer, 100000, stdin))
    {
      for (i = 0, j = 0; buffer[i]; )
	{
	  if (incomment || incomment2)
	    {
	      // try replace.
	      // TODO use trie.
	      if (!isalnum (buffer[i - 1]))
		for (k = 0; k < name_number; k++)
		  if (!cmp (buffer + i, names[k]) && !isalnum (buffer[i + strlen (names[k])]))
		    {
		      strcpy (buffer2 + j, replacements[k]);
		      i += strlen (names[k]);
		      j += strlen (replacements[k]);
		    }
	    }
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
      memcpy (ip, buffer, i); ip += i;
      memcpy (op, buffer2, j); op += j;
    }
  WRITE_IF_COMMON
}
