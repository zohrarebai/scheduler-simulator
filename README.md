# Scheduler Simulator – User Guide

## Overview
The Scheduler Simulator is a visual tool for simulating different CPU scheduling algorithms. It provides an interactive interface with Gantt chart visualization, process management, and performance metrics analysis.

## Features

- **Multiple Scheduling Algorithms:** FIFO, Round Robin, Priority, and Multilevel Queue
- **Interactive Gantt Chart:** Visual representation of process execution timeline
- **Process Table:** View all loaded processes with their attributes
- **Performance Metrics:** Calculate waiting time and turnaround time
- **Configuration Generator:** Automatically generate test cases

## Prerequisites
Before running the program, ensure you have the following installed:

**Required Software**

- **GCC Compiler** (version 7.0 or higher)
- **GTK+ 3.0** development libraries
- **pkg-config** utility
- **Make** build tool

## Installation
**Step 1: Install Requirements on Different Systems**

**Ubuntu/Debian**
```bash
sudo apt-get update
sudo apt-get install build-essential libgtk-3-dev pkg-config
```
**Fedora**
```bash
sudo dnf install gcc make gtk3-devel pkg-config
```

**macOS**
```bash
brew install gtk+3 pkg-config
```
**Step 2: Clone or Download the Repository**
```bash
git clone <repository-url>
cd scheduler-simulator
```
**Step 3: Build the Program**
```bash
make clean
make
```
This will compile all source files and create an executable named `simulateur`.
## Running the Program
**Basic Usage:** Run the simulator with the default configuration file:
```bash
./simulateur config/process.txt
```
The program requires exactly one argument: the path to a configuration file.

Or simply:
```bash
make run
```
This automatically runs `./simulateur config/process.txt`.
## How to Use
**1. Start the Program**

The window opens with a table of processes

Default algorithm (FIFO) runs automatically

**2. Try Different Algorithms**

Click any button: FIFO, Round Robin, Priority, Multilevel

For Round Robin / Multilevel:

- enter quantum value (example: 3)

**3. View Performance**

Click “Métriques” -> You can see:

- **waiting times (WT):** Time a process spends waiting in the ready queue `= Turnaround Time - Execution Time`

- **turnaround times (TT):** Total time from arrival to completion `= Completion Time - Arrival Time`

Lower numbers = better performance

**4. Generate New Tests**

- Click “Générer fichier”

- Enter number of processes

- Enter ranges

- Click “Générer” to create a new test case

## Configuration File Format

**Edit config/process.txt :**
```python
#Format: name, arrival_time, execution_time, priority

P1,0,3,2
P2,4,4,2
P3,0,5,3
P4,2,4,1
```

Rules:

- Lines starting with # = comments

- Format: Name,Arrival,Execution,Priority

- No spaces around commas

- All numbers must be positive integers

## Algorithms Explained
- **FIFO** (First In, First Out)

Executes processes in order of arrival

Simple and fair

No parameters needed

- **Round Robin**

Each process gets equal time slices (quantum)

Good for interactive systems

Recommended quantum: 2 → 5

- **Priority**

Higher priority = runs first

Can interrupt low-priority processes

Uses priority values from config file

- **Multilevel**

Multiple queues by priority

Highest-priority queue runs first

Quantum used for lower queues

## Understanding the Gantt Chart

- Colored blocks → execution

- Numbers below → time markers

- Names inside blocks → running process

- Bottom list → waiting queue

## Common Problems
- **“Can't find gtk/gtk.h”**
```bash
sudo apt-get install libgtk-3-dev
```

- **“Can't open config/process.txt”**

✔️ Check file exists
✔️ Check you're in the correct directory

**Program crashes**

- Incorrect config format

- Missing commas

- Negative or empty values

## What is pkg-config?

A helper tool that tells the compiler where libraries are located.

Used because GTK+ requires:

header file paths

library paths

compilation options

pkg-config provides these automatically.

## License

MIT License — feel free to use for educational purposes.
