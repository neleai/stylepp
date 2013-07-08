/* Copyright 2013 by Ondřej Bílka <neleai@seznam.cz> */

/*
   Fix misspellings in comments. Reads list of word replacement lines from
   dictionary and replaces all these occurences in comments.
   You can specify multiword pattern by using underscore instead space.
 */


#include "common.h"
int
isword (char c)
{
  return isalpha (c) || c == '\'' || c == '-';
}
int
isseparator (char c)
{
  return isspace (c) || c == '.' || c == ',' || c == ';';
}



void
parseword (char *word)
{
  int i;
  for (i = 0; word[i]; i++)
    {
      if (word[i] == '_')
	word[i] = ' ';
      else if (!isword (word[i]) && !isdigit (word[i]))
	abort ();
    }
}

int name_number;
char names[10000][100], replacements[10000][100];
int
main (int argc, char **argv)
{
  // START_TEST

  int i, j, k, len;
  int incomment = 0, incomment2 = 0, insquote = 0, indquote = 0, inmail = 0;
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
  /* Verify that dictionary consists only of letters and '.  */
  for (i = 0; i < name_number; i++)
    {
      parseword (names[i]);
      parseword (replacements[i]);
    }



  while (fgets (buffer, 100000, stdin))
    {
      for (i = 0, j = 0; buffer[i]; )
	{
	  if (incomment || incomment2)
	    {
	      // Simple heuristic to identify mail address. Has false negatives
	      // but they are relatively rare.
	      if (buffer[i] == '<')
		inmail = 1;
	      if (buffer[i] == '>')
		inmail = 0;

	      // We try all replacement candidates.
	      // TODO use trie.
	      if (!inmail && isseparator (buffer[i - 1]) && isalnum (buffer[i]))
		for (k = 0; k < name_number; k++)
		  if (!cmp (buffer + i, names[k]) && isseparator (buffer[i + strlen (names[k])]))
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
	      if (!cmp (buffer + i, "//"))
		incomment2 = 1;
	      if (incomment || incomment2)
		inmail = 0;
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
