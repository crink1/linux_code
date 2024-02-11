#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <cstdlib>
#include <sys/types.h>
#include <sys/ipc.h>

const std::string pathname = "prosesspool/shm";
const int project_id = 0x11223344;
using namespace std;

int main()
{
  key_t key = ftok(pathname.c_str(), project_id);
  cout << key << endl;
  return 0;
}


// using namespace std;

// int count = 0;

// void handler(int signo)
// {
//   cout << "signo:" << signo << "cnt：" <<  count << endl;
//   exit(0);
// }

// int main()
// {
//   signal(14, handler);
//   alarm(1);
//   while(true)
//   {
//     count++;
//   }
//   return 0;
// }
