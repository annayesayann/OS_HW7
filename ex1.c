#include <stdio.h>
#include <stdlib.h>

struct Process {
    int pid;
    int arrival_time;
    int burst_time;
    int waiting_time;
    int turnaround_time;
    int response_time;
};

void printGantt(struct Process proc[], int order[], int n) {
    printf("Gantt chart: ");
    for (int k = 0; k < n; k++) {
        int i = order[k];
        int width = proc[i].burst_time * 2;
        printf("|");
        int mid = width / 2 - 1;
        for (int j = 0; j < width; j++) {
            if (j == mid)
                printf("P%d", proc[i].pid);
            else
                printf(" ");
        }
    }
    printf("|\n\n");
}

void printTable(struct Process proc[], int n) {
    printf("PID\tAT\tBT\tWT\tTAT\tRT\n");
    double waitingTime = 0;
    double turnaroundTime = 0;
    double totalRunTime = 0;

    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t%d\t%d\n", 
               proc[i].pid, 
               proc[i].arrival_time, 
               proc[i].burst_time, 
               proc[i].waiting_time, 
               proc[i].turnaround_time, 
               proc[i].response_time);
        waitingTime += proc[i].waiting_time;
        turnaroundTime += proc[i].turnaround_time;
        totalRunTime += proc[i].response_time;
    }

    printf("\nAverage Waiting Time: %.2f", waitingTime / n);
    printf("\nAverage Turnaround Time: %.2f", turnaroundTime / n);
    printf("\nAverage Response Time: %.2f\n", totalRunTime / n);
}

int compareArrival(const void *a, const void *b) {
    const struct Process *proc1 = (const struct Process *)a;
    const struct Process *proc2 = (const struct Process *)b;
    return proc1->arrival_time - proc2->arrival_time;
}

void fcfs(struct Process proc[], int n) {
    qsort(proc, n, sizeof(struct Process), compareArrival);

    int curr_time = 0;
    for (int i = 0; i < n; i++) {
        if (curr_time < proc[i].arrival_time) {
            curr_time = proc[i].arrival_time;
        }
        proc[i].response_time = curr_time - proc[i].arrival_time;
        proc[i].waiting_time = proc[i].response_time;
        curr_time += proc[i].burst_time;
        proc[i].turnaround_time = curr_time - proc[i].arrival_time;
    }

    int order[n];
    for (int i = 0; i < n; i++) {
        order[i] = i;
    }

    printGantt(proc, order, n);
    printTable(proc, n);
}

void sjf(struct Process proc[], int n) {
    int curr_time = 0;
    int completed = 0;
    int finished[n];
    int order[n];

    for (int i = 0; i < n; i++) {
        finished[i] = 0;
    }

    while (completed < n) {
        int index = -1;
        for (int i = 0; i < n; i++) {
            if (proc[i].arrival_time <= curr_time && !finished[i]) {
                if (index == -1 || proc[i].burst_time < proc[index].burst_time) {
                    index = i;
                }
            }
        }

        if (index == -1) {
            curr_time++;
            continue;
        }

        proc[index].response_time = curr_time - proc[index].arrival_time;
        proc[index].waiting_time = proc[index].response_time;
        curr_time += proc[index].burst_time;
        proc[index].turnaround_time = curr_time - proc[index].arrival_time;

        finished[index] = 1;
        order[completed] = index;
        completed++;
    }

    printGantt(proc, order, completed);
    printTable(proc, n);
}

int main() {
    int n;
    printf("Enter number of processes: ");
    scanf("%d", &n);

    struct Process proc[n];
    for (int i = 0; i < n; i++) {
        proc[i].pid = i + 1;
        printf("Enter arrival and burst time for process %d: ", proc[i].pid);
        scanf("%d %d", &proc[i].arrival_time, &proc[i].burst_time);
    }

    fcfs(proc, n);
    sjf(proc, n);

    return 0;
}
