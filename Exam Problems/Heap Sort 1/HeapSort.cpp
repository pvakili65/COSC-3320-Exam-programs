#include <iostream>
#include <iomanip>
#include <cmath>
using namespace std;

int swapCount = 0;

void PrintArray(int arr[], int size, int heapSize = -1, int highlightIdx1 = -1, int highlightIdx2 = -1){
    cout << "   Array: [";
    for(int i = 0; i < size; i++){
        if(i == highlightIdx1 || i == highlightIdx2){
            cout << "(" << arr[i] << ")";
        } else {
            cout << " " << arr[i] << " ";
        }
        if(i < size - 1) cout << ",";
    }
    cout << "]";
    if(heapSize >= 0 && heapSize < size){
        cout << "  (heap size: " << heapSize << ", sorted: " << size - heapSize << ")";
    }
    cout << endl;
}

void PrintHeapTree(int arr[], int heapSize, int totalSize){
    if(heapSize <= 0) return;
    
    int height = (int)log2(heapSize) + 1;
    int maxWidth = (1 << height) * 3;
    
    cout << "   Heap tree:" << endl;
    
    int index = 0;
    for(int level = 0; level < height && index < heapSize; level++){
        int nodesInLevel = 1 << level;
        int spacing = maxWidth / (nodesInLevel + 1);
        
        string line = "   ";
        for(int i = 0; i < nodesInLevel && index < heapSize; i++){
            int pos = spacing * (i + 1);
            while(line.length() < pos) line += " ";
            
            string val = to_string(arr[index]);
            line += val;
            index++;
        }
        cout << line << endl;
        
        // Print branches if not last level
        if(level < height - 1 && index < heapSize){
            string branchLine = "   ";
            int parentIndex = (1 << level) - 1;
            int branchSpacing = maxWidth / ((nodesInLevel * 2) + 1);
            
            for(int i = 0; i < nodesInLevel; i++){
                int parentPos = spacing * (i + 1);
                int leftChildIdx = 2 * (parentIndex + i) + 1;
                int rightChildIdx = 2 * (parentIndex + i) + 2;
                
                if(leftChildIdx < heapSize){
                    int leftPos = parentPos - spacing/4;
                    if(leftPos < 3) leftPos = 3;
                    while(branchLine.length() < leftPos) branchLine += " ";
                    branchLine += "/";
                }
                if(rightChildIdx < heapSize){
                    int rightPos = parentPos + spacing/4 + 1;
                    while(branchLine.length() < rightPos) branchLine += " ";
                    branchLine += "\\";
                }
            }
            cout << branchLine << endl;
        }
    }
}

void MaxHeapPercolateDown(int index, int arr[], int size, bool showSteps = true, int depth = 0){
    int leftChildIndex = 2 * index + 1;
    int rightChildIndex = 2 * index + 2;
    int largestIndex = index;

    if(showSteps && depth == 0){
        cout << "      Percolating down from index " << index << " (value: " << arr[index] << ")" << endl;
    }

    if(leftChildIndex < size && arr[leftChildIndex] > arr[largestIndex]){
        largestIndex = leftChildIndex;
    }
    if(rightChildIndex < size && arr[rightChildIndex] > arr[largestIndex]){
        largestIndex = rightChildIndex;
    }
    
    if(largestIndex != index){
        if(showSteps){
            cout << "      Swap arr[" << index << "]=" << arr[index] << " with arr[" << largestIndex << "]=" << arr[largestIndex] << endl;
        }
        swapCount++;
        
        int temp = arr[largestIndex];
        arr[largestIndex] = arr[index];
        arr[index] = temp;
        
        MaxHeapPercolateDown(largestIndex, arr, size, showSteps, depth + 1);
    } else if(showSteps && depth == 0){
        cout << "      No swap needed (heap property satisfied)" << endl;
    }
}

void Heapify(int arr[], int size){
    cout << "=== PHASE 1: BUILD MAX HEAP (Heapify) ===" << endl;
    cout << "Starting from last non-leaf node (index " << size/2 - 1 << ") and working up to root" << endl;
    cout << endl;
    
    cout << "Initial array:" << endl;
    PrintArray(arr, size);
    PrintHeapTree(arr, size, size);
    cout << endl;
    
    for(int i = size/2 - 1; i >= 0; i--){
        cout << "--- Step " << (size/2 - 1 - i + 1) << ": Process node at index " << i << " (value: " << arr[i] << ") ---" << endl;
        
        // Show children
        int left = 2*i + 1;
        int right = 2*i + 2;
        cout << "   Children: ";
        if(left < size) cout << "left[" << left << "]=" << arr[left];
        if(right < size) cout << ", right[" << right << "]=" << arr[right];
        cout << endl;
        
        MaxHeapPercolateDown(i, arr, size);
        
        cout << "   Result:" << endl;
        PrintArray(arr, size);
        PrintHeapTree(arr, size, size);
        cout << endl;
    }
    
    cout << "=== MAX HEAP BUILT ===" << endl;
    PrintArray(arr, size);
    PrintHeapTree(arr, size, size);
    cout << endl;
}

void HeapSort(int arr[], int size){
    cout << "****************************************" << endl;
    cout << "*          HEAPSORT ALGORITHM          *" << endl;
    cout << "****************************************" << endl;
    cout << endl;
    
    cout << "Input array: [";
    for(int i = 0; i < size; i++){
        cout << arr[i];
        if(i < size - 1) cout << ", ";
    }
    cout << "]" << endl;
    cout << "Size: " << size << endl;
    cout << endl;
    
    // Phase 1: Build heap
    Heapify(arr, size);
    
    // Phase 2: Extract elements
    cout << "=== PHASE 2: EXTRACT ELEMENTS ===" << endl;
    cout << "Repeatedly swap root (max) with last heap element, reduce heap size, and restore heap" << endl;
    cout << endl;
    
    for(int i = size - 1; i > 0; i--){
        cout << "--- Extraction " << (size - i) << " of " << (size - 1) << " ---" << endl;
        cout << "   Swap root arr[0]=" << arr[0] << " (max) with arr[" << i << "]=" << arr[i] << " (last in heap)" << endl;
        
        int temp = arr[0];
        arr[0] = arr[i];
        arr[i] = temp;
        swapCount++;
        
        cout << "   After swap:" << endl;
        PrintArray(arr, size, i);
        
        cout << "   Value " << arr[i] << " is now in final sorted position" << endl;
        cout << "   Restore heap property (heap size now " << i << "):" << endl;
        
        MaxHeapPercolateDown(0, arr, i);
        
        cout << "   Result:" << endl;
        PrintArray(arr, size, i);
        if(i > 1){
            PrintHeapTree(arr, i, size);
        }
        cout << endl;
    }
    
    cout << "=== SORTING COMPLETE ===" << endl;
    cout << "Total swaps: " << swapCount << endl;
    cout << endl;
}

int main(){
    int arr[] = {10, 1, 3, 2, 8, 5, 6, 4, 7, 9};
    int size = 10;
    
    HeapSort(arr, size);
    
    cout << "Final sorted array: [";
    for(int i = 0; i < size; i++){
        cout << arr[i];
        if(i < size - 1) cout << ", ";
    }
    cout << "]" << endl;
    
    return 0;
}