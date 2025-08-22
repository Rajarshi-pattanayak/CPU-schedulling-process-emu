#include <stdio.h>
#include <limits.h>

struct Process {
    int pid, at, bt, ct, tat, wt, rt, start, priority, remaining;
    int completed, first_exec;
};

void calculate_avg(struct Process p[], int n) {
    float total_tat = 0, total_wt = 0, total_rt = 0;
    for (int i = 0; i < n; i++) {
        total_tat += p[i].tat;
        total_wt += p[i].wt;
        total_rt += p[i].rt;
    }
    printf("\nAvg TAT = %.2f", total_tat / n);
    printf("\nAvg WT  = %.2f", total_wt / n);
    printf("\nAvg RT  = %.2f\n", total_rt / n);
}

void fcfs(struct Process p[], int n) {
    int time = 0;
    for (int i = 0; i < n; i++) {
        if (time < p[i].at) time = p[i].at;
        p[i].start = time;
        p[i].ct = time + p[i].bt;
        p[i].tat = p[i].ct - p[i].at;
        p[i].wt = p[i].tat - p[i].bt;
        p[i].rt = p[i].start - p[i].at;
        time = p[i].ct;
    }
}

void sjf_nonpre(struct Process p[], int n) {
    int completed = 0, time = 0;
    while (completed < n) {
        int idx = -1, min_bt = INT_MAX;
        for (int i = 0; i < n; i++) {
            if (!p[i].completed && p[i].at <= time && p[i].bt < min_bt) {
                min_bt = p[i].bt;
                idx = i;
            }
        }
        if (idx == -1) { time++; continue; }
        p[idx].start = time;
        p[idx].ct = time + p[idx].bt;
        p[idx].tat = p[idx].ct - p[idx].at;
        p[idx].wt = p[idx].tat - p[idx].bt;
        p[idx].rt = p[idx].start - p[idx].at;
        p[idx].completed = 1;
        time = p[idx].ct;
        completed++;
    }
}

void sjf_pre(struct Process p[], int n) {
    int completed = 0, time = 0;
    for (int i = 0; i < n; i++) { p[i].remaining = p[i].bt; p[i].first_exec = 1; }
    while (completed < n) {
        int idx = -1, min_rt = INT_MAX;
        for (int i = 0; i < n; i++) {
            if (!p[i].completed && p[i].at <= time && p[i].remaining < min_rt && p[i].remaining > 0) {
                min_rt = p[i].remaining;
                idx = i;
            }
        }
        if (idx == -1) { time++; continue; }
        if (p[idx].first_exec) {
            p[idx].rt = time - p[idx].at;
            p[idx].first_exec = 0;
        }
        p[idx].remaining--;
        time++;
        if (p[idx].remaining == 0) {
            p[idx].ct = time;
            p[idx].tat = p[idx].ct - p[idx].at;
            p[idx].wt = p[idx].tat - p[idx].bt;
            p[idx].completed = 1;
            completed++;
        }
    }
}

void priority_pre(struct Process p[], int n) {
    int completed = 0, time = 0;
    for (int i = 0; i < n; i++) { p[i].remaining = p[i].bt; p[i].first_exec = 1; }
    while (completed < n) {
        int idx = -1, best_pr = INT_MAX;
        for (int i = 0; i < n; i++) {
            if (!p[i].completed && p[i].at <= time && p[i].priority < best_pr && p[i].remaining > 0) {
                best_pr = p[i].priority;
                idx = i;
            }
        }
        if (idx == -1) { time++; continue; }
        if (p[idx].first_exec) {
            p[idx].rt = time - p[idx].at;
            p[idx].first_exec = 0;
        }
        p[idx].remaining--;
        time++;
        if (p[idx].remaining == 0) {
            p[idx].ct = time;
            p[idx].tat = p[idx].ct - p[idx].at;
            p[idx].wt = p[idx].tat - p[idx].bt;
            p[idx].completed = 1;
            completed++;
        }
    }
}

void round_robin(struct Process p[], int n, int tq) {
    int completed = 0, time = 0, queue[100], front = 0, rear = 0, in_queue[100] = {0};
    for (int i = 0; i < n; i++) { p[i].remaining = p[i].bt; p[i].first_exec = 1; }
    queue[rear++] = 0; in_queue[0] = 1;
    while (completed < n) {
        if (front == rear) { time++; for (int i = 0; i < n; i++) if (!p[i].completed && p[i].at <= time && !in_queue[i]) { queue[rear++] = i; in_queue[i] = 1; } continue; }
        int idx = queue[front++];
        if (p[idx].first_exec) {
            p[idx].rt = time - p[idx].at;
            p[idx].first_exec = 0;
        }
        int exec = (p[idx].remaining < tq) ? p[idx].remaining : tq;
        p[idx].remaining -= exec;
        time += exec;
        for (int i = 0; i < n; i++) if (!p[i].completed && p[i].at <= time && !in_queue[i]) { queue[rear++] = i; in_queue[i] = 1; }
        if (p[idx].remaining > 0) queue[rear++] = idx;
        else {
            p[idx].ct = time;
            p[idx].tat = p[idx].ct - p[idx].at;
            p[idx].wt = p[idx].tat - p[idx].bt;
            p[idx].completed = 1;
            completed++;
        }
    }
}

int main() {
    printf("Registration number:- 24BCE1585 Name:- Rajarshi Pattanayak\n");
    int n, choice, tq;
    printf("Enter number of processes: ");
    scanf("%d", &n);
    struct Process p[n];
    printf("Enter AT and BT for each process:\n");
    for (int i = 0; i < n; i++) {
        p[i].pid = i+1;
        scanf("%d %d", &p[i].at, &p[i].bt);
        p[i].completed = 0;
    }
    printf("\n1. FCFS\n2. SJF Non-preemptive\n3. SJF Preemptive\n4. Priority Preemptive\n5. Round Robin\nEnter choice: ");
    scanf("%d", &choice);
    if (choice == 4) {
        printf("Enter priorities: ");
        for (int i = 0; i < n; i++) scanf("%d", &p[i].priority);
    }
    if (choice == 5) {
        printf("Enter Time Quantum: ");
        scanf("%d", &tq);
    }
    switch (choice) {
        case 1: fcfs(p, n); break;
        case 2: sjf_nonpre(p, n); break;
        case 3: sjf_pre(p, n); break;
        case 4: priority_pre(p, n); break;
        case 5: round_robin(p, n, tq); break;
    }
    printf("\nPID\tAT\tBT\tCT\tTAT\tWT\tRT\n");
    for (int i = 0; i < n; i++)
        printf("P%d\t%d\t%d\t%d\t%d\t%d\t%d\n", p[i].pid, p[i].at, p[i].bt, p[i].ct, p[i].tat, p[i].wt, p[i].rt);
    calculate_avg(p, n);
    return 0;
}
