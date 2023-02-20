# Cpu Scheduling

A C++ Program that can create an array of processes, assign random arrival time and burst time, perform Cpu Scheduling based on the user input, and finally output a text file that contains the completion time, turn around time, waiting time and response time for the processes.

## Code Explanation

- A class Process is declared. This contains the Process Id, Arrival Time, Burst Time, Completion Time, 
Turn Around Time, Waiting Time, Response Time, Virtual Run Time, Time Slice, Static Priority. It has 
a constructor that takes Process Id, Arrival Time, Burst Time and Nice as it's input arguments. 
It also has necessary Member functions.
        
- A class Process_Creator is declared. This contains an vector which holds objects of the class Process. 
This vector is created dynamically using new. I named it as "array".

- The default constructor of Process_Creator contains a for loop to assign Arrival Time, Burst Time and 
Nice values to each process. To assign random values mt19937 has been used.

- A class MinHeap is declared. This contains a pointer to vector of type Process and necessary member 
functions like MinHeapify, Build Min Heap, Get Min, Pop.
    
- Next a compare function is delcared. This is used while comparing the virtual run times of processes 
in case of Completely Fair Scheduler.

- A class Scheduler is declared. This contains a dynamic vector pointer of type Process. This is our 
ready queue. All Processes created by the Process_Creator and stored in it's array are pushed into the 
ready queue according to the Scheduling algorithm. Scheduler constructor takes the address of a 
Process_Creator Object as parameter. This constructor makes it's private data member array pointer to 
point towards the address of Dynamically created array of Processes in the Process_Creator's object.
This class also contains the fcfs, rr, cfs member functions. They take the running queue as input 
parameter and push the appropriate processes from ready queue to running queue accordingly.

- Simulator class contains a dynamic vector pointer of type Process. This is our running queue. It also 
has array and ready queue pointers. These two will be used to access the locations of array and ready 
queue created by other class objects. Also sch is a pointer to Scheduler object is declared. This 
pointer location is set in the Simulator constructor and I will be using this to access the fcfs, rr,
cfs, member functions when needed. 
Simulator class also has Run, runFCFS, runCFS, runRR, CaptureValues, printProcesses member functions.


- runFCFS Algorithm :
Minheap is performed on the processes array. The process with minimum arrival time is kept at the top
of the min-heap. 
Sometimes the processes may not arrive at 0 milliseconds, so the currentTime is equated to the process
with minimum arrival time.
Next, we run a while loop till simulation time. In this loop the following things are done.
Another while loop is run. In this, we check the processes whose arrival time is less than current time 
and push them in a sequential order into the ready queue, and pop out from the array.
Next we pass running queue as pass by reference into fcfs function present in the scheduler class. This 
fcfs function, has access to the ready queue. So, it pushes the first process into running queue and
removes it from ready queue. Now, the Response time, Completion time are set.
This loop runs till the processes array and ready queue become empty and all processes go into the running
queue.

- runRR Algorithm :
Minheap is performed on the processes array. The process with minimum arrival time is kept at the top
of the min-heap.
Sometimes the processes may not arrive at 0 milliseconds, so the currentTime is equated to the process
with minimum arrival time.
Next, we run a while loop till simulation time. In this loop the following things are done.
Another while loop is run. In this, we check the processes whose arrival time is less than current time 
and push them in a sequential order into the ready queue, and pop out from the array.
We check if the running is not empty and the last process with non zero left burst time is kept back in
the ready queue.
Next we pass running queue as pass by reference into rr function present in the scheduler class. This 
rr function, has access to the ready queue. So, it pushes the first process of ready queue into running 
queue and removes it from ready queue. The Response time is set.
Next, we update the left burst time, current time of the last process in running queue.
This loop runs till the processes array and ready queue become empty and all processes go into the running
queue. 
   
- runCFS Algorithm :
Minheap is performed on the processes array. The process with minimum arrival time is kept at the top
of the min-heap.
Sometimes the processes may not arrive at 0 milliseconds, so the currentTime is equated to the process
with minimum arrival time.
Next, we run a while loop till simulation time. In this loop the following things are done.
Another while loop is run. In this, we check the processes whose arrival time is less than current time 
and push them in a sequential order into Red Black Tree and pop out from the array.
Here Red Black Tree is implemented using map STL. In this Red Black Tree the processes are placed as per 
their virtual run times (if they are equal, then nice values, followed by arrival time, and then by 
comparision between process Ids)
We check if the running is not empty and the put last process with non zero left burst time back in the 
ready queue.
The process with minimum virtual run time is kept at the left most position of the red black tree. We use 
it->first to point to this process with minimum virtual runtime. This process and the running queue are 
passed into the cfs function of schedular class.
This cfs function in scheduler class pushes the process into running queue and sets its response time.
Next we fix the virtual run time of the process in running queue. Note that the formula used in the code 
is empirical and derived taking the real formuals into consideration.
After setting the virtual run time, we update the left burst time and current time.
This loop runs till the processes array and ready queue become empty and all processes go into the running
queue.

- Status.txt is obtained when the run codes of scheduling algorithms start running.

- Next we have callAlgo function which creates Process_Creator object obj, Scheduler object sch, Simulator 
object sim and runs the algorithm as per user's choice and sends the output to processes.txt.

- Main function takes the Number of processes and simulation time in seconds.
   
## Resources : 

- Round Robin Scheduler learning resource [Youtube video](https://www.youtube.com/watch?v=TxjIlNYRZ5M&t=421s)

- Completely Fair Scheduler learning resource [Slides](https://www.cse.iitd.ac.in/~rijurekha/col788/scheduling1.pdf) and [ Youtube video](https://www.youtube.com/watch?v=MkJfuI5_hjc)
