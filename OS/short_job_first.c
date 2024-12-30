#include<stdio.h>

int main() {
    int n, i, j, temp, time = 0, total_time = 0;
    float avg_wt = 0, avg_tat = 0;
    printf("Enter the number of processes: ");
    scanf("%d", &n);

    int burst_time[n], arrival_time[n], completion_time[n], waiting_time[n], turnaround_time[n], remaining_time[n];

    for (i = 0; i < n; i++) {
        printf("Enter Arrival Time and Burst Time for Process %d: ", i + 1);
        scanf("%d %d", &arrival_time[i], &burst_time[i]);
        remaining_time[i] = burst_time[i];
    }

    for (i = 0; i < n - 1; i++) {
        for (j = i + 1; j < n; j++) {
            if (burst_time[i] > burst_time[j]) {
                temp = burst_time[i];
                burst_time[i] = burst_time[j];
                burst_time[j] = temp;

                temp = arrival_time[i];
                arrival_time[i] = arrival_time[j];
                arrival_time[j] = temp;
            }
        }
    }

    int min_burst_time = 0, shortest_job = 0, flag = 0;

    for (time = 0; ; time++) {
        min_burst_time = n + 1;
        shortest_job = -1;

        for (i = 0; i < n; i++) {
            if ((remaining_time[i] > 0) && (remaining_time[i] < min_burst_time) && (arrival_time[i] <= time)) {
                min_burst_time = remaining_time[i];
                shortest_job = i;
                flag = 1;
            }
        }

        if (flag == 0) {
            printf("\nAll processes are completed.");
            break;
        }

        remaining_time[shortest_job]--;

        if (remaining_time[shortest_job] == 0) {
            completion_time[shortest_job] = time + 1;
            turnaround_time[shortest_job] = completion_time[shortest_job] - arrival_time[shortest_job];
            waiting_time[shortest_job] = turnaround_time[shortest_job] - burst_time[shortest_job];
            avg_tat += turnaround_time[shortest_job];
            avg_wt += waiting_time[shortest_job];
            flag = 0;
        }
    }

    printf("\n\nProcess\t Arrival Time\t Burst Time\t Completion Time\t Turnaround Time\t Waiting Time\n");

    for (i = 0; i < n; i++) {
        printf("%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t\t%d\n", i + 1, arrival_time[i], burst_time[i], completion_time[i], turnaround_time[i], waiting_time[i]);
        total_time += waiting_time[i];
    }

    avg_wt = (float) total_time / n;
    avg_tat = (float) avg_tat / n;

    printf("\nAverage Turnaround Time: %.2f", avg_tat);
    printf("\nAverage Waiting Time: %.2f", avg_wt);

    return 0;
}
