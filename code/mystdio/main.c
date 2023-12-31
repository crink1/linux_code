
#include "mystdio.h"
#include<stdio.h>
#include<string.h>
#include<unistd.h>
int main()
{
  myFILE *fp = my_fopen("./log.txt","w");
  if(fp == NULL)
  {
    perror("my_fopen fail\n");
    return 1;
  }
  int cnt = 10;
  const char *str = "hello linux!";

  while(cnt)
  {
    my_fwrite(str,strlen(str),fp);
    sleep(1);
    cnt--;
  }
  my_fclose(fp);
  return 0;
}
