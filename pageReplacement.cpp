#include <bits/stdc++.h>
using namespace std;

void fifo(vector<int>& pages, int capacity) {
    set<int> s;
    queue<int> indexes;
    int page_faults = 0;

    for (int page : pages) {
        if (s.size() < capacity) {
            if (s.find(page) == s.end()) {
                s.insert(page);
                page_faults++;
                indexes.push(page);
            }
        } else {
            if (s.find(page) == s.end()) {
                int val = indexes.front();
                indexes.pop();
                s.erase(val);
                s.insert(page);
                indexes.push(page);
                page_faults++;
            }
        }
    }

    cout << "FIFO Page Faults: " << page_faults << endl;
}

void lru(vector<int>& pages, int capacity) {
    set<int> s;
    map<int, int> indexes;
    int page_faults = 0;

    for (int i = 0; i < pages.size(); i++) {
        int page = pages[i];
        if (s.size() < capacity) {
            if (s.find(page) == s.end()) {
                s.insert(page);
                page_faults++;
            }
            indexes[page] = i;
        } else {
            if (s.find(page) == s.end()) {
                int lru = INT_MAX, val;
                for (auto it : s) {
                    if (indexes[it] < lru) {
                        lru = indexes[it];
                        val = it;
                    }
                }
                s.erase(val);
                s.insert(page);
                page_faults++;
            }
            indexes[page] = i;
        }
    }

    cout << "LRU Page Faults: " << page_faults << endl;
}

void optimal(vector<int>& pages, int capacity) {
    set<int> s;
    int page_faults = 0;

    for (int i = 0; i < pages.size(); i++) {
        int page = pages[i];
        if (s.size() < capacity) {
            if (s.find(page) == s.end()) {
                s.insert(page);
                page_faults++;
            }
        } else {
            if (s.find(page) == s.end()) {
                int farthest = i, val;
                for (auto it : s) {
                    int j;
                    for (j = i + 1; j < pages.size(); j++) {
                        if (pages[j] == it) {
                            if (j > farthest) {
                                farthest = j;
                                val = it;
                            }
                            break;
                        }
                    }
                    if (j == pages.size()) {
                        val = it;
                        break;
                    }
                }
                s.erase(val);
                s.insert(page);
                page_faults++;
            }
        }
    }

    cout << "Optimal Page Faults: " << page_faults << endl;
}

void lfu(vector<int>& pages, int capacity) {
    map<int, int> freq;
    set<int> s;
    int page_faults = 0;

    for (int page : pages) {
        if (s.size() < capacity) {
            if (s.find(page) == s.end()) {
                s.insert(page);
                page_faults++;
            }
            freq[page]++;
        } else {
            if (s.find(page) == s.end()) {
                int lfu = INT_MAX, val;
                for (auto it : s) {
                    if (freq[it] < lfu) {
                        lfu = freq[it];
                        val = it;
                    }
                }
                s.erase(val);
                freq.erase(val);
                s.insert(page);
                page_faults++;
            }
            freq[page]++;
        }
    }

    cout << "LFU Page Faults: " << page_faults << endl;
}

void secondChance(vector<int>& pages, int capacity) {
    set<int> s;
    queue<pair<int, bool>> q; // pair of page and reference bit
    int page_faults = 0;

    for (int page : pages) {
        if (s.size() < capacity) {
            if (s.find(page) == s.end()) {
                s.insert(page);
                q.push({page, true});
                page_faults++;
            } else {
                // Set reference bit to true
                queue<pair<int, bool>> temp;
                while (!q.empty()) {
                    auto p = q.front();
                    q.pop();
                    if (p.first == page) {
                        p.second = true;
                    }
                    temp.push(p);
                }
                q = temp;
            }
        } else {
            if (s.find(page) == s.end()) {
                while (true) {
                    auto p = q.front();
                    q.pop();
                    if (p.second) {
                        p.second = false;
                        q.push(p);
                    } else {
                        s.erase(p.first);
                        s.insert(page);
                        q.push({page, true});
                        page_faults++;
                        break;
                    }
                }
            } else {
                // Set reference bit to true
                queue<pair<int, bool>> temp;
                while (!q.empty()) {
                    auto p = q.front();
                    q.pop();
                    if (p.first == page) {
                        p.second = true;
                    }
                    temp.push(p);
                }
                q = temp;
            }
        }
    }

    cout << "Second Chance Page Faults: " << page_faults << endl;
}

int main() {
    int capacity;
    cout << "Enter number of frames: ";
    cin >> capacity;

    vector<int> pages = {1, 2, 3, 4, 2, 1, 5, 6, 2, 1, 2, 3, 7, 6, 3, 2, 1, 2, 3, 6};
    /*
    int n, page;
    cout << "Enter number of page references: ";
    cin >> n;
    cout << "Enter the page reference string: ";
    for (int i = 0; i < n; i++) {
        cin >> page;
        pages.push_back(page);
    }
    */

    fifo(pages, capacity);
    lru(pages, capacity);
    optimal(pages, capacity);
    lfu(pages, capacity);
    secondChance(pages, capacity);

    return 0;
}