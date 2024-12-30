#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>

//main function begins
int main()
{
 fork();
 printf("The process is created using fork() system call PID=%d\n",getpid());
 return 0;
}
//using fork() system call single time
