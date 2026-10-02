#include "Sorter.h"
#include <utility>
using namespace std;

static int fieldValue(const Process &p, SortField field) {
    switch (field) {
        case FIELD_PID:      return p.pid;
        case FIELD_ARRIVAL:  return p.arrivalTime;
        case FIELD_BURST:    return p.burstTime;
        case FIELD_PRIORITY: return p.priority;
    }
    return 0;
}

static bool lessThan(const Process &a, const Process &b, SortField field) {
    return fieldValue(a, field) < fieldValue(b, field);
}

const char *sortFieldName(SortField field) {
    switch (field) {
        case FIELD_PID:      return "Process ID";
        case FIELD_ARRIVAL:  return "Arrival Time";
        case FIELD_BURST:    return "Burst Time";
        case FIELD_PRIORITY: return "Priority";
    }
    return "";
}

const char *sortAlgoName(SortAlgo algo) {
    switch (algo) {
        case ALGO_BUBBLE:    return "Bubble Sort";
        case ALGO_SELECTION: return "Selection Sort";
        case ALGO_INSERTION: return "Insertion Sort";
        case ALGO_MERGE:     return "Merge Sort";
        case ALGO_QUICK:     return "Quick Sort";
        case ALGO_HEAP:      return "Heap Sort";
    }
    return "";
}

// ---------------------------------------------------------------- Bubble --
static void bubbleSort(vector<Process> &v, SortField field) {
    int n = (int)v.size();
    for (int i = 0; i < n - 1; i++) {
        bool swapped = false;
        for (int j = 0; j < n - 1 - i; j++) {
            if (lessThan(v[j + 1], v[j], field)) {
                swap(v[j], v[j + 1]);
                swapped = true;
            }
        }
        if (!swapped) break;
    }
}

// ------------------------------------------------------------- Selection --
static void selectionSort(vector<Process> &v, SortField field) {
    int n = (int)v.size();
    for (int i = 0; i < n - 1; i++) {
        int minIdx = i;
        for (int j = i + 1; j < n; j++) if (lessThan(v[j], v[minIdx], field)) minIdx = j;
        if (minIdx != i) swap(v[i], v[minIdx]);
    }
}

// ------------------------------------------------------------- Insertion --
static void insertionSort(vector<Process> &v, SortField field) {
    int n = (int)v.size();
    for (int i = 1; i < n; i++) {
        Process key = v[i];
        int j = i - 1;
        while (j >= 0 && lessThan(key, v[j], field)) {
            v[j + 1] = v[j];
            j--;
        }
        v[j + 1] = key;
    }
}

// ----------------------------------------------------------------- Merge --
static void mergeRanges(vector<Process> &v, int left, int mid, int right, SortField field) {
    vector<Process> leftPart(v.begin() + left, v.begin() + mid + 1);
    vector<Process> rightPart(v.begin() + mid + 1, v.begin() + right + 1);

    size_t i = 0, j = 0;
    int k = left;
    while (i < leftPart.size() && j < rightPart.size()) {
        if (fieldValue(leftPart[i], field) <= fieldValue(rightPart[j], field)) v[k++] = leftPart[i++];
        else v[k++] = rightPart[j++];
    }
    while (i < leftPart.size()) v[k++] = leftPart[i++];
    while (j < rightPart.size()) v[k++] = rightPart[j++];
}

static void mergeSort(vector<Process> &v, int left, int right, SortField field) {
    if (left >= right) return;
    int mid = left + (right - left) / 2;
    mergeSort(v, left, mid, field);
    mergeSort(v, mid + 1, right, field);
    mergeRanges(v, left, mid, right, field);
}

// ----------------------------------------------------------------- Quick --
static int partitionRange(vector<Process> &v, int low, int high, SortField field) {
    int pivotValue = fieldValue(v[high], field);
    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (fieldValue(v[j], field) <= pivotValue) {
            i++;
            swap(v[i], v[j]);
        }
    }
    swap(v[i + 1], v[high]);
    return i + 1;
}

static void quickSort(vector<Process> &v, int low, int high, SortField field) {
    if (low < high) {
        int pivotIdx = partitionRange(v, low, high, field);
        quickSort(v, low, pivotIdx - 1, field);
        quickSort(v, pivotIdx + 1, high, field);
    }
}

// -------------------------------------------------------------- Heap Sort --
static void siftDown(vector<Process> &v, int n, int rootIdx, SortField field) {
    int largest = rootIdx;
    int left = 2 * rootIdx + 1;
    int right = 2 * rootIdx + 2;

    if (left < n && fieldValue(v[left], field) > fieldValue(v[largest], field)) largest = left;
    if (right < n && fieldValue(v[right], field) > fieldValue(v[largest], field)) largest = right;

    if (largest != rootIdx) {
        swap(v[rootIdx], v[largest]);
        siftDown(v, n, largest, field);
    }
}

static void heapSort(vector<Process> &v, SortField field) {
    int n = (int)v.size();
    // Phase 1: build a MAX HEAP in place.
    for (int i = n / 2 - 1; i >= 0; --i) siftDown(v, n, i, field);
    // Phase 2: repeatedly move the max to the end, shrink the heap.
    for (int end = n - 1; end > 0; --end) {
        swap(v[0], v[end]);
        siftDown(v, end, 0, field);
    }
}

// ------------------------------------------------------------ dispatcher --
void sortProcesses(vector<Process> &v, SortField field, SortAlgo algo) {
    if (v.empty()) return;
    switch (algo) {
        case ALGO_BUBBLE:    bubbleSort(v, field); break;
        case ALGO_SELECTION: selectionSort(v, field); break;
        case ALGO_INSERTION: insertionSort(v, field); break;
        case ALGO_MERGE:     mergeSort(v, 0, (int)v.size() - 1, field); break;
        case ALGO_QUICK:     quickSort(v, 0, (int)v.size() - 1, field); break;
        case ALGO_HEAP:      heapSort(v, field); break;
    }
}
