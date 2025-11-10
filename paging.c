#include <stdio.h>
#include <stdlib.h>

#define MAX_PROCESSES 10
#define MAX_FRAMES 100

// Function to translate logical to physical address
void translateAddress(int pid, int logicalAddress, int pageSize, int processCount,
                      int processSize[], int *pageTable[], int pageTableSize[]) {
    int processIndex = pid - 1;

    if (processIndex < 0 || processIndex >= processCount) {
        printf("Invalid process ID!\n");
        return;
    }

    if (pageTableSize[processIndex] == 0) {
        printf("Process %d not allocated any pages!\n", pid);
        return;
    }

    if (logicalAddress < 0 || logicalAddress >= processSize[processIndex]) {
        printf("Invalid logical address!\n");
        return;
    }

    int pageNumber = logicalAddress / pageSize;
    int offset = logicalAddress % pageSize;

    if (pageNumber >= pageTableSize[processIndex]) {
        printf("Page fault! Page not allocated.\n");
        return;
    }

    int frameNumber = pageTable[processIndex][pageNumber];
    int physicalAddress = (frameNumber * pageSize) + offset;

    printf("\nLogical Address %d (Process %d)\n", logicalAddress, pid);
    printf("Page Number: %d, Offset: %d\n", pageNumber, offset);
    printf("Frame Number: %d\n", frameNumber);
    printf("Physical Address = %d KB\n", physicalAddress);
}

// Function to simulate paging and build page tables
void simulatePaging(int memSize, int pageSize, int processSize[], int numProcesses) {
    int numFrames = memSize / pageSize;

    printf("=== PAGING SIMULATION ===\n");
    printf("Total Physical Memory: %d KB\n", memSize);
    printf("Page/Frame Size: %d KB\n", pageSize);
    printf("Total Frames: %d\n\n", numFrames);

    int *pageTable[MAX_PROCESSES];
    int pageTableSize[MAX_PROCESSES] = {0};
    int frameTable[MAX_FRAMES];
    int nextFreeFrame = 0;

    for (int i = 0; i < numFrames; i++)
        frameTable[i] = -1; // all frames free

    // Allocate frames to processes
    for (int i = 0; i < numProcesses; i++) {
        int numPages = (processSize[i] + pageSize - 1) / pageSize;
        printf("Process %d (size %d KB) needs %d pages.\n", i + 1, processSize[i], numPages);

        if (nextFreeFrame + numPages > numFrames) {
            printf("Not enough free frames for Process %d!\n\n", i + 1);
            pageTable[i] = NULL;
            continue;
        }

        pageTable[i] = (int *)malloc(numPages * sizeof(int));
        if (!pageTable[i]) {
            printf("Memory allocation failed!\n");
            exit(1);
        }

        printf("Allocating frames: ");
        for (int p = 0; p < numPages; p++) {
            pageTable[i][p] = nextFreeFrame;
            frameTable[nextFreeFrame] = i;
            printf("%d ", nextFreeFrame);
            nextFreeFrame++;
        }
        pageTableSize[i] = numPages;
        printf("\n\n");
    }

    // Display page tables
    printf("=============================\n");
    printf(" PAGE TABLES\n");
    printf("=============================\n");
    for (int i = 0; i < numProcesses; i++) {
        printf("\nProcess %d Page Table:\n", i + 1);
        printf("Page No.\tFrame No.\n");
        if (pageTableSize[i] == 0) {
            printf("No pages allocated.\n");
            continue;
        }
        for (int p = 0; p < pageTableSize[i]; p++) {
            printf("%5d\t\t%5d\n", p, pageTable[i][p]);
        }
    }

    // Display physical memory layout
    printf("\n=============================\n");
    printf(" PHYSICAL MEMORY LAYOUT\n");
    printf("=============================\n");
    for (int f = 0; f < numFrames; f++) {
        printf("Frame %2d : ", f);
        if (frameTable[f] == -1)
            printf("Free\n");
        else
            printf("Occupied by Process %d\n", frameTable[f] + 1);
    }

    // Address translation loop
    printf("\n=============================\n");
    printf(" ADDRESS TRANSLATION\n");
    printf("=============================\n");
    char choice;
    do {
        int pid, logicalAddress;
        printf("\nEnter Process ID (1-%d): ", numProcesses);
        scanf("%d", &pid);

        printf("Enter Logical Address (in KB): ");
        scanf("%d", &logicalAddress);

        translateAddress(pid, logicalAddress, pageSize, numProcesses, processSize, pageTable, pageTableSize);

        printf("\nDo you want to translate another address? (y/n): ");
        scanf(" %c", &choice);
    } while (choice == 'y' || choice == 'Y');

    for (int i = 0; i < numProcesses; i++) {
        if (pageTableSize[i] > 0)
            free(pageTable[i]);
    }
}

int main() {
    int memorySize = 1024; 
    int pageSize = 128;  

    int processSizes[] = {300, 250, 500, 128};
    int numProcesses = sizeof(processSizes) / sizeof(processSizes[0]);

    simulatePaging(memorySize, pageSize, processSizes, numProcesses);

    return 0;
}
