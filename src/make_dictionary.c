/*
 Print word tuples that are not identical in files
 */
#include <string.h>
#include <stdio.h>
#define MAX_LEN 100
int main(int argc,char ** argv){
  int i,j;
  FILE *fp[2];
  unsigned char word[2][MAX_LEN];
  for(i=0;i<2;i++){
    fp[i]=fopen(argv[i+1],"r");
  }
  while (1) {
    for (i=0;i<2;i++){
    if (!fgets(word[i],MAX_LEN,fp[i]))
      return 0;
      for(j=0;word[i][j];j++){
        if (word[i][j]==' ') 
          word[i][j]='_';
        if (word[i][j]=='\n') 
          word[i][j]=0;
      }
    }
    if (strcmp(word[0],word[1])){
      printf("%s ",word[0]);
      for(i=0;i<20-strlen(word[0]);i++)
        printf(" ");
      printf("%s\n",word[1]);
    }
  }
}
