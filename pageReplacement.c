#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>

#define MAX_PAGES 100

// Helper function to check if page is in frames
int isInFrames(int frames[], int capacity, int page) {
    for (int i = 0; i < capacity; i++) {
        if (frames[i] == page)
            return 1;
    }
    return 0;
}

// FIFO Page Replacement
void fifo(int pages[], int n, int capacity) {
    int frames[MAX_PAGES];
    int front = 0, rear = 0, size = 0;
    int page_faults = 0;

    for (int i = 0; i < n; i++) {
        int page = pages[i];
        if (!isInFrames(frames, size, page)) {
            if (size < capacity) {
                frames[rear++] = page;
                size++;
            } else {
                frames[front] = page;
                front = (front + 1) % capacity;
                rear = (rear + 1) % capacity;
            }
            page_faults++;
        }
    }

    printf("FIFO Page Faults: %d\n", page_faults);
}

// LRU Page Replacement
void lru(int pages[], int n, int capacity) {
    int frames[MAX_PAGES];
    int time[MAX_PAGES];
    int page_faults = 0;
    int t = 0;

    for (int i = 0; i < capacity; i++) {
        frames[i] = -1;
        time[i] = 0;
    }

    for (int i = 0; i < n; i++) {
        int page = pages[i];
        int found = 0;

        for (int j = 0; j < capacity; j++) {
            if (frames[j] == page) {
                found = 1;
                time[j] = ++t;
                break;
            }
        }

        if (!found) {
            int lruIndex = 0, minTime = INT_MAX;
            for (int j = 0; j < capacity; j++) {
                if (frames[j] == -1) {
                    lruIndex = j;
                    break;
                }
                if (time[j] < minTime) {
                    minTime = time[j];
                    lruIndex = j;
                }
            }
            frames[lruIndex] = page;
            time[lruIndex] = ++t;
            page_faults++;
        }
    }

    printf("LRU Page Faults: %d\n", page_faults);
}

// Optimal Page Replacement
void optimal(int pages[], int n, int capacity) {
    int frames[MAX_PAGES];
    int page_faults = 0;

    for (int i = 0; i < capacity; i++)
        frames[i] = -1;

    for (int i = 0; i < n; i++) {
        int page = pages[i];
        if (!isInFrames(frames, capacity, page)) {
            int replaceIndex = -1, farthest = i;
            for (int j = 0; j < capacity; j++) {
                if (frames[j] == -1) {
                    replaceIndex = j;
                    break;
                }
                int k;
                for (k = i + 1; k < n; k++) {
                    if (frames[j] == pages[k])
                        break;
                }
                if (k == n) {
                    replaceIndex = j;
                    break;
                }
                if (k > farthest) {
                    farthest = k;
                    replaceIndex = j;
                }
            }
            frames[replaceIndex] = page;
            page_faults++;
        }
    }

    printf("Optimal Page Faults: %d\n", page_faults);
}

// LFU Page Replacement
void lfu(int pages[], int n, int capacity) {
    int frames[MAX_PAGES];
    int freq[MAX_PAGES] = {0};
    int page_faults = 0;

    for (int i = 0; i < capacity; i++)
        frames[i] = -1;

    for (int i = 0; i < n; i++) {
        int page = pages[i];
        int found = 0;

        for (int j = 0; j < capacity; j++) {
            if (frames[j] == page) {
                freq[j]++;
                found = 1;
                break;
            }
        }

        if (!found) {
            int lfuIndex = -1, minFreq = INT_MAX;
            for (int j = 0; j < capacity; j++) {
                if (frames[j] == -1) {
                    lfuIndex = j;
                    break;
                }
                if (freq[j] < minFreq) {
                    minFreq = freq[j];
                    lfuIndex = j;
                }
            }
            frames[lfuIndex] = page;
            freq[lfuIndex] = 1;
            page_faults++;
        }
    }

    printf("LFU Page Faults: %d\n", page_faults);
}

// Second Chance Page Replacement
typedef struct {
    int page;
    bool ref_bit;
} Frame;

void secondChance(int pages[], int n, int capacity) {
    Frame frames[MAX_PAGES];
    int pointer = 0, page_faults = 0, size = 0;

    for (int i = 0; i < capacity; i++) {
        frames[i].page = -1;
        frames[i].ref_bit = false;
    }

    for (int i = 0; i < n; i++) {
        int page = pages[i];
        int found = 0;
        for (int j = 0; j < capacity; j++) {
            if (frames[j].page == page) {
                frames[j].ref_bit = true;
                found = 1;
                break;
            }
        }

        if (!found) {
            while (frames[pointer].ref_bit) {
                frames[pointer].ref_bit = false;
                pointer = (pointer + 1) % capacity;
            }
            frames[pointer].page = page;
            frames[pointer].ref_bit = true;
            pointer = (pointer + 1) % capacity;
            page_faults++;
        }
    }

    printf("Second Chance Page Faults: %d\n", page_faults);
}

int main() {
    int capacity;
    printf("Enter number of frames: ");
    scanf("%d", &capacity);

    int pages[] = {1, 2, 3, 4, 2, 1, 5, 6, 2, 1, 2, 3, 7, 6, 3, 2, 1, 2, 3, 6};
    int n = sizeof(pages) / sizeof(pages[0]);

    fifo(pages, n, capacity);
    lru(pages, n, capacity);
    optimal(pages, n, capacity);
    lfu(pages, n, capacity);
    secondChance(pages, n, capacity);

    return 0;
}
