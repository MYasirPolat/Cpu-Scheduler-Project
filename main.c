#include <stdbool.h>
#include <stdio.h>

#define MAX_LINE_LENGTH 100
#define MAX_PROCESSES 100
#define CPU1_RAM 512
#define CPU2_RAM 1536

//Structs for processes that will be read
struct Process {
    int processNumber;
    int arrivalTime;
    int priority;
    int burstTime;
    int ram;
    int cpuUsage;
    bool isTerminated;
    bool isQueued;
};


//Function to read and assigns procceses to structs
void readFile(FILE* file,struct Process processes[],int* processCount,int* count0, int* count1, int* count2, int* count3) {

    char line[MAX_LINE_LENGTH]; //Creating an array to read lines

    //Reads the input file and create process structures for each process
    while (fgets(line, sizeof(line), file)) {
        struct Process newProcess;
        sscanf(line, "P%d,%d,%d,%d,%d,%d",
               &newProcess.processNumber,
               &newProcess.arrivalTime,
               &newProcess.priority,
               &newProcess.burstTime,
               &newProcess.ram,
               &newProcess.cpuUsage);
        newProcess.isTerminated =false;
        newProcess.isQueued= false;

        processes[(*processCount)++] = newProcess; // Counts total number of processes
        //Counts how many processes are there for each priority
        switch (newProcess.priority) {
            case 0: (*count0)++; break;
            case 1: (*count1)++; break;
            case 2: (*count2)++; break;
            case 3: (*count3)++; break;
            default: break;
        }
    }
}

//Function to check if there is enough resources
bool isEnoughResource(int cpuNumber, int currentRam, int currentCpu) {

    int maxRam;
    //Deciding the maximum ram depending on the cpu we are operating on
    if(cpuNumber ==1) {
        maxRam= CPU1_RAM;
    }else if(cpuNumber == 2) {
        maxRam = CPU2_RAM;
    }else{perror("There is no such cpu");}

    if (currentRam<=maxRam && currentCpu<= 100) {return true;}  //Returns true if limits are not exceeded
    return false;   //Returns false if limits are exceeded
}

//Function to assign procesess to cpus
void assignCpus(FILE* file, struct Process processes[], int processCount) {
    for (int i = 0; i< processCount;i++) {
        switch (processes[i].priority) {
            case 0 : fprintf(file,"Process %d is queued to be assigned to CPU-1.\n",processes[i].processNumber); break;
            case 1 : fprintf(file,"Process %d is queued to be assigned to CPU-2.\n",processes[i].processNumber); break;
            case 2 : fprintf(file,"Process %d is queued to be assigned to CPU-2.\n",processes[i].processNumber); break;
            case 3 : fprintf(file,"Process %d is queued to be assigned to CPU-2.\n",processes[i].processNumber); break;
            default: perror("No such priority is defined");
        }
    }
}

//Functions to process the processes with priority 0 at cpu1
void FCFS(FILE* file,struct Process processes[],int processCount,int priority0Count) {
    struct Process priority0[priority0Count];   //Struct to hold every process with priority 0
    //Add all priority 0 processes to cpu1 queue
    for(int i =0, j=0 ;i<processCount;i++) {
        if(processes[i].priority==0 && isEnoughResource(1,processes[i].ram,processes[i].cpuUsage)) {
            priority0[j] = processes[i];
            j++;
        }
    }
    if(priority0 != NULL){  //Doesnt run if there is no process with priority 0
        //Sort the process according to arrival time with bubble sort
        for(int i=0;i<priority0Count-1;i++) {
            for (int j =0; j< priority0Count-i-1;j++) {
                if(priority0[j].arrivalTime>priority0[j+1].arrivalTime) {
                    struct Process temp = priority0[j];
                    priority0[j] = priority0[j+1];
                    priority0[j+1] = temp;
                }
            }
        }
        printf("CPU-1 que1(priority-0) (FCFS) ->");
        //Assign processes to the cpu1 within resource limits
        for(int i=0;i<priority0Count;i++) {
            if(isEnoughResource(1,priority0[i].ram, priority0[i].cpuUsage)) {
                printf("P%d,", priority0[i].processNumber);
                fprintf(file,"Process %d is assigned to CPU-1.\n" , priority0[i].processNumber);
                fprintf(file,"Process %d is completed and terminated.\n" , priority0[i].processNumber);
            }
        }
    }
}

//Functions to process the processes with priority 1 at cpu2
void SJF(FILE* file, struct Process processes[], int processCount, int priority1Count) {
    int elapsedTime = 0;        //Integer for storing the time that has passed
    int processedCount = 0;     //Integer for storing the processes that are terminated

    printf("\nCPU-2 que2(priority-1) (SJF) ->");
    //Runs until all the processes with priority 0 are executed and terminated
    while (processedCount < priority1Count) {
        int fastestProcessIndex = -1;   //Holds the index of fastest process
        // Find the process with the shortest burst time that has arrived and is not processed yet
        for (int i = 0; i < processCount; i++) {
            if (processes[i].priority == 1 && processes[i].arrivalTime <= elapsedTime && !processes[i].isTerminated) {
                if (fastestProcessIndex == -1 || processes[i].burstTime < processes[fastestProcessIndex].burstTime) {
                    fastestProcessIndex = i;
                }
            }
        }

        if (fastestProcessIndex != -1) {    //Checks if there is a fastest process
            //Assign processes to the cpu1 within resource limits
            struct Process* fastestProcess = &processes[fastestProcessIndex];
            if(isEnoughResource(2, processes[fastestProcessIndex].ram,processes[fastestProcessIndex].cpuUsage)) {
                printf("P%d,",fastestProcess->processNumber);
                fprintf(file, "Process %d is assigned to CPU-2.\n", fastestProcess->processNumber);
                fprintf(file, "Process %d is completed and terminated.\n", fastestProcess->processNumber);
                elapsedTime += fastestProcess->burstTime;
            }else{fprintf(file,"Not enough resources for Process %d\n",processes[fastestProcessIndex].processNumber);}

            fastestProcess->isTerminated = true;
            processedCount++;
        } else {
            // No process arrived yet, increment the time
            elapsedTime++;
        }
    }
}


int main(void)
{
    struct Process processes[MAX_PROCESSES];    //Create a struct for all processes

    //Variables to store how many processes are there for each priority
    int processCount= 0;
    int count0= 0;
    int count1= 0;
    int count2= 0;
    int count3= 0;

    //Opening the files for input and output files
    FILE* inputFile=fopen("input.txt","r");
    FILE* outputFile=fopen("output.txt","w");
    if (inputFile == NULL || outputFile==NULL) {
        perror("Error opening file");
        return -1;
    }

    readFile(inputFile,processes, &processCount, &count0, &count1, &count2, &count3);   //Reads the input file
    assignCpus(outputFile,processes,processCount);                                      //Assigns processes to cpus
    FCFS(outputFile,processes,processCount,count0);                         //Runs the FCFS algortihm for priority 0 processes in cpu1
    SJF(outputFile,processes,processCount,count1);                          //Runs the SJF algortihm for priority 1 processes in cpu2
  
    //Closes the files
    fclose(inputFile);
    fclose(outputFile);
    return 0;
}