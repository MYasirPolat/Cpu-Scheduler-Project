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
  
    //Closes the files
    fclose(inputFile);
    fclose(outputFile);
    return 0;
}