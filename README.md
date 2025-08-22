# OS Lab - CPU Scheduling Algorithms Simulator

This project is a C implementation of various **CPU Scheduling Algorithms** commonly taught in Operating Systems (OS) courses.  
It allows you to simulate how different scheduling techniques handle process execution and compare performance metrics.

---

## Features

- **First Come First Serve (FCFS)**
- **Shortest Job First (SJF - Non-preemptive)**
- (Extendable: Priority Scheduling, Round Robin, SJF Preemptive, etc.)
- Calculates for each process:
  - Completion Time (CT)
  - Turnaround Time (TAT)
  - Waiting Time (WT)
  - Response Time (RT)
- Computes **average TAT, WT, RT** across all processes

---

## Compilation & Execution

### Compile:
```bash
gcc Oslab.c -o oslabs
