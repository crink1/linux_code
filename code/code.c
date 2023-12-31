#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
int main()
{

    int fd = open("./bite.txt", O_RDWR | O_CREAT, 0664);
    if (fd < 0) 
    {
      return -1;
    }
    const char* str = "i like linux!!!!!\n";
    write(fd,str,strlen(str));
    close(fd);

    
    fd = open("./bite.txt", O_RDONLY, 0664);
    char buff[128];
    int c = read(fd,buff,strlen(str));
    buff[strlen(str)] = '\0';
    if(c == -1)
    {
      printf("read fail!\n");
    }
    printf("%s",buff);
    close(fd);
  return 0;
}
