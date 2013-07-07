/*
   We extract words that are contained in comments but not in actual source.
 */

/*
   TODO  use trie.
 */

int
isword (char c)
{
  return isalpha (c) || c == '\'' || c == '-';
}

#include <string.h>
#define HASH_SIZE 1000000
char _word[1000];
char *
word (char *c)
{
  int i;
  for (i = 0; isword (c[i]); i++)
    _word[i] = c[i];
  _word[i] = '\0';
  return _word;
}

char code_words[HASH_SIZE];
unsigned int
hash (char *s)
{
  int collect = 0;
  while (*s)
    {
      collect = 11 * collect + *s;
      s++;
    }
  return collect;
}
int
add_word (char *word)
{
  int h = hash (word) % HASH_SIZE;
  if (code_words[h])
    return 0;
  code_words[h] = 1;
  return 1;
}



#include "common.h"
int
main (int argc, char **argv)
{
//START_TEST
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
	  if ((incomment || incomment2) && !isalpha (buffer[i - 1]) && isalpha (buffer[i]))
	    if (add_word (word (buffer + i)))
	      printf ("%s\n", word (buffer + i));

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
