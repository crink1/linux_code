#include <stdio.h>
#include "add.h"
#include "sub.h"
int main()
{
    int x = 200;
    int y = 100;
    printf("%d\n", add(x,y));
    printf("%d\n", sub(x,y));
    return 0;
}