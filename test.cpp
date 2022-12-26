#include <iostream>
#include <vector>
#include <time.h>
int N;
int simulationTime;

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
    int responseTime;
    int startTime = 0;
    int endTime;
    int leftBurstTime;

public:
    Process()
    {
    }

    ~Process()
    {
    }

    Process(int pid, int aTime, int bTime)
    {
        processId = pid;
        arrivalTime = aTime;
        burstTime = bTime;
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
        // cout << " Start Time : " << startTime << " ms" << endl;
        // cout << " End Time : " << endTime << " ms" << endl;
        cout << " Burst Time : " << burstTime << " ms" << endl;
        cout << " Completion Time : " << completionTime << " ms" << endl;
        cout << " Turn Around Time : " << turnAroundTime << " ms" << endl;
        cout << " Waiting Time : " << waitingTime << " ms" << endl;
        cout << " Response Time : " << responseTime << " ms" << endl;
    }

    friend class Process_Creator;
    friend class Scheduler;
    friend class MinHeap;
    friend class Simulator;
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
            int u = 0;

            while (u == 0)
            {
                u = rand() % 10;
            }

            int bTime = u;

            array->at(i) = Process(i, aTime, bTime);
        }
    }

    // friend class Scheduler;
    friend class Simulator;
};

class MinHeap
{
private:
    vector<Process> *arr;
    int leftChild(int parent) { return 2 * parent + 1; }
    int rightChild(int parent) { return 2 * parent + 2; }
    int parent(int child) { return (child - 1) / 2; }

    void heapifyUp(int child)
    {
        if (child == 0)
            return;

        int p = parent(child);
        if (arr->at(p).arrivalTime > arr->at(child).arrivalTime)
        {
            swap(arr->at(p), arr->at(child));
            heapifyUp(p);
        }
    }

    void heapifyDown(int parent)
    {
        int left = leftChild(parent);
        int right = rightChild(parent);
        int smallest = parent;

        if (left < arr->size() && arr->at(left).arrivalTime < arr->at(smallest).arrivalTime)
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
            heapifyDown(smallest);
        }
    }

public:
    MinHeap(vector<Process> *readyQueue)
    {
        arr = readyQueue;
        buildMinHeap();
    }

    void buildMinHeap()
    {
        for (int start = (arr->size() / 2) - 1; start >= 0; --start)
        {
            heapifyDown(start);
        }
    }

    Process getMin() { return arr->at(0); }

    void pop()
    {
        arr->at(0) = arr->back();
        arr->pop_back();
        heapifyDown(0);
    }

    void insert(Process key)
    {
        arr->push_back(key);
        heapifyUp(arr->size() - 1);
    }

    // void heapSort(vector<Process> *readyQueue, vector<Process> *arr)
    // {
    //     int i = arr->size();
    //     while (i > 0)
    //     {
    //         readyQueue->push_back(getMin(arr));
    //         pop(arr);
    //         i = arr->size();
    //     }
    // }
};

class RedBlackTree
{
public:
    friend class Scheduler;
};

class Scheduler
{
private:
    // vector<Process> *readyQueue = new vector<Process>();
    // vector<Process> *array;

public:
    Scheduler()
    {
        // array = obj.array;

        // for(int i= 0; i<N; i++)
        // {
        //     readyQueue.push_back(obj.array->at(i));
        // }
    }

    ~Scheduler()
    {
    }

    // vector<Process> &returnReadyQueue()
    // {
    //     return readyQueue;
    // }

    static void fcfs(vector<Process> *readyQueue, vector<Process> *runningQueue);
    void rr(int tQ);
    void cfs();

    friend class RedBlackTree;
    friend class MinHeap;
    friend class Simulator;
};

void Scheduler::fcfs(vector<Process> *readyQueue, vector<Process> *runningQueue)
{
    MinHeap min(readyQueue);

    runningQueue->push_back(min.getMin());

    min.pop();

    // for (int i = 0; i < readyQueue->size(); i++)
    // {
    //     if (i == 0)
    //     {
    //         readyQueue->at(i).startTime = readyQueue->at(i).arrivalTime;
    //         readyQueue->at(i).endTime = readyQueue->at(i).burstTime;
    //     }
    //     else
    //     {
    //         readyQueue->at(i).startTime = readyQueue->at(i - 1).endTime;
    //     }
    // }
}

void Scheduler::rr(int tQ)
{
    cout << "\n Hey! Round Robin Here\n"
         << endl;
}

void Scheduler::cfs()
{
    cout << " cfs function has been called" << endl;
}

class Simulator
{
private:
    vector<Process> *readyQueue;
    vector<Process> *runningQueue = new vector<Process>();
    int currentTime, processNum;
    int timeQuantum;
    int a;

    void ExecuteProcesses()
    {
        if (a == 1)
        {
            runFCFS();
        }
        else if (a == 2)
        {
            runRR();
        }
        else
        {
            runCFS();
        }
    }

    void CaptureValues()
    {
        for (int i = 0; i < runningQueue->size(); i++)
        {
            runningQueue->at(i).setTurnAroundTime();
            runningQueue->at(i).setWaitingTime();
        }
    }

public:
    Simulator(Process_Creator &obj, int y)
    {
        readyQueue = obj.array;
        a = y;
    }

    void Run()
    {
        currentTime = 0, processNum = 0;

        if(a == 2)
        {
            cout << " Enter the Time Quantum : ";
            cin >> timeQuantum;
        }

        while (currentTime < simulationTime)
        {
            ExecuteProcesses();

            if (readyQueue->empty())
            {
                break;
            }
        }

        CaptureValues();
        printProcesses();
    }

    void printProcesses()
    {
        for (int i = 0; i < runningQueue->size(); i++)
        {
            runningQueue->at(i).print();
        }
    }

    void runFCFS();
    void runRR();
    void runCFS();
};

void Simulator::runFCFS()
{
    Scheduler::fcfs(readyQueue, runningQueue);

    if (processNum == 0)
    {
        currentTime = runningQueue->at(processNum).arrivalTime;
    }

    runningQueue->at(processNum).startTime = currentTime;
    int startTime = runningQueue->at(processNum).startTime;
    runningQueue->at(processNum).setResponseTime(startTime);

    runningQueue->at(processNum).endTime = currentTime + runningQueue->at(processNum).burstTime;
    int endTime = runningQueue->at(processNum).endTime;
    runningQueue->at(processNum).setCompletionTime(endTime);

    currentTime = runningQueue->at(processNum).endTime;

    processNum++;
}

void Simulator::runRR()
{
}

void Simulator::runCFS()
{
}

void callAlgo()
{
    int a;
    // Scheduler sch(obj);

    cout << " Choose the Scheduling Algorithm (1/2/3): ";
    cin >> a;

    if (!(a == 1 || a == 2 || a == 3))
    {
        cout << " Error! invalid input\n" << endl;
        callAlgo();
        return;
    }

    Process_Creator obj;
    Simulator sim(obj, a);
    sim.Run();
    // sim.printProcesses(obj);

    return;
}

int main()
{
    cout << "\n In this program, the arrival time and burst time " << endl;
    cout << " will be set in the range of (1-10) milli seconds " << endl;
    cout << "\n Enter the Number of Processes : ";
    cin >> N;

    cout << "\n Enter the Simulation Time (secs) : ";
    cin >> simulationTime;

    // Converting from seconds to milliseconds
    simulationTime *= 1000;

    cout << "\n1) First Come First Serve" << endl;
    cout << "2) Round Robin" << endl;
    cout << "3) Completely Fair Scheduler\n"
         << endl;

    callAlgo();

    return 0;
}