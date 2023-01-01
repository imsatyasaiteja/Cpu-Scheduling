#include <fstream>
#include <iostream>
#include <vector>
#include <time.h>
#include <random>
#include <string>
#include <map>
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
    float virtualRunTime;
    float timeSlice;
    int staticPriority;

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
        staticPriority = 120 + nice;
        virtualRunTime = nice;
        timeSlice = 0;
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
        cout << " Nice : " << staticPriority - 120 << endl;
        cout << " Arrival Time : " << arrivalTime << " ms" << endl;
        cout << " Burst Time : " << burstTime << " ms" << endl;
        cout << " Completion Time : " << completionTime << " ms" << endl;
        cout << " Turn Around Time : " << turnAroundTime << " ms" << endl;
        cout << " Waiting Time : " << waitingTime << " ms" << endl;
        cout << " Response Time : " << responseTime << " ms" << endl;
    }

    friend class Process_Creator;
    friend class Scheduler;
    friend class MinHeap;
    friend bool compare(const Process &p1, const Process &p2);
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

bool compare(const Process &p1, const Process &p2)
{
    return p1.virtualRunTime < p2.virtualRunTime;
}

class Scheduler
{
private:
    vector<Process> *readyQueue = new vector<Process>();
    vector<Process> *array;

public:
    Scheduler(Process_Creator &obj)
    {
        array = obj.array;
    }

    void fcfs(vector<Process> *runningQueue);
    void rr(vector<Process> *runningQueue);
    void cfs(Process p, vector<Process> *runningQueue);

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

void Scheduler::cfs(Process p, vector<Process> *runningQueue)
{

    runningQueue->push_back(p);

    if (runningQueue->back().responseTime == Infinity)
    {
        runningQueue->back().setResponseTime(currentTime);
    }
}

class Simulator
{
private:
    vector<Process> *array;
    vector<Process> *runningQueue = new vector<Process>();
    vector<Process> *readyQueue;
    Scheduler *sch;
    int timeQuantum, choice;

    void CaptureValues()
    {
        for (int i = 0; i < array->size(); i++)
        {
            array->at(i).setTurnAroundTime();
            array->at(i).setWaitingTime();
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
        for (int i = 0; i < array->size(); i++)
        {
            array->at(i).print();
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

        for(int i=0; i<runningQueue->size(); i++)
        {
            array->push_back(runningQueue->at(i));
        }

        delete readyQueue;
        delete runningQueue;

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

    if (heap.getMin().arrivalTime != 0)
    {
        currentTime = heap.getMin().arrivalTime;
    }

    ofstream status_file("status.txt");
    status_file << "\n\tFirst Come First Serve: \n";
    status_file << "\n\tTime Stamp\tProcess Id\tStatus\n\n";

    while (currentTime < simulationTime)
    {
        while (!array->empty())
        {
            if (heap.getMin().arrivalTime <= currentTime)
            {
                readyQueue->push_back(heap.getMin());
                status_file << "\t" << readyQueue->back().arrivalTime << " ms"
                            << "\t\t" << readyQueue->back().processId << "\t\t\tArrived"
                            << "\n";
                heap.pop();
            }
            else
            {
                break;
            }
        }

        if (!runningQueue->empty())
        {
            status_file << "\t" << currentTime << " ms"
                        << "\t\t" << runningQueue->back().processId << "\t\t\tExit"
                        << "\n";
        }

        if (array->empty() && readyQueue->empty())
        {
            break;
        }

        sch->fcfs(runningQueue);

        runningQueue->back().setResponseTime(currentTime);

        status_file << "\t" << currentTime << " ms"
                    << "\t\t" << runningQueue->back().processId << "\t\t\tRunning"
                    << "\n";

        int endTime = currentTime + runningQueue->back().burstTime;
        runningQueue->back().setCompletionTime(endTime);

        currentTime = endTime;
    }

    status_file.close();
}

void Simulator::runRR()
{
    cout << "\n Hey! Round Robin Here\n"
         << endl;
    cout << " Enter the Time Quantum : ";
    cin >> timeQuantum;

    MinHeap heap(array);

    if (heap.getMin().arrivalTime != 0)
    {
        currentTime = heap.getMin().arrivalTime;
    }

    ofstream status_file("status.txt");
    status_file << "\n\tRound Robin: \n";
    status_file << "\n\tTime Stamp\tProcess Id\tStatus\n\n";

    while (currentTime < simulationTime)
    {
        while (!array->empty())
        {
            if (heap.getMin().arrivalTime <= currentTime)
            {
                readyQueue->push_back(heap.getMin());
                status_file << "\t" << readyQueue->back().arrivalTime << " ms"
                            << "\t\t" << readyQueue->back().processId << "\t\t\tArrived"
                            << "\n";
                heap.pop();
            }
            else
            {
                break;
            }
        }

        if (!runningQueue->empty())
        {
            if (runningQueue->back().leftBurstTime != 0)
            {
                readyQueue->push_back(runningQueue->back());

                status_file << "\t" << currentTime << " ms"
                            << "\t\t" << runningQueue->back().processId << "\t\t\tArrived"
                            << "\n";

                runningQueue->erase(runningQueue->end());
            }
            else
            {
                status_file << "\t" << currentTime << " ms"
                            << "\t\t" << runningQueue->back().processId << "\t\t\tExit"
                            << "\n";
            }
        }

        if (readyQueue->empty() && array->empty())
        {
            break;
        }

        sch->rr(runningQueue);

        status_file << "\t" << currentTime << " ms"
                    << "\t\t" << runningQueue->back().processId << "\t\t\tRunning"
                    << "\n";

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
                currentTime += 0;
            }

            runningQueue->back().setCompletionTime(currentTime);
        }
    }

    status_file.close();
}

void Simulator::runCFS()
{
    cout << "\n Hey! Completely Fair Scheduler Here\n"
         << endl;

    MinHeap heap(array);
    map<Process, bool, decltype(&compare)> redBlackTree(&compare);
    auto it = redBlackTree.begin();

    ofstream status_file("status.txt");
    status_file << "\n\tCompletely Fair Scheduler: \n";
    status_file << "\n\tTime Stamp\tProcess Id\tStatus\n\n";

    if (heap.getMin().arrivalTime != 0)
    {
        currentTime = heap.getMin().arrivalTime;
    }

    while (currentTime < simulationTime)
    {
        while (!array->empty())
        {
            if (array->at(0).arrivalTime <= currentTime)
            {
                redBlackTree[array->at(0)] = true;
                status_file << "\t" << heap.getMin().arrivalTime << " ms"
                            << "\t\t" << heap.getMin().processId << "\t\t\tArrived"
                            << "\n";
                heap.pop();
            }
            else
            {
                break;
            }
        }

        if (!runningQueue->empty())
        {
            if (runningQueue->back().leftBurstTime != 0)
            {
                redBlackTree[runningQueue->back()] = true;

                status_file << "\t" << currentTime << " ms"
                            << "\t\t" << runningQueue->back().processId << "\t\t\tArrived"
                            << "\n";

                runningQueue->erase(runningQueue->end());
            }
            else
            {
                status_file << "\t" << currentTime << " ms"
                            << "\t\t" << runningQueue->back().processId << "\t\t\tExit"
                            << "\n";
            }
        }

        if (redBlackTree.empty() && array->empty())
        {
            break;
        }

        it = redBlackTree.begin();
        Process p = it->first;
        redBlackTree.erase(it);

        sch->cfs(p, runningQueue);
        redBlackTree.erase(p);

        status_file << "\t" << currentTime << " ms"
                    << "\t\t" << runningQueue->back().processId << "\t\t\tRunning"
                    << "\n";

        if(runningQueue->back().staticPriority < 120)
        {
            runningQueue->back().timeSlice += 4;
        }
        else
        {
            runningQueue->back().timeSlice += 1;
        }

        runningQueue->back().virtualRunTime += (runningQueue->back().timeSlice * (runningQueue->back().staticPriority - 120));
        
        if (!runningQueue->empty() && runningQueue->back().leftBurstTime >= runningQueue->back().timeSlice)
        {
            currentTime += runningQueue->back().timeSlice;
            runningQueue->back().leftBurstTime -= runningQueue->back().timeSlice;
        }
        else if (runningQueue->back().leftBurstTime != 0 && runningQueue->back().leftBurstTime < runningQueue->back().timeSlice)
        {
            currentTime += runningQueue->back().leftBurstTime;
            runningQueue->back().leftBurstTime = 0;
        }
        else
        {
            currentTime += 0;
        }

        runningQueue->back().setCompletionTime(currentTime);
    
    }

    status_file.close();
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

    f = sim.array;

    ofstream output_file("processes.txt");

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
        output_file << "\tID\tAT\tBT\tCT\tTAT\tWT\tRT\t\tNICE\tVRT\n";

        for (int i = 0; i < f->size(); i++)
        {
            output_file << "\t" << f->at(i).processId << "\t"
                        << f->at(i).arrivalTime << "\t" << f->at(i).burstTime << "\t" << f->at(i).completionTime
                        << "\t" << f->at(i).turnAroundTime << "\t" << f->at(i).waitingTime << "\t" << f->at(i).responseTime 
                        << "\t\t" << f->at(i).staticPriority - 120 << "\t\t" << f->at(i).virtualRunTime << "\n";
        }
    }

    if (a == 1 || a == 2)
    {
        output_file << "\tID\tAT\tBT\tCT\tTAT\tWT\tRT\n";

        for (int i = 0; i < f->size(); i++)
        {
            output_file << "\t" << f->at(i).processId << "\t"
                        << f->at(i).arrivalTime << "\t" << f->at(i).burstTime << "\t" << f->at(i).completionTime
                        << "\t" << f->at(i).turnAroundTime << "\t" << f->at(i).waitingTime << "\t" << f->at(i).responseTime << "\n";
        }
    }

    output_file.close();

    delete f;
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
