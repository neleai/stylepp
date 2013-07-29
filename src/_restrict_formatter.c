#define BUFSIZE 10000
#include "common.h"
int
main (int argc, char **argv)
{
  char *oldfile = argv[1];
  char *newfile = argv[2];
  char *hash = argv[3];
  char *blamefile = argv[4];
  FILE *fold = fopen (oldfile, "r"), *fnew = fopen (newfile, "r"), *fblame = fopen (blamefile, "r");
  char oldbuf[BUFSIZE], newbuf[BUFSIZE], blamebuf[BUFSIZE];
  while (1)
    {
      if (!fgets (oldbuf, BUFSIZE, fold))
	return 0;
      if (!fgets (newbuf, BUFSIZE, fnew))
	return 1;
      if (!fgets (blamebuf, BUFSIZE, fblame))
	return 1;

      if (!memcmp (hash, blamebuf, 8))
	printf ("%s", newbuf);
      else
	printf ("%s", oldbuf);
    }
}
