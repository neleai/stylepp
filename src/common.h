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
#define one_from(x,y) (x && strchr(y,x))

#define START_TEST \
  char strbuf[1000];strcpy(strbuf,argv[1]);\
  if (!fopen(strcat(dirname(strbuf), "/.indent.on"),"r"))\
    return 1;

#define WRITE_IF_COMMON\
  if ((ip - input == op - output) && !memcmp (input, output, ip - input))\
    return 1;\
  else\
    {\
      printf ("%s", output);\
      return 0;\
    }

