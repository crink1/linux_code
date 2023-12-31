#include"mystdio.h"
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<errno.h>
#include<stdlib.h>
#include<unistd.h>

#define DEL_MODE 0666

myFILE *my_fopen(const char *path,const char *mode)
{
  int fd = 0;
  int flag = 0;
  if(strcmp(mode, "r") == 0)
  {
    flag |= O_RDONLY;
  }
  else if(strcmp(mode, "w") == 0)
  {
    flag |= (O_WRONLY | O_CREAT | O_TRUNC); 
  }
  else if(strcmp(mode, "a") == 0)
  {
    flag |= (O_WRONLY | O_CREAT | O_APPEND);
  }
  else
  {
    //n
  }
  if(flag & O_CREAT)
  {
    fd = open(path, flag, DEL_MODE);
  
  }
  else
  {
    fd = open(path,flag);
  }
  if(fd < 0)
  {
    errno = 2;
    return NULL;
  }
  
  myFILE *fp = (myFILE*)malloc(sizeof(myFILE));
  if(!fp)
  {
    errno = 3;
    return NULL;
  }
  fp->end = 0;
  fp->flag = FLUSH_LINE;
  fp->fileno = fd;
  return fp;
}

int my_fwrite(const char *s, int num,  myFILE *stream)
{
  memcpy(stream->buffer + stream->end, s, num);

  stream->end += num;

  if((stream->flag & FLUSH_LINE) && stream->end > 0 && stream->buffer[stream->end - 1] != '\0')
  {
    my_fflush(stream);
  }
  return 0;
}

int my_fflush(myFILE *stream)
{
  
  if(stream->end > 0)
  {
    write(stream->fileno, stream->buffer, stream->end);
    stream->end = 0;
  }
  return 0;
}
int my_fclose(myFILE *stream)
{

    
    my_fflush(stream);
    return close(stream->fileno);
}
