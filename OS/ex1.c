#include <stdio.h>

int main()

{

    int pid[15], at[15], bt[15], v,i,n;
    printf("Enter the number of processes: ");
    scanf("%d",&n);
    printf("Enter process id of all the processes: ");
    for(int i=0;i<n;i++)
    {
        scanf("%d",&pid[i]);
    }
    printf("Enter arrival time of all the processes: ");
    for(int i=0;i<n;i++)
    {
        scanf("%d",&at[i]);
    }
    printf("Enter burst time of all the processes: ");
    for(int i=0;i<n;i++)
    {	
        scanf("%d",&bt[i]);
    }
    int ct[n];
    ct[0]=bt[0];
    for(i=1; i<n; i++)
    {
        ct[i]= ct[i-1]+bt[i];
    }
    int tt[n];
    tt[0]=ct[0]-at[0];
    for(i=1; i<n; i++)
    {
        tt[i]= ct[i]-at[i];
    }
    int wt[n];
    wt[0]=0;
    for(i=1; i<n; i++)
    {
        wt[i]= tt[i]-bt[i];
    }
    printf("Process ID     Arrival Time     Burst Time     Completion Time     TurnAround Time     Waiting Time\n");
    float twt=0.0;
    float tat= 0.0;
    for(i=0; i<n; i++)
    {
        printf("%d\t\t", pid[i]);
        printf("%d\t\t", at[i]);
        printf("%d\t\t", bt[i]);
        printf("%d\t\t\t", ct[i]);
        printf("%d\t\t\t", tt[i]);
        printf("%d\t\t\t", wt[i]);
        printf("\n");
        twt += wt[i];
        tat += tt[i];
    }
    float att,awt;
    awt = twt/n;
    att = tat/n;
    printf("Avg. waiting time= %f\n",awt);
    printf("Avg. turnaround time= %f",att);

}
