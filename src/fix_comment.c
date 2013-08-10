/* Copyright 2013 by Ondřej Bílka <neleai@seznam.cz> */

/*
   Fix misspellings in comments. Reads list of word replacement lines from
   dictionary and replaces all these occurrences in comments.
   You can specify multiword pattern by using underscore instead space.
 */

#include <string.h>

#define HASH_TYPE int


#include "common.h"
#include "hash.h"

#include "comments.h"

void
parseword (char *word)
{
  int i;
  for (i = 0; word[i]; i++)
    {
      if (word[i] == '_')
	word[i] = ' ';
      else if (!isword (word[i]) && !isdigit (word[i]))
	{
	  printf ("illegal character in dictionary in: %s\n", word);
	  abort ();
	}
    }
}

int name_number;
char **names, **replacements;
int
main (int argc, char **argv)
{
  int lang = find_language (argv[1]);
  test_indent_off (argv[1]);

  int unfiltered = 0;
  if (argc >= 3 && !strcmp (argv[2], "--unfiltered"))
    unfiltered = 1;

  int i, j, k, l, len;
  int incomment = 0, incomment2 = 0, insquote = 0, indquote = 0, inmail = 0, inhtml = 0;
  char _buffer[100000], *buffer = _buffer + 1;
  char buffer2[100000];
  char *input = malloc (100000000), *ip = input;
  char *output = malloc (100000000), *op = output;
  names = malloc (100000000);
  replacements = malloc (100000000);

  _buffer[0] = ' ';

  // Treat everything in plain files as comment.
  if (lang == LANG_PLAIN)
    incomment = 1;

#ifndef FIX_A_AN
  /* Read a dictionary */
  FILE *dictionary = fopen (".stylepp/dictionary", "r");
  name_number = 0;
  while (fscanf (dictionary, "%s %s", buffer, buffer2) != EOF)
    {
      names[name_number] = strdup (buffer);
      replacements[name_number] = strdup (buffer2);
      name_number++;
    }
  /* Verify that dictionary consists only of letters and '.  */
  for (i = 0; i < name_number; i++)
    {
      parseword (names[i]);
      parseword (replacements[i]);
      add_word (names[i], i);
      names[i + name_number] = strdup (names[i]);
      names[i + name_number][0] = toupper (names[i][0]);
      replacements[i + name_number] = strdup (replacements[i]);
      replacements[i + name_number][0] = toupper (replacements[i][0]);
      add_word (names[i + name_number], i + name_number);
    }
#else
  /* Read a dictionary */
  FILE *dictionary = fopen ("dictionary", "r");
  name_number = 1;
  while (fscanf (dictionary, "%s", buffer) != EOF)
    {
      names[name_number] = strdup (buffer);
      name_number++;
    }
  for (i = 1; i < name_number; i++)
    {
      parseword (names[i]);
      add_word (names[i], i);
    }
#endif

  /* End if there is nothing to do. */
  if (!name_number)
    return 1;
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
#ifndef FIX_A_AN
	      if ((unfiltered && isalnum (buffer[i]) && !isalnum (buffer[i - 1])) || (!inmail && !inhtml && isseparator (buffer + i - 1) && buffer[i - 1] != '.' && isalnum (buffer[i])))
		{
		  k = get_word (word (buffer + i));
		  if (!cmp (buffer + i, names[k]) && (isseparator (buffer + i + strlen (names[k])) || (unfiltered && !isalnum (buffer[i + strlen (names[k])]))))
		    {
		      if (isupper (names[k][0]))
			{
			  l = j - 1;
			  while (isspace (buffer2[l]))
			    l--;
			  if (buffer2[l] != '.' && buffer2[l] != '*')
			    goto skip2;
			}
		      strcpy (buffer2 + j, replacements[k]);
		      i += strlen (names[k]);
		      j += strlen (replacements[k]);
		    skip2:;
		    }
		}
#else
	      /* TODO check if a/A does appear in code to avoid more false positives.*/
	      if (isspace (buffer[i - 1]) && !cmp (buffer + i, "a ") && one_from (buffer[i + 2], "aeio") && strlen (word (buffer + i + 2)) > 3 && !get_word (word (buffer + i + 2)))
		{
		  strcpy (buffer2 + j, "an ");
		  i += strlen ("a ");
		  j += strlen ("an ");
		}
	      if (buffer[i - 2] == '.' && isspace (buffer[i - 1]) && !cmp (buffer + i, "A ") && one_from (buffer[i + 2], "aeio") && strlen (word (buffer + i + 2)) > 3 && !get_word (word (buffer + i + 2)))
		{
		  strcpy (buffer2 + j, "An ");
		  i += strlen ("A ");
		  j += strlen ("An ");
		}
	      if (isspace (buffer[i - 1]) && !cmp (buffer + i, "an ") && islower (buffer[i + 2]) && !one_from (buffer[i + 2], "aeiouy") && strlen (word (buffer + i + 2)) > 3 && !get_word (word (buffer + i + 3)))
		{
		  strcpy (buffer2 + j, "a ");
		  i += strlen ("an ");
		  j += strlen ("a ");
		}
	      if (buffer[i - 2] == '.' && isspace (buffer[i - 1]) && !cmp (buffer + i, "An ") && islower (buffer[i + 2]) && !one_from (buffer[i + 2], "aeiouy") && strlen (word (buffer + i + 2)) > 3 && !get_word (word (buffer + i + 3)))
		{
		  strcpy (buffer2 + j, "A ");
		  i += strlen ("An ");
		  j += strlen ("A ");
		}
#endif
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
	      if (start_comment (lang, buffer + i))
		incomment = 1;
	      if (start_line_comment (lang, buffer + i))
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
  return write_if_common (input, ip, output, op);
}
