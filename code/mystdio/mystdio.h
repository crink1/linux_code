#pragma once 

#define SIZE 4096
#define FLUSH_NONE 1
#define FLUSH_LINE (1 << 1)
#define FLUSH_ALL  (1 << 2)

typedef struct _myFILE  
{
  char buffer[SIZE];
  int fileno;
  int flag;
  int end;
}myFILE;


extern myFILE *my_fopen(const char *path,const char *mode);

extern int my_fwrite(const char *s, int num,  myFILE *stream);


extern int my_fflush(myFILE *stream);

extern int my_fclose(myFILE *stream);
