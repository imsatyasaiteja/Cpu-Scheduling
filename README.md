# Cpu-Scheduling
Cpu Scheduling Program

  Code Explanation:
    A class Process is declared. This contains the Process Id, Arrival Time, Burst Time, Completion Time, Turn Around Time, Waiting Time, Response Time, Virtual Run Time, Time Slice, Static Priority. It has a constructor that takes Process Id, Arrival Time, Burst Time and Nice as it's input arguments. It also has necessary Member functions.
    
    A class Process_Creator is declared. This contains an vector which holds objects of the class Process. This vector is is created dynamically using new. I named it as array.
    The default constructor of Process_Creator contains a for loop to assign Arrival Time, Burst Time and Nice values to each process. To assign random values, srand() and mt19937 have been used.
    
    A class MinHeap is declared. This contains a pointer to vector of type Process and necessary member functions like MinHeapify, Build Min Heap, Get Min, Pop.
    
    Next a compare function is delcared.This is used while comparing the virtual run times of processes in case of Completely Fair Scheduler.
    
    A class Scheduler is declared. This contains a dynamic vector pointer of type Process. This is our ready queue. All Processes created by the Process_Creator and stored in it's array are pushed into the ready queue according the Scheduling algorithm.
    Scheduler constructor takes a the address of a Process_Creator Object as parameter. This constructor makes it's private data member array pointer to point towards the address of Dynamically created array of Processes in the Process_Creator's object.
    This class also contains the fcfs, rr, cfs member functions. They take the running queue as input parameter and push the appropriate processes from ready queue to running queue accordingly.
    
    Simulator class contains a dynamic vector pointer of type Process. This is our running queue. It also has array and ready queue pointers. These two will be used to access the locations of array and ready queue created by other class objects.
