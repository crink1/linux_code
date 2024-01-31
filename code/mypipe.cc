#include <iostream>
#include <cassert>
#include <unistd.h>

using namespace std;

int main()
{
    int pipefd[2];
    int n = pipe(pipefd);
    assert(n == 0);

    (void)n;
    return 0;
}