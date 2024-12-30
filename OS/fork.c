#include <stdio.h>
#include <sys/types.h>
int fork();
int main()
{
    fork();
    fork();
    fork();
    printf("hello\n");
    return 0;
}
