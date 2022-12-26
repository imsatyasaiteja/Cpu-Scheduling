#include <iostream>
#include <vector>
#include <time.h>
int N;
int simulationTime;

class Simulator;

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
    int priority = 0;

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

    void setResponseTime(int startTime)
    {
        responseTime = startTime - arrivalTime;
    }

    void print()
    {
        // cout << "\n Process Id : " << processId << endl;
        cout << " Arrival Time : " << arrivalTime << " ms" << endl;
        // cout << " Burst Time : " << burstTime << " ms" << endl;
        cout << " Priority : " << priority << endl;
        // cout << " Completion Time : " << completionTime << " ms" <<  endl;
        // cout << " Turn Around Time : " << turnAroundTime << " ms" <<  endl;
        // cout << " Waiting Time : " << waitingTime << " ms" <<  endl;
        // cout << " Response Time : " << responseTime << " ms" <<  endl;
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

    friend class Scheduler;
    // friend class Simulator;
};

class MinHeap
{
private:
    static int leftChild(int parent) { return 2 * parent + 1; }
    static int rightChild(int parent) { return 2 * parent + 2; }
    static int parent(int child) { return (child - 1) / 2; }

    // void heapifyUp(int child)
    // {
    //     if (child == 0)
    //         return;

    //     int p = parent(child);
    //     if (arr[p].priority > arr[child].priority)
    //     {
    //         std::swap(arr[p], arr[child]);
    //         heapifyUp(p);
    //     }
    // }

    static void heapifyDown(vector<Process> *arr, int parent)
    {
        int left = leftChild(parent);
        int right = rightChild(parent);
        int smallest = parent;

        if (left < arr->size() && arr->at(left).priority < arr->at(smallest).priority)
        {
            smallest = left;
        }
        else
        {
            smallest = parent;
        }

        if (right < arr->size() && arr->at(right).priority < arr->at(smallest).priority)
        {
            smallest = right;
        }

        if (smallest != parent)
        {
            swap(arr->at(smallest), arr->at(parent));
            heapifyDown(arr, smallest);
        }
    }

public:
    MinHeap(vector<Process> *readyQueue, vector<Process> *arr)
    {
        // for (int j = 0; j < N; j++)
        // {
        //     arr->at(j).print();
        // }

        // cout << endl;

        buildMinHeap(arr);
    }

    void buildMinHeap(vector<Process> *arr)
    {
        for (int start = (arr->size() / 2) - 1; start >= 0; --start)
        {
            heapifyDown(arr, start);
        }
    }

    Process getMin(vector<Process> *arr) { return arr->at(0); }

    void pop(vector<Process> *arr)
    {
        arr->at(0) = arr->back();
        arr->pop_back();
        heapifyDown(arr, 0);
    }

    // void insert(Process key)
    // {
    //     arr.push_back(key);
    //     heapifyUp(arr.size() - 1);
    // }

    void heapSort(vector<Process> *readyQueue, vector<Process> *arr)
    {
        int i = arr->size();
        while (i > 0)
        {
            readyQueue->push_back(getMin(arr));
            pop(arr);
            i = arr->size();
        }
    }
};

class RedBlackTree
{
public:
    friend class Scheduler;
};

class Scheduler
{
private:
    int aTime, bTime;
    vector<Process> *readyQueue = new vector<Process>();
    vector<Process> *array;

public:
    Scheduler(Process_Creator &obj)
    {
        array = obj.array;

        // for(int i= 0; i<N; i++)
        // {
        //     readyQueue.push_back(obj.array->at(i));
        // }
    }

    // vector<Process> &returnReadyQueue()
    // {
    //     return readyQueue;
    // }

    void printProcesses()
    {
        for (int i = 0; i < N; i++)
        {
            readyQueue->at(i).print();
        }
    }

    void fcfs();
    void rr(int tQ);
    void cfs();

    friend class RedBlackTree;
    friend class MinHeap;
    friend class Simulator;
};

void Scheduler::fcfs()
{
    cout << "\n Hey! FCFS here\n" << endl;

    for (int val = 1; val <= N; val++)
    {
        int k;
        int min = 999999;
        for (int j = 0; j < N; j++)
        {
            if (array->at(j).priority == 0 && array->at(j).arrivalTime < min)
            {
                min = array->at(j).arrivalTime;
                k = j;
            }
        }

        array->at(k).priority = val;
    }

    MinHeap min(readyQueue, array);

    min.heapSort(readyQueue, array);

    // for (int i = 0; i < N; i++)
    // {
    //     array->at(i).print();
    // }

    // for (int j = 0; j < readyQueue->size(); j++)
    // {
    //     readyQueue->at(j).print();
    // }

    // Scheduler::printProcesses();
    delete array;
}

void Scheduler::rr(int tQ)
{
    cout << "\n Hey! Round Robin Here\n" << endl;

}

void Scheduler::cfs()
{
    cout << " cfs function has been called" << endl;
}

class Simulator
{
private:
    vector<Process> *rQ;
    void ExecuteProcesses()
    {

    }
    void CaptureValues()
    {

    }

public:
    Simulator(Scheduler &sch)
    {
        rQ = sch.readyQueue;

        for (int j = 0; j < rQ->size(); j++)
        {
            rQ->at(j).print();
        }
    }

    void Run()
    {
        for (int i = 0; i < simulationTime; i++)
        {
            ExecuteProcesses();
            CaptureValues();
        }
    }
};

void callAlgo()
{
    int s;
    Process_Creator obj;
    Scheduler sch(obj);

    cout << " Choose the Scheduling Algorithm (1/2/3): ";
    cin >> s;

    switch (s)
    {
    case (1):
        sch.fcfs();
        break;
    case (2):
        int timeQuantum;
        cout << " Enter the Time Quantum : ";
        cin >> timeQuantum;
        sch.rr(timeQuantum);
        break;
    case (3):
        sch.cfs();
        break;
    default:
        cout << " Error! invalid input\n"
             << endl;
        callAlgo();
        return;
    }

    Simulator sim(sch);

    // sim.printProcesses(obj);
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