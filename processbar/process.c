#include"process.h"
const char cc[] = "|/-\\";


void process_v1()
{
    char bar[101] = {0};
    int sz = strlen(cc);
    int i = 1;
    for(i = 0;i <= 100; i++)
    {
        usleep(1000*20);
        printf("[%-100s][%d%%][%c]\r",bar,i,cc[i%sz]);
        bar[i] = '#';
    
    }
    printf("\n");
}
void process_v2()
{
    char bar[101] = {0};
    int sz = strlen(cc);
    int i = 1;
    for(i = 0;i <= 100; i++)
    {
        usleep(1000*20);
        printf("[%-100s][%d%%][%c]\r",bar,i,cc[i%sz]);
        bar[i] = '#';
    
    }
    printf("\n");
}
