#include "BinarySearch.h"
using namespace std;

int binarySearchByPid(const vector<Process> &sortedByPid, int pid) {
    int low = 0;
    int high = (int)sortedByPid.size() - 1;

    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (sortedByPid[mid].pid == pid) {
            return mid;
        } else if (sortedByPid[mid].pid < pid) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return -1; // not found
}
