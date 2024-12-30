#include <stdio.h>

int main() {
    int n, i, j, quantum, total_time = 0, total_waiting_time = 0, total_turnaround_time = 0;
    float avg_wt, avg_tat;
    printf("Enter the number of processes: ");
    scanf("%d", &n);
    printf("Enter the time quantum: ");
    scanf("%d", &quantum);

    int arrival_time[n], burst_time[n], remaining_time[n], completion_time[n], waiting_time[n], turnaround_time[n];

    for (i = 0; i < n; i++) {
        printf("Enter Arrival Time and Burst Time for Process %d: ", i + 1);
        scanf("%d %d", &arrival_time[i], &burst_time[i]);
        remaining_time[i] = burst_time[i];
    }

    int time = 0, flag = 0;

    while (1) {
        flag = 0;
        for (i = 0; i < n; i++) {
            if (remaining_time[i] > 0) {
                flag = 1;

                if (remaining_time[i] > quantum) {
                    time += quantum;
                    remaining_time[i] -= quantum;
                } else {
                    time += remaining_time[i];
                    completion_time[i] = time;
                    turnaround_time[i] = completion_time[i] - arrival_time[i];
                    waiting_time[i] = turnaround_time[i] - burst_time[i];
                    remaining_time[i] = 0;
                    total_waiting_time += waiting_time[i];
                    total_turnaround_time += turnaround_time[i];
                }
            }
        }

        if (flag == 0) {
            break;
        }
    }

    printf("\n\nProcess\t Arrival Time\t Burst Time\t Completion Time\t Turnaround Time\t Waiting Time\n");

    for (i = 0; i < n; i++) {
        printf("%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t\t%d\n", i + 1, arrival_time[i], burst_time[i], completion_time[i], turnaround_time[i], waiting_time[i]);
    }

    avg_wt = (float) total_waiting_time / n;
    avg_tat = (float) total_turnaround_time / n;

    printf("\nAverage Turnaround Time: %.2f", avg_tat);
    printf("\nAverage Waiting Time: %.2f", avg_wt);

    return 0;
}
