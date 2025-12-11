# **Scheduler Simulator – User Guide**

## What is this?

A visual tool to simulate CPU scheduling algorithms.
See how different algorithms schedule processes with colorful Gantt charts!

## Installation
**Step 1: Install Requirements**

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
**Step 2: Build the Program**
```bash
cd scheduler-simulator
make
```
## Running the Program
```bash
./simulateur config/process.txt
```

Or simply:
```bash
make run
```
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

- waiting times

- turnaround times

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
