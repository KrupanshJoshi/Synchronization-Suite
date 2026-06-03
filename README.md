# SynchronizationSuite

SynchronizationSuite is an Operating System simulation project that demonstrates famous synchronization and concurrency problems using C programming, POSIX threads, semaphores, mutexes, and Python-based visualization tools.

The project provides real-time graphical simulations for understanding how synchronization mechanisms work internally in multi-threaded environments.

---

## Features

- Real-time visualization of synchronization algorithms
- Multi-threaded implementations using POSIX Threads
- Semaphore and Mutex based synchronization
- Deadlock simulation and monitoring
- GUI launcher using Python Tkinter
- Process state tracking and visualization

---

## Implemented Problems

### 1. Dining Philosophers Problem
- Simulates philosophers competing for forks
- Demonstrates deadlock handling
- Includes AI-enhanced deadlock probability reduction
- Real-time process state visualization

Files:
- `dp.c`
- `dp_ai.c`
- `viz.py`

---

### 2. Producer Consumer Problem
- Uses bounded buffer implementation
- Synchronization using semaphores
- Real-time buffer visualization

Files:
- `pc.c`
- `visualizer.py`

---

### 3. Reader Writer Problem
- Multiple readers can access simultaneously
- Writers get exclusive access
- Reader and writer state monitoring

Files:
- `rw.c`
- `rw_viz.py`

---

## Technologies Used

### Programming Languages
- C
- Python

### Concepts Used
- POSIX Threads (pthreads)
- Mutex Locks
- Semaphores
- Deadlock Detection
- Process Synchronization
- Concurrency Control

### GUI & Visualization
- Python Tkinter
- Real-time File Monitoring

---

## Project Structure

```bash
SynchronizationSuite/
│
├── dp.c                # Dining Philosophers implementation
├── dp_ai.c             # AI-enhanced Dining Philosophers
├── pc.c                # Producer Consumer implementation
├── rw.c                # Reader Writer implementation
│
├── main.py             # Main GUI launcher
├── viz.py              # Dining Philosophers visualization
├── visualizer.py       # Producer Consumer visualization
├── rw_viz.py           # Reader Writer visualization
├── graph.py            # Graph plotting utility
│
├── status.txt
├── buffer_log.txt
├── pc_status.txt
├── rw_status.txt
│
└── rl_results.txt
