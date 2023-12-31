#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
//int main()
//
//{
//    printf("before\n");
//    pid_t pid = fork();
//    printf("after\n");
//    if(pid == 0)
//    {
//        printf("i m c pid:%d\n",getpid());
//    }
//    else
//    {
//        printf("i m f pid：%d\n",getpid());
//    }
//    
//
//    return 0;
//}
//int main()
//{
//  pid_t pid = fork();
//  if(0 == pid)
//  {
//    int n = 5;
//    while(n)
//    {
//      printf("i m c pid:%d\n",getpid());
//      n--;
//      sleep(1);
//    }
//    exit(10);
//  }
//  else
//  {
//    printf("wait befor\n");
//    
//    int status = 0;
//    //sleep(7);
//    pid_t wid = waitpid(pid,&status,0);
//    printf("wait after\n");
//    printf("c ecode:%d,c sig:%d,waitpid:%d\n",status >> 8 & 0xFF,status & 0x7F,wid);
//    //sleep(2);
//  }
//
//  return 0;
//}
int main()
{
  FILE* fd = fopen("bite.txt","w+");
  if(fd == NULL)
  {
    printf("fopen error\n");
  }
  const char* str = "linux so easy!\n";
  
  fputs(str,fd);
  fclose(fd);

  FILE* fd1 = fopen("bite.txt","r");
  char buff[1024];
  fgets(buff,1024,fd1);
  fclose(fd1);
  printf("%s",buff);
  return 0;
}
