#include <fstream>
#include <iostream>
#include <vector>
#include <time.h>
#include <random>
#define Infinity 999999
int N;
int simulationTime, currentTime;

using namespace std;

class Process
{
private:
    int processId;
    int arrivalTime;
    int burstTime;
    int completionTime;
    int turnAroundTime;
    int waitingTime;
    int responseTime = Infinity;
    int leftBurstTime;
    int virtualRunTime;
    int priority;

public:
    Process()
    {
    }

    ~Process()
    {
    }

    Process(int pid, int aTime, int bTime, int nice)
    {
        processId = pid;
        arrivalTime = aTime;
        burstTime = bTime;
        leftBurstTime = bTime;
        priority = 20 - nice;
        virtualRunTime = priority;
    }

    void setCompletionTime(int cTime)
    {
        completionTime = cTime;
    }

    void setTurnAroundTime()
    {
        turnAroundTime = completionTime - arrivalTime;
    }

    void setWaitingTime()
    {
        waitingTime = turnAroundTime - burstTime;
    }

    void setResponseTime(int resTime)
    {
        responseTime = resTime - arrivalTime;
    }

    void print()
    {
        cout << "\n Process Id : " << processId << endl;
        cout << " Arrival Time : " << arrivalTime << " ms" << endl;
        cout << " Burst Time : " << burstTime << " ms" << endl;
        cout << " Completion Time : " << completionTime << " ms" << endl;
        cout << " Turn Around Time : " << turnAroundTime << " ms" << endl;
        cout << " Waiting Time : " << waitingTime << " ms" << endl;
        cout << " Response Time : " << responseTime << " ms" << endl;
        cout << " Priority : " << priority << endl;
    }

    friend class Process_Creator;
    friend class Scheduler;
    friend class MinHeap;
    friend class Simulator;
    void friend callAlgo();
};

class Process_Creator
{
private:
    vector<Process> *array = new vector<Process>();

public:
    Process_Creator()
    {
        array->resize(N);
        srand(time(0));

        for (int i = 0; i < N; i++)
        {
            int aTime = rand() % 10;
            int bTime;

            while (1)
            {
                bTime = rand() % 10;

                if (bTime != 0)
                {
                    break;
                }
            }

            int nice;

            random_device rd;
            mt19937 gen(rd());
            uniform_int_distribution<int> distNice(-20, 19);

            nice = distNice(gen);

            array->at(i) = Process(i, aTime, bTime, nice);
        }
    }

    friend class Scheduler;
    friend class Simulator;
};

class MinHeap
{
private:
    vector<Process> *arr;
    int leftChild(int parent) { return 2 * parent + 1; }
    int rightChild(int parent) { return 2 * parent + 2; }
    int parent(int child) { return (child - 1) / 2; }

    void minHeapify(int parent)
    {
        int left = leftChild(parent);
        int right = rightChild(parent);
        int smallest = parent;

        if (left < arr->size() && arr->at(left).arrivalTime <= arr->at(smallest).arrivalTime)
        {
            smallest = left;
        }
        else
        {
            smallest = parent;
        }

        if (right < arr->size() && arr->at(right).arrivalTime < arr->at(smallest).arrivalTime)
        {
            smallest = right;
        }

        if (smallest != parent)
        {
            swap(arr->at(smallest), arr->at(parent));
            minHeapify(smallest);
        }
    }

public:
    MinHeap(vector<Process> *array)
    {
        arr = array;
        buildMinHeap();
    }

    void buildMinHeap()
    {
        for (int start = (arr->size() / 2) - 1; start >= 0; --start)
        {
            minHeapify(start);
        }
    }

    Process getMin() { return arr->at(0); }

    void pop()
    {
        arr->at(0) = arr->back();
        arr->pop_back();
        minHeapify(0);
    }
};

class RedBlackTree
{

};

class Scheduler
{
private:
    vector<Process> *readyQueue = new vector<Process>();
    vector<Process> *array;

public:
    Scheduler()
    {
    }

    ~Scheduler()
    {
    }

    Scheduler(Process_Creator &obj)
    {
        array = obj.array;
    }

    void fcfs(vector<Process> *runningQueue);
    void rr(vector<Process> *runningQueue);
    void cfs(vector<Process> *runningQueue);

    friend class RedBlackTree;
    friend class MinHeap;
    friend class Simulator;
};

void Scheduler::fcfs(vector<Process> *runningQueue)
{
    MinHeap min(readyQueue);

    runningQueue->push_back(min.getMin());

    min.pop();
}

void Scheduler::rr(vector<Process> *runningQueue)
{
    if (!readyQueue->empty())
    {
        runningQueue->push_back(readyQueue->at(0));
        readyQueue->erase(readyQueue->begin());

        if (runningQueue->back().responseTime == Infinity)
        {
            runningQueue->back().setResponseTime(currentTime);
        }
    }
}

void Scheduler::cfs(vector<Process> *runningQueue)
{
    cout << "\n Hey CFS here" << endl;
}

class Simulator
{
private:
    vector<Process> *array;
    vector<Process> *runningQueue = new vector<Process>();
    vector<Process> *readyQueue;
    Scheduler *sch;
    int processNum;
    int timeQuantum, timeSlice;
    int choice;

    void CaptureValues()
    {
        for (int i = 0; i < runningQueue->size(); i++)
        {
            runningQueue->at(i).setTurnAroundTime();
            runningQueue->at(i).setWaitingTime();
        }
    }

public:
    Simulator(Scheduler &schObj, int a)
    {
        readyQueue = schObj.readyQueue;
        array = schObj.array;
        sch = &schObj;
        choice = a;
    }

    void printProcesses()
    {
        for (int i = 0; i < runningQueue->size(); i++)
        {
            runningQueue->at(i).print();
        }
    }

    void Run()
    {
        if (choice == 1)
        {
            runFCFS();
        }
        else if (choice == 2)
        {
            runRR();
        }
        else
        {
            runCFS();
        }

        CaptureValues();
        printProcesses();
    }

    void runFCFS();
    void runRR();
    void runCFS();

    void friend callAlgo();
};

void Simulator::runFCFS()
{
    cout << "\n Hey! FCFS here\n"
         << endl;

    MinHeap heap(array);

    while (!array->empty())
    {
        readyQueue->push_back(array->at(0));
        heap.pop();
    }

    while (currentTime < simulationTime)
    {
        sch->fcfs(runningQueue);

        if (processNum == 0)
        {
            currentTime = runningQueue->at(processNum).arrivalTime;
        }

        runningQueue->at(processNum).setResponseTime(currentTime);

        int endTime = currentTime + runningQueue->at(processNum).burstTime;
        runningQueue->at(processNum).setCompletionTime(endTime);

        currentTime = endTime;
        processNum++;

        if (readyQueue->empty())
        {
            break;
        }
    }
}

void Simulator::runRR()
{
    cout << "\n Hey! Round Robin Here\n"
         << endl;
    cout << " Enter the Time Quantum : ";
    cin >> timeQuantum;

    MinHeap heap(array);

    if (array->at(0).arrivalTime != 0)
    {
        currentTime = array->at(0).arrivalTime;
    }

    while (currentTime < simulationTime)
    {
        while (!array->empty())
        {
            if (array->at(0).arrivalTime <= currentTime)
            {
                readyQueue->push_back(array->at(0));
                heap.pop();
            }
            else
            {
                break;
            }
        }

        if (!runningQueue->empty() && runningQueue->back().leftBurstTime != 0)
        {
            readyQueue->push_back(runningQueue->back());
            runningQueue->erase(runningQueue->end());
        }

        sch->rr(runningQueue);

        if (!runningQueue->empty())
        {
            if (runningQueue->back().leftBurstTime >= timeQuantum)
            {
                currentTime += timeQuantum;
                runningQueue->back().leftBurstTime -= timeQuantum;
            }
            else if (runningQueue->back().leftBurstTime != 0 && runningQueue->back().leftBurstTime < timeQuantum)
            {
                currentTime += runningQueue->back().leftBurstTime;
                runningQueue->back().leftBurstTime = 0;
            }
            else
            {
                ;
            }

            runningQueue->back().setCompletionTime(currentTime);
        }

        if (readyQueue->empty() && array->empty())
        {
            break;
        }
    }
}

void Simulator::runCFS()
{
    cout << "\n Hey! Completely Fair Scheduler Here\n"
         << endl;
    cout << " Enter the Time Slice : ";
    cin >> timeSlice;

    MinHeap heap(array);
    processNum = 1;

    if (array->at(0).arrivalTime != 0)
    {
        currentTime = array->at(0).arrivalTime;
    }

    while (currentTime < simulationTime)
    {
        while (!array->empty())
        {
            if (array->at(0).arrivalTime <= currentTime)
            {
                readyQueue->push_back(array->at(0));
                heap.pop();
            }
            else
            {
                break;
            }
        }

        // if (!runningQueue->empty())
        // {
        //     if (runningQueue->back().leftBurstTime != 0)
        //     {
        //         readyQueue->push_back(runningQueue->back());
        //         runningQueue->erase(runningQueue->end());
        //     }
        // }

        sch->cfs(runningQueue);

        // if (!runningQueue->empty())
        // {
        //     if (runningQueue->back().leftBurstTime >= timeQuantum)
        //     {
        //         currentTime += timeQuantum;
        //         runningQueue->back().leftBurstTime -= timeQuantum;
        //     }
        //     else if (runningQueue->back().leftBurstTime != 0 && runningQueue->back().leftBurstTime < timeQuantum)
        //     {
        //         currentTime += runningQueue->back().leftBurstTime;
        //         runningQueue->back().leftBurstTime = 0;
        //     }
        //     else
        //     {
        //         ;
        //     }
        //     runningQueue->back().setCompletionTime(currentTime);
        // }

        if (readyQueue->empty() && array->empty())
        {
            break;
        }
    }
}

void callAlgo()
{
    int a;
    vector<Process> *f;

    cout << "\n1) First Come First Serve" << endl;
    cout << "2) Round Robin" << endl;
    cout << "3) Completely Fair Scheduler\n"
         << endl;

    cout << " Choose the Scheduling Algorithm (1/2/3): ";
    cin >> a;

    if (!(a == 1 || a == 2 || a == 3))
    {
        cout << "\n Error! invalid input" << endl;
        callAlgo();
        return;
    }

    Process_Creator obj;
    Scheduler sch(obj);
    Simulator sim(sch, a);
    sim.Run();

    f = sim.runningQueue;

    std::ofstream output_file("processes.txt");

    if (a == 1)
    {
        output_file << "\n\tFirst Come First Serve : \n\n";
    }
    else if (a == 2)
    {
        output_file << "\n\tRound Robin: \n\n";
    }
    else
    {
        output_file << "\n\tCompletely Fair Scheduler : \n\n";
    }

    output_file << "\tID\tAT\tBT\tCT\tTAT\tWT\tRT\n";

    for (int i = 0; i < f->size(); i++)
    {
        output_file << "\t" << f->at(i).processId << "\t"
                    << f->at(i).arrivalTime << "\t" << f->at(i).burstTime << "\t" << f->at(i).completionTime
                    << "\t" << f->at(i).turnAroundTime << "\t" << f->at(i).waitingTime << "\t" << f->at(i).responseTime << "\n";
    }

    output_file.close();

    return;
}

int main()
{
    cout << "\n In, the arrival time and burst time " << endl;
    cout << " will be set in the range of (1-10) milli seconds " << endl;
    cout << "\n Enter the Number of Processes : ";
    cin >> N;

    cout << "\n Enter the Simulation Time (secs) : ";
    cin >> simulationTime;

    // Converting from seconds to milliseconds
    simulationTime *= 1000;

    callAlgo();

    return 0;
}
