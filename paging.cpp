#include <iostream>
#include <vector>
#include <iomanip>
using namespace std;

// Function to translate logical to physical address
void translateAddress(int pid, int logicalAddress, int pageSize, const vector<int>& processSize, const vector<vector<int>>& pageTable) {
    int processIndex = pid - 1;

    if (processIndex < 0 || processIndex >= processSize.size()) {
        cout << "Invalid process ID!\n";
        return;
    }

    if (pageTable[processIndex].empty()) {
        cout << "Process " << pid << " not allocated any pages!\n";
        return;
    }

    if (logicalAddress < 0 || logicalAddress >= processSize[processIndex]) {
        cout << "Invalid logical address!\n";
        return;
    }

    int pageNumber = logicalAddress / pageSize;
    int offset = logicalAddress % pageSize;

    if (pageNumber >= pageTable[processIndex].size()) {
        cout << "Page fault! Page not allocated.\n";
        return;
    }

    int frameNumber = pageTable[processIndex][pageNumber];
    int physicalAddress = (frameNumber * pageSize) + offset;

    cout << "Logical Address " << logicalAddress << " (Process " << pid << ")\n";
    cout << "Page Number: " << pageNumber << ", Offset: " << offset << endl;
    cout << "Frame Number: " << frameNumber << endl;
    cout << "Physical Address = " << physicalAddress << " KB\n";
}

// Function to simulate paging and build page tables
void simulatePaging(int memSize, int pageSize, const vector<int>& processSize) {
    int numFrames = memSize / pageSize;
    int numProcesses = processSize.size();

    cout << "=== PAGING SIMULATION ===\n";
    cout << "Total Physical Memory: " << memSize << " KB\n";
    cout << "Page/Frame Size: " << pageSize << " KB\n";
    cout << "Total Frames: " << numFrames << "\n\n";

    vector<vector<int>> pageTable(numProcesses); // page table for each process
    vector<int> frameTable(numFrames, -1);       // -1 indicates free frame
    int nextFreeFrame = 0;

    // Allocate frames to processes
    for (int i = 0; i < numProcesses; i++) {
        int numPages = (processSize[i] + pageSize - 1) / pageSize; // ceil division
        cout << "Process " << i + 1 << " (size " << processSize[i]
             << " KB) needs " << numPages << " pages.\n";

        if (nextFreeFrame + numPages > numFrames) {
            cout << "Not enough free frames for Process " << i + 1 << "!\n\n";
            continue;
        }

        cout << "Allocating frames: ";
        for (int p = 0; p < numPages; p++) {
            pageTable[i].push_back(nextFreeFrame);
            frameTable[nextFreeFrame] = i;
            cout << nextFreeFrame << " ";
            nextFreeFrame++;
        }
        cout << "\n\n";
    }

    // Display page tables
    cout << "=============================\n";
    cout << " PAGE TABLES\n";
    cout << "=============================\n";
    for (int i = 0; i < numProcesses; i++) {
        cout << "\nProcess " << i + 1 << " Page Table:\n";
        cout << "Page No.\tFrame No.\n";
        for (int p = 0; p < pageTable[i].size(); p++) {
            cout << setw(5) << p << "\t\t" << setw(5) << pageTable[i][p] << endl;
        }
        if (pageTable[i].empty())
            cout << "No pages allocated.\n";
    }

    // Display physical memory layout
    cout << "\n=============================\n";
    cout << " PHYSICAL MEMORY LAYOUT\n";
    cout << "=============================\n";
    for (int f = 0; f < numFrames; f++) {
        cout << "Frame " << setw(2) << f << " : ";
        if (frameTable[f] == -1)
            cout << "Free\n";
        else
            cout << "Occupied by Process " << frameTable[f] + 1 << endl;
    }

    // Address translation loop
    cout << "\n=============================\n";
    cout << " ADDRESS TRANSLATION\n";
    cout << "=============================\n";
    char choice;
    do {
        int pid, logicalAddress;
        cout << "\nEnter Process ID (1-" << numProcesses << "): ";
        cin >> pid;

        cout << "Enter Logical Address (in KB): ";
        cin >> logicalAddress;

        translateAddress(pid, logicalAddress, pageSize, processSize, pageTable);

        cout << "\nDo you want to translate another address? (y/n): ";
        cin >> choice;
    } while (choice == 'y' || choice == 'Y');
}

int main() {
    int memorySize = 1024; 
    int pageSize = 128; 

    vector<int> processSizes = {300, 250, 500, 128};
    simulatePaging(memorySize, pageSize, processSizes);

    return 0;
}
