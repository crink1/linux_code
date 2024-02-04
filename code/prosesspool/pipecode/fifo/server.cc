#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>
#include <cerrno>

using namespace std;
#include "comm.hpp"

int main()
{
  int n = mkfifo(FILENAME,0664); 
  if( n < 0 )
  {
    return -1;
  }


  int rfd = open(FILENAME, O_RDONLY);
  if(rfd < 0)
  {
    cerr << "errno" << errno << ", errstring" << strerror(errno) << endl;

  }



  char buffer[1024];
  while(true)
  {
    ssize_t s = read(rfd, buffer, sizeof(buffer) - 1);
    if( s > 0) 
    {
      buffer[s] = 0;
      cout << "client say:" << buffer << endl;
    }
    else if(s == 0)
    {
      cout << "quit" << endl;
      break;
    }
  }



  close(rfd);
  cout << "close" << endl;

  return 0;
}
