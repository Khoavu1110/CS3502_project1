# CS3502_project1
---
## This project consist of two part
1. **Project A: Multi-threading Implementation**
    - Implements a multi-threaded.
    - Demonstrates synchronization.
    - Includes deadlock detection and resolution.
2. **Project B: Inter-Process Communication (IPC)**
    - Enable communication between two processes.
---
## System Requirements:
1. OS: Linux or Windows or Mac
2. Compiler: 'g++' with 'pthread' support
---
## How to compile and run the code
### I. Project A: Multi-threading Implementation
1. Make sure you are in the main branch:
`git status`
2. If you are not in main branch, switch to main branch:
`git checkout main`
3. If you are in main branch, compile the code:
`g++ -std=c++11 main.cpp -pthread -o main`
4. Run the program:
`./main`
### II. Project B: Inter-Process Communication (IPC)
1. Make sure you are in the PartB branch:
`git status`
2. If you are not in PartB branch, switch to PartB branch:
`git checkout main`
3. If you are in PartB branch, compile the code:
`g++ -std=c++11 main.cpp -o main`
4. Run the program:
`./main`
---
## Usage Details
### Project A: Multi-threading
- Waiters generate customer orders.
- Chefs process those order.
- The simmulation runs for a set duration before closing.
### Project B: Inter-Processing Communication (IPC)
- The parent process reads the data from the pipe.
- The child process reads the data from the pipe.
- The communication ends when the parent closes the pipe.
