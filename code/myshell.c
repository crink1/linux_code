#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/stat.h>
#include<string.h>
#include<stdlib.h>
#include<fcntl.h>
#include<ctype.h>

#define NUM 1024
#define SIZE 64
#define SEP " "
//#define Debug 1

//redir
#define NoneRedir 0
#define OutputRedir 1
#define AppendRedir 2
#define InputRedir 3

int redir = NoneRedir;
char *filename = NULL;


char cwd[1024];
char enval[1024];
int lastcode = 0;

const char* getUsername()
{
  const char* name = getenv("USER");
  if(name)
  {
    return name;
  }
  else
  {
    return "none";
  }
}

const char *getHostname()
{
  const char* hostname = getenv("HOSTNAME");

  if(hostname)
  {
    return hostname;
  }
  else
  {
    return "none";
  }
}


const char *getCwd()
{
  const char *cwd = getenv("PWD");
  
  if(cwd)
  {
    return cwd;
  }
  else
  {
    return "none";
  }

}






int getusercommand(char* command,int n)
{
  printf("[%s@%s %s]#",getUsername(),getHostname(),getCwd());
  char *r = fgets(command,n,stdin);
  if(r == NULL)
  {
    return -1;
  }
  command[strlen(command) - 1] = '\0';

  return strlen(command);

}


void commandSplit(char *in,char **out)
{
  int argc = 0;
  out[argc++] = strtok(in,SEP);

  while(out[argc++] = strtok(NULL,SEP))
  {
    ;
  }

#ifdef Debug
  for(int i = 0; out[i]; i++)
  {
    printf("%d:%s\n",i,out[i]);
  }

#endif
}


int execute(char** argv)
{
  pid_t id = fork();
  if(id == 0)
  {
    if(redir == InputRedir)
    {
      int fd = open(filename,O_RDONLY);
      dup2(fd,0);
    }
    else if(redir == OutputRedir)
    {
      int fd = open(filename,O_WRONLY | O_CREAT | O_TRUNC, 0664);
      dup2(fd,1);
    }
    else if(redir == AppendRedir)
    {
      int fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0664);
      dup2(fd,1);
    }
    execvp(argv[0],argv);
    exit(1);
  }
  else if(id < 0)
  {
    return -1; 
  }
  else
  {
    int status = 0;
    pid_t rid = waitpid(id,&status,0);
    if(rid > 0)
    {
      lastcode = WEXITSTATUS(status);
    }
  }
  return 0;
}


void cd(const char * path)
{
    chdir(path);
    char  tmp[1024];
    getcwd(tmp,sizeof(tmp));
    sprintf(cwd,"PWD=%s",tmp);
    putenv(cwd);

}


int DoBuildin(char **argv)
{
  if(strcmp(argv[0],"cd") == 0)
  {
    char *path = NULL;
    if(argv[1] == NULL)
    {
      path = ".";
    }
    else
    {
      path  = argv[1];
    }
    cd(path);
    return 1;
  }
  else if(strcmp(argv[0],"export") == 0)
  {
    if(argv[1] == NULL)
    {
      return 1;
    }
    strcpy(enval,argv[1]);
    putenv(enval);
    return 1;
  }
  else if(strcmp(argv[0], "echo") == 0)
  {
    char *val = argv[1] + 1;
    if(strcmp(val,"?") == 0)
    {
      printf("%d\n",lastcode);
      lastcode = 0;
    }
    else
    {
      printf("%s\n",getenv(val));
    }
    return 1;
  }
  else if(0){}



  return 0;
}

#define SkipSpace(pos) do{while(isspace(*pos)){pos++;}}while(0);

void checkredir(char usercommand[],int len)
{
  char *start = usercommand;
  char *end = usercommand + len - 1;
  while(end > start)
  {
    if(*end == '>')
    {
      if(*(end - 1) == '>')
      {
        *(end - 1) = '\0';
        filename = end + 1;
        SkipSpace(filename);
        redir = AppendRedir;
        break;
      }
      else
      {
        *end = '\0';
        filename = end + 1;
        SkipSpace(filename);
        redir = OutputRedir;
        break;
      }
    }
    else if(*end == '<')
    {
      *end = '\0';
      filename = end + 1;
      SkipSpace(filename);
      redir = InputRedir;
      break;
    }
    else
    {
      end--;
    }
  }
}



int main()
{
  while(1)
  {
    redir = NoneRedir;
    filename = NULL;
    char entercommand[NUM];
    char *argv[SIZE];

    int n = getusercommand(entercommand,sizeof(entercommand));

   if(n <= 0)
   {
     continue;
   }
   //判断是否有重定向
   checkredir(entercommand,strlen(entercommand));

    //分割字符串
    
    commandSplit(entercommand,argv);
    //处理内建命令
     n = DoBuildin(argv);
    execute(argv);

    if(n)
    {
      continue;
    }
  }
  return 0;
}
