Operating System simulation 
Author: Vishnu Rampersaud 
Completion Date: 12/14/18

This repository contains a cpp file that simulates some aspects of operating systems. There is no real system programming involved, the whole simulation is based on text inputs that the program recieves from the user. 

Some aspects that are implemented include: 
- preemptive prioriy CPU scheduling 
- FCFS I/O queues
- contiguous memory management with 'best fit' approach 
- Each process is represented by its own PCB 
- All PCB's are stored in a process table 
- We do not want to move around whole PCBs, so we move around the indices of the process in the process table

The user needs to answer these questions before a simulation begins: 
- How much RAM memory is there on the simulated computer? 
	- the program recieves this number in units of bytes
- How many hard disks does the simulated computer have? 
	- The enumeration of the hard disks starts with 0.

After these questions are answered, the simulation begins. 
The program constantly listens for the user inputs. You should NOT ask for a confirmation to enter another input. 
The user inputs signal some system events. The program simulates the corresponding system behavior. 

The possible inputs are:


- A priority memory_size 
	- ‘A’ input means that a new process has been created. This process has a 'priority' priority and requires 'memory_size' bytes of memory. When choosing a PID for a new process start from 1 and go up. Do NOT reuse PIDs of the terminated processes.
- t 
	- The process that is currently using the CPU terminates. It leaves the system immediately. The memory used by this process is released. Once the process is gone from the system it is also gone from the memory.
- d number file_name 
	- The process that currently uses the CPU requests the hard disk 'number'. It wants to read or write the file 'file_name'.
- D number 
	- The hard disk 'number' has finished the work for one process.
- S r 
	- Shows a process currently using the CPU and processes waiting in the readyqueue. For each process its PID and priority is displayed. 
- S i 
	- Shows what processes are currently using the hard disks and what processes are waiting to use them. For each busy hard disk, it shows the process that uses it and its I/O-queue. Displays the filenames (from the d command) for each process. The enumeration of hard disks starts from 0. Processes in I/O-queue are displayed in the correct queue order. Remember that the I/O-queues are FCFS.
- S m 
	- Shows the state of memory. Show the range of memory addresses used by each process in the system.