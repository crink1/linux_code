#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <cstdlib>
using namespace std;

int count = 0;

void handler(int signo)
{
  cout << "signo:" << signo << "cnt：" <<  count << endl;
  exit(0);
}

int main()
{
  signal(14, handler);
  alarm(1);
  while(true)
  {
    count++;
  }
  return 0;
}
