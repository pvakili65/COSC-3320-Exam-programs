#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

enum PivotPosition { FIRST, MIDDLE, LAST };

void printArray(vector<int>& arr) {
    cout << "[";
    for (size_t i = 0; i < arr.size(); i++) {
        cout << arr[i];
        if (i < arr.size() - 1) cout << ", ";
    }
    cout << "]" << endl;
}

int getPivotIndex(int low, int high, PivotPosition pivotPos) {
    switch (pivotPos) {
        case FIRST:  return low;
        case MIDDLE: return (low + high) / 2;
        case LAST:   return high;
    }
    return low;
}

/*
 * Different approach: Generate the array by placing values based on 
 * what position they will be selected as pivot from.
 * 
 * Build a mapping from "sorted position" to "initial position"
 * based on the sequence of pivot selections.
 */

// Track where each sorted value should go in the initial array
void assignPositions(vector<int>& positionOf, int arrLow, int arrHigh, int valLow, int valHigh, PivotPosition pivotPos) {
    if (arrLow > arrHigh) return;
    
    int size = arrHigh - arrLow + 1;
    if (size == 1) {
        positionOf[valLow] = arrLow;
        return;
    }
    
    // For balanced partition, pivot should be the median
    int leftCount = (size - 1) / 2;
    int medianVal = valLow + leftCount;
    
    // Where is the pivot selected from?
    int pivotIdx = getPivotIndex(arrLow, arrHigh, pivotPos);
    
    // Place median value at the pivot selection position
    positionOf[medianVal] = pivotIdx;
    
    // After partition, quicksort recurses on:
    // - Left: [arrLow .. arrLow + leftCount - 1]
    // - Right: [arrLow + leftCount + 1 .. arrHigh]
    // These are the FINAL positions after partition
    
    // We need to figure out what INITIAL positions map to these final positions
    // This depends on how Lomuto partition moves elements
    
    // For FIRST pivot:
    //   pivot at arrLow, elements < pivot get swapped to front
    //   After: [elems < pivot | pivot | elems > pivot]
    //   Initial left elements: positions [arrLow+1 .. arrLow+leftCount]
    //   Initial right elements: positions [arrLow+leftCount+1 .. arrHigh]
    
    // For MIDDLE pivot:
    //   pivot at (arrLow+arrHigh)/2
    //   Elements get rearranged around the middle
    //   This is complex because the middle position itself changes
    
    // For LAST pivot:
    //   pivot at arrHigh
    //   After: [elems < pivot | pivot | elems > pivot]
    //   Initial left elements need to be at positions that end up at [arrLow..arrLow+leftCount-1]
    //   Initial right elements need to be at positions that end up at [arrLow+leftCount+1..arrHigh]
    
    // The key insight: in Lomuto partition with last pivot:
    //   - Elements < pivot that start at positions >= arrLow+leftCount will be swapped left
    //   - Elements > pivot that start at positions < arrLow+leftCount will be swapped right
    //   - If we place elements optimally, no extra swaps needed except pivot
    
    // Simplification: assign left values to first `leftCount` positions (excluding pivot position)
    // and right values to remaining positions (excluding pivot position)
    
    int leftPos = arrLow;
    int rightPos = arrLow + leftCount;  // This might include pivot position, need to skip it
    
    // Positions for left values (values < median)
    vector<int> leftPositions, rightPositions;
    for (int p = arrLow; p <= arrHigh; p++) {
        if (p == pivotIdx) continue;  // Skip pivot position
        if (leftPositions.size() < leftCount) {
            leftPositions.push_back(p);
        } else {
            rightPositions.push_back(p);
        }
    }
    
    // Assign left values to leftPositions
    for (int i = 0; i < leftCount; i++) {
        positionOf[valLow + i] = leftPositions[i];
    }
    
    // Assign right values to rightPositions
    int rightCount = size - 1 - leftCount;
    for (int i = 0; i < rightCount; i++) {
        positionOf[medianVal + 1 + i] = rightPositions[i];
    }
    
    // Recurse based on where quicksort will recurse AFTER partition
    // After partition: left subarray is [arrLow .. arrLow+leftCount-1]
    //                  right subarray is [arrLow+leftCount+1 .. arrHigh]
    // But we're assigning to INITIAL positions, so we use leftPositions/rightPositions ranges
    
    // Actually, let's think recursively about the STRUCTURE, not the positions:
    // The left subarray (after partition) will have leftCount elements with values [valLow..medianVal-1]
    // The right subarray (after partition) will have rightCount elements with values [medianVal+1..valHigh]
    // 
    // For the recursive structure to work, we need the INITIAL positions of these subarrays
    // to maintain the property that the pivot selection will pick the median.
    
    // For FIRST pivot: left positions [arrLow+1..arrLow+leftCount], right [arrLow+leftCount+1..arrHigh]
    // For LAST pivot: left positions [arrLow..arrLow+leftCount-1], right [arrLow+leftCount..arrHigh-1]
    
    int leftArrLow, leftArrHigh, rightArrLow, rightArrHigh;
    switch (pivotPos) {
        case FIRST:
            leftArrLow = arrLow + 1;
            leftArrHigh = arrLow + leftCount;
            rightArrLow = arrLow + leftCount + 1;
            rightArrHigh = arrHigh;
            break;
        case MIDDLE:
            leftArrLow = arrLow;
            leftArrHigh = pivotIdx - 1;
            rightArrLow = pivotIdx + 1;
            rightArrHigh = arrHigh;
            break;
        case LAST:
            leftArrLow = arrLow;
            leftArrHigh = arrLow + leftCount - 1;
            rightArrLow = arrLow + leftCount;
            rightArrHigh = arrHigh - 1;
            break;
    }
    
    if (leftCount > 0) {
        assignPositions(positionOf, leftArrLow, leftArrHigh, valLow, medianVal - 1, pivotPos);
    }
    if (rightCount > 0) {
        assignPositions(positionOf, rightArrLow, rightArrHigh, medianVal + 1, valHigh, pivotPos);
    }
}

void quickSortVerify(vector<int> arr, int low, int high, PivotPosition pivotPos, int depth) {
    if (low >= high) return;
    
    string indent(depth * 2, ' ');
    int pivotIdx = getPivotIndex(low, high, pivotPos);
    int pivot = arr[pivotIdx];
    
    cout << indent << "[" << low << ".." << high << "] pivot=" << pivot;
    
    swap(arr[pivotIdx], arr[high]);
    int i = low;
    for (int j = low; j < high; j++) {
        if (arr[j] < pivot) {
            swap(arr[i], arr[j]);
            i++;
        }
    }
    swap(arr[i], arr[high]);
    
    int finalPos = i;
    int leftSz = finalPos - low;
    int rightSz = high - finalPos;
    int diff = abs(leftSz - rightSz);
    
    cout << " -> pos " << finalPos << " (L:" << leftSz << " R:" << rightSz << ")";
    cout << (diff <= 1 ? " BALANCED" : " UNBALANCED") << endl;
    
    quickSortVerify(arr, low, finalPos - 1, pivotPos, depth + 1);
    quickSortVerify(arr, finalPos + 1, high, pivotPos, depth + 1);
}

int main() {
    int n, choice;
    
    cout << "======================================" << endl;
    cout << "  OPTIMAL QUICKSORT ARRAY GENERATOR  " << endl;
    cout << "======================================" << endl;
    
    cout << "\nEnter n: ";
    cin >> n;
    
    cout << "\nPivot position:\n";
    cout << "  1. First element\n";
    cout << "  2. Middle element\n";
    cout << "  3. Last element\n";
    cout << "Choice: ";
    cin >> choice;
    
    PivotPosition pivotPos;
    string name;
    switch (choice) {
        case 1: pivotPos = FIRST; name = "First"; break;
        case 2: pivotPos = MIDDLE; name = "Middle"; break;
        case 3: pivotPos = LAST; name = "Last"; break;
        default: cout << "Invalid\n"; return 1;
    }
    
    // positionOf[v] = initial position where value v should go
    vector<int> positionOf(n + 1);
    
    // For LAST pivot, generate FIRST pivot optimal then reverse
    PivotPosition genPivot = (pivotPos == LAST) ? FIRST : pivotPos;
    assignPositions(positionOf, 0, n - 1, 1, n, genPivot);
    
    // Build result array
    vector<int> result(n);
    for (int v = 1; v <= n; v++) {
        result[positionOf[v]] = v;
    }
    
    // Reverse if LAST pivot
    if (pivotPos == LAST) {
        reverse(result.begin(), result.end());
    }
    
    cout << "\n======================================" << endl;
    cout << "              RESULT                  " << endl;
    cout << "======================================" << endl;
    cout << "\nPivot selection: " << name << " element" << endl;
    cout << "Array size: " << n << endl;
    cout << "Values: 1 to " << n << endl;
    cout << "\nOptimal array: ";
    printArray(result);
    
    cout << "\n======================================" << endl;
    cout << "    QUICKSORT TRACE (VERIFICATION)    " << endl;
    cout << "======================================\n" << endl;
    quickSortVerify(result, 0, n - 1, pivotPos, 0);
    
    vector<int> sorted = result;
    sort(sorted.begin(), sorted.end());
    cout << "\nFinal sorted array: ";
    printArray(sorted);
    
    return 0;
}