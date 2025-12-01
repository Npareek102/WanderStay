#include <stdio.h>
#include <string.h> // For memcpy

// A single, simple structure for all algorithms
struct Process {
    int pid;
    int arrival_time;
    int burst_time;
    int priority;
    
    // Values to be calculated
    int remaining_burst; // Used by RR
    int completed;       // A flag (0 or 1)
    int completion_time;
    int waiting_time;
    int turnaround_time;
};

// --- 1. HELPER: Prints the final results ---
void findAvgTimes(struct Process p[], int n) {
    int total_wt = 0, total_tat = 0;
    printf("\nProcess\tArrival\tBurst\tCompletion\tTurnaround\tWaiting\n");
    printf("-----------------------------------------------------------------\n");
    for (int i = 0; i < n; i++) {
        p[i].turnaround_time = p[i].completion_time - p[i].arrival_time;
        p[i].waiting_time = p[i].turnaround_time - p[i].burst_time;
        total_wt += p[i].waiting_time;
        total_tat += p[i].turnaround_time;
        
        printf("P%d\t%d\t%d\t%d\t\t%d\t\t%d\n",
               p[i].pid, p[i].arrival_time, p[i].burst_time,
               p[i].completion_time, p[i].turnaround_time, p[i].waiting_time);
    }
    printf("\nAverage Waiting Time = %.2f\n", (float)total_wt / n);
    printf("Average Turnaround Time = %.2f\n", (float)total_tat / n);
}

// --- 2. FCFS (First Come, First Served) ---
void FCFS(struct Process processes[], int n) {
    printf("\n--- FCFS Scheduling ---\n");
    struct Process p[n];
    memcpy(p, processes, n * sizeof(struct Process));

    int current_time = 0;
    int completed_count = 0;

    while (completed_count < n) {
        int next_job_index = -1;
        int earliest_arrival = 9999;

        // Find the *arrived* process with the *earliest arrival time*
        for (int i = 0; i < n; i++) {
            if (p[i].arrival_time <= current_time && p[i].completed == 0) {
                if (p[i].arrival_time < earliest_arrival) {
                    earliest_arrival = p[i].arrival_time;
                    next_job_index = i;
                }
            }
        }

        if (next_job_index == -1) {
            current_time++; // CPU is idle
        } else {
            // Execute the process
            current_time += p[next_job_index].burst_time;
            p[next_job_index].completion_time = current_time;
            p[next_job_index].completed = 1;
            completed_count++;
        }
    }
    findAvgTimes(p, n);
}

// --- 3. SJF (Shortest Job First) ---
void SJF(struct Process processes[], int n) {
    printf("\n--- Non-Preemptive SJF Scheduling ---\n");
    struct Process p[n];
    memcpy(p, processes, n * sizeof(struct Process));

    int current_time = 0;
    int completed_count = 0;

    while (completed_count < n) {
        int next_job_index = -1;
        int shortest_burst = 9999;

        // Find the *arrived* process with the *shortest burst time*
        for (int i = 0; i < n; i++) {
            if (p[i].arrival_time <= current_time && p[i].completed == 0) {
                if (p[i].burst_time < shortest_burst) {
                    shortest_burst = p[i].burst_time;
                    next_job_index = i;
                }
            }
        }

        if (next_job_index == -1) {
            current_time++; // CPU is idle
        } else {
            // Execute the process
            current_time += p[next_job_index].burst_time;
            p[next_job_index].completion_time = current_time;
            p[next_job_index].completed = 1;
            completed_count++;
        }
    }
    findAvgTimes(p, n);
}

// --- 4. Priority (Non-Preemptive) ---
void Priority(struct Process processes[], int n) {
    printf("\n--- Non-Preemptive Priority Scheduling ---\n");
    struct Process p[n];
    memcpy(p, processes, n * sizeof(struct Process));

    int current_time = 0;
    int completed_count = 0;

    while (completed_count < n) {
        int next_job_index = -1;
        int highest_priority = 9999; // Lower number = higher priority

        // Find the *arrived* process with the *highest priority*
        for (int i = 0; i < n; i++) {
            if (p[i].arrival_time <= current_time && p[i].completed == 0) {
                if (p[i].priority < highest_priority) {
                    highest_priority = p[i].priority;
                    next_job_index = i;
                }
            }
        }

        if (next_job_index == -1) {
            current_time++; // CPU is idle
        } else {
            // Execute the process
            current_time += p[next_job_index].burst_time;
            p[next_job_index].completion_time = current_time;
            p[next_job_index].completed = 1;
            completed_count++;
        }
    }
    findAvgTimes(p, n);
}

// --- 5. Round Robin (Pre-emptive) ---
void RoundRobin(struct Process processes[], int n, int quantum) {
    printf("\n--- Round Robin (Quantum = %d) ---\n", quantum);
    struct Process p[n];
    memcpy(p, processes, n * sizeof(struct Process));
    for (int i = 0; i < n; i++) p[i].remaining_burst = p[i].burst_time;

    int current_time = 0;
    int completed_count = 0;

    while (completed_count < n) {
        int job_ran = 0; // Flag to check if CPU was idle
        
        for (int i = 0; i < n; i++) {
            if (p[i].arrival_time <= current_time && p[i].completed == 0) {
                job_ran = 1; // At least one job is ready

                if (p[i].remaining_burst > quantum) {
                    // Run for one quantum
                    current_time += quantum;
                    p[i].remaining_burst -= quantum;
                } else {
                    // Run to completion
                    current_time += p[i].remaining_burst;
                    p[i].completion_time = current_time;
                    p[i].remaining_burst = 0;
                    p[i].completed = 1;
                    completed_count++;
                }
            }
        }
        
        if (job_ran == 0) {
            current_time++; // CPU is idle, tick clock
        }
    }
    findAvgTimes(p, n);
}

// --- MAIN FUNCTION ---
int main() {
    int n, choice, quantum;
    printf("Enter the number of processes: ");
    scanf("%d", &n);

    struct Process processes[n];
    printf("Enter details (arrival burst priority):\n");
    for (int i = 0; i < n; i++) {
        processes[i].pid = i + 1;
        printf("Process P%d: ", i + 1);
        scanf("%d %d %d", &processes[i].arrival_time, &processes[i].burst_time, &processes[i].priority);
        processes[i].completed = 0; // Initialize flag
    }

    do {
        printf("\n--- MENU ---\n");
        printf("1. FCFS\n2. SJF\n3. Priority\n4. Round Robin\n5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: FCFS(processes, n); break;
            case 2: SJF(processes, n); break;
            case 3: Priority(processes, n); break;
            case 4:
                printf("Enter time quantum: ");
                scanf("%d", &quantum);
                RoundRobin(processes, n, quantum);
                break;
            case 5: printf("Exiting.\n"); break;
            default: printf("Invalid choice.\n");
        }
    } while (choice != 5);

    return 0;
}