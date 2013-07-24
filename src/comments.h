#define LANG_C 1
#define LANG_SH 2
#define LANG_PLAIN 3
#define LANG_ADA 4
#define LANG_FORTRAN 5
#define LANG_OCAML 6

int
find_language (char *file)
{
  if (TEST_EXTENSION (file, ".txt") ||
      TEST_EXTENSION (file, ".htm") ||
      TEST_EXTENSION (file, ".html") ||
      TEST_EXTENSION (file, ".texi"))
    return LANG_PLAIN;
  if (TEST_EXTENSION (file, ".py") ||
      TEST_EXTENSION (file, ".sh") ||
      TEST_EXTENSION (file, ".rb"))
    return LANG_SH;
  if (TEST_EXTENSION (file, ".ads") ||
      TEST_EXTENSION (file, ".adb"))
    return LANG_ADA;
  if (TEST_EXTENSION (file, ".f") ||
      TEST_EXTENSION (file, ".f90") ||
      TEST_EXTENSION (file, ".f95"))
    return LANG_FORTRAN;
  if (TEST_EXTENSION (file, ".ml") ||
      TEST_EXTENSION (file, ".mli"))
    return LANG_OCAML;
  return LANG_C;
}

int
start_comment (int lang, char *s)
{
  if (lang == LANG_C && !cmp (s, "/*"))
    return 1;
  if (lang == LANG_FORTRAN && !cmp (s, "(*"))
    return 1;
  if (lang == LANG_OCAML && !cmp (s, "(*")) // TODO nesting.
    return 1;
  return 0;
}
int
end_comment (int lang, char *s)
{
  if (lang == LANG_C && !cmp (s, "*/"))
    return 1;
  if (lang == LANG_FORTRAN && !cmp (s, "*)"))
    return 1;
  if (lang == LANG_OCAML && !cmp (s, "*)"))
    return 1;
  return 0;
}
int
start_line_comment (int lang, char *s)
{
  if (lang == LANG_C && !cmp (s, "//"))
    return 1;
  if (lang == LANG_SH && !cmp (s, "#"))
    return 1;
  if (lang == LANG_ADA && !cmp (s, "--"))
    return 1;
  if (lang == LANG_FORTRAN && !cmp (s, "!"))
    return 1;
  return 0;
}
