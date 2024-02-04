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
  int wfd = open(FILENAME, O_WRONLY);
  if(wfd < 0)
  {
    return -1;
  }

  string str;
  while(true)
  {
    cout << "输入：" << endl;
    getline(cin, str);

    ssize_t s = write(wfd, str.c_str(), str.size());
    if(s < 0)
    {
      break;
    }
  }

  close(wfd);
  cout << "cquit" <<endl;
  return 0;
}
