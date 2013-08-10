/* Copyright 2013 by Ondřej Bílka <neleai@seznam.cz> */


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

HASH_TYPE code_words[HASH_SIZE];
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
static int
add_word (char *word, HASH_TYPE value)
{
  int h = hash (word) % HASH_SIZE;
  while (code_words[h])
    {
      if (!strcmp (word, names[code_words[h]]))
	return 0;
      h++;
      if (h > HASH_SIZE)
	h = 0;
    }
  code_words[h] = value;
  return 1;
}

static HASH_TYPE
get_word (char *word)
{
  int h = hash (word) % HASH_SIZE;
  while (1)
    {
      if (!code_words[h])
	return 0;
      if (!strcmp (word, names[code_words[h]]))
	return code_words[h];
      h++;
      if (h > HASH_SIZE)
	h = 0;
    }
}
