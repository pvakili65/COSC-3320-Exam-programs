#include <iostream>
#include <iomanip>
#include <cmath>
using namespace std;

void PrintArray(int arr[], int size, string label){
    cout << "   " << label << ": [";
    for(int i = 0; i < size; i++){
        cout << arr[i];
        if(i < size - 1) cout << ", ";
    }
    cout << "]" << endl;
}

void PrintHeapTree(int arr[], int size){
    if(size <= 0){
        cout << "   (empty)" << endl;
        return;
    }
    
    int height = (int)log2(size) + 1;
    int maxWidth = (1 << height) * 3;
    
    int index = 0;
    for(int level = 0; level < height && index < size; level++){
        int nodesInLevel = 1 << level;
        int spacing = maxWidth / (nodesInLevel + 1);
        
        string line = "   ";
        for(int i = 0; i < nodesInLevel && index < size; i++){
            int pos = spacing * (i + 1);
            while(line.length() < pos) line += " ";
            
            string val = to_string(arr[index]);
            line += val;
            index++;
        }
        cout << line << endl;
        
        // Print branches if not last level
        if(level < height - 1 && index < size){
            string branchLine = "   ";
            int parentIndex = (1 << level) - 1;
            
            for(int i = 0; i < nodesInLevel; i++){
                int parentPos = spacing * (i + 1);
                int leftChildIdx = 2 * (parentIndex + i) + 1;
                int rightChildIdx = 2 * (parentIndex + i) + 2;
                
                if(leftChildIdx < size){
                    int leftPos = parentPos - spacing/4;
                    if(leftPos < 3) leftPos = 3;
                    while(branchLine.length() < leftPos) branchLine += " ";
                    branchLine += "/";
                }
                if(rightChildIdx < size){
                    int rightPos = parentPos + spacing/4 + 1;
                    while(branchLine.length() < rightPos) branchLine += " ";
                    branchLine += "\\";
                }
            }
            cout << branchLine << endl;
        }
    }
}

void PrintAllArrays(int input[], int inputSize, int heap[], int heapSize, int sorted[], int sortedSize){
    cout << "   +-----------------+-----------------+-----------------+" << endl;
    cout << "   |  Input Array    |   Heap Array    |  Sorted Array   |" << endl;
    cout << "   +-----------------+-----------------+-----------------+" << endl;
    
    // Print arrays
    cout << "   |";
    
    // Input array
    string inputStr = " [";
    for(int i = 0; i < inputSize; i++){
        inputStr += to_string(input[i]);
        if(i < inputSize - 1) inputStr += ",";
    }
    inputStr += "]";
    cout << left << setw(17) << inputStr << "|";
    
    // Heap array
    string heapStr = " [";
    for(int i = 0; i < heapSize; i++){
        heapStr += to_string(heap[i]);
        if(i < heapSize - 1) heapStr += ",";
    }
    heapStr += "]";
    cout << left << setw(17) << heapStr << "|";
    
    // Sorted array
    string sortedStr = " [";
    for(int i = 0; i < sortedSize; i++){
        sortedStr += to_string(sorted[i]);
        if(i < sortedSize - 1) sortedStr += ",";
    }
    sortedStr += "]";
    cout << left << setw(17) << sortedStr << "|" << endl;
    
    cout << "   +-----------------+-----------------+-----------------+" << endl;
}

// Percolate UP for insertion (max heap)
void MaxHeapPercolateUp(int heap[], int index){
    while(index > 0){
        int parentIndex = (index - 1) / 2;
        if(heap[index] <= heap[parentIndex]){
            cout << "      Index " << index << " (value " << heap[index] << ") <= parent index " << parentIndex << " (value " << heap[parentIndex] << "). Stop." << endl;
            return;
        }
        cout << "      Swap index " << index << " (value " << heap[index] << ") with parent index " << parentIndex << " (value " << heap[parentIndex] << ")" << endl;
        
        int temp = heap[index];
        heap[index] = heap[parentIndex];
        heap[parentIndex] = temp;
        
        index = parentIndex;
    }
    cout << "      Reached root. Stop." << endl;
}

// Percolate DOWN for extraction (max heap)
void MaxHeapPercolateDown(int heap[], int index, int size){
    while(true){
        int leftChildIndex = 2 * index + 1;
        int rightChildIndex = 2 * index + 2;
        int largestIndex = index;
        
        if(leftChildIndex < size && heap[leftChildIndex] > heap[largestIndex]){
            largestIndex = leftChildIndex;
        }
        if(rightChildIndex < size && heap[rightChildIndex] > heap[largestIndex]){
            largestIndex = rightChildIndex;
        }
        
        if(largestIndex == index){
            cout << "      Index " << index << " (value " << heap[index] << ") >= both children. Heap property satisfied." << endl;
            return;
        }
        
        cout << "      Swap index " << index << " (value " << heap[index] << ") with index " << largestIndex << " (value " << heap[largestIndex] << ")" << endl;
        
        int temp = heap[largestIndex];
        heap[largestIndex] = heap[index];
        heap[index] = temp;
        
        index = largestIndex;
    }
}

void HeapSort(int input[], int size){
    cout << "****************************************" << endl;
    cout << "*     HEAPSORT WITH THREE ARRAYS       *" << endl;
    cout << "****************************************" << endl;
    cout << endl;
    
    // Create separate arrays
    int* heap = new int[size];
    int* sorted = new int[size];
    int heapSize = 0;
    int sortedSize = 0;
    
    cout << "Initial state:" << endl;
    PrintArray(input, size, "Input ");
    cout << "   Heap:   (empty)" << endl;
    cout << "   Sorted: (empty)" << endl;
    cout << endl;
    
    // ==================== PHASE 1: BUILD HEAP ====================
    cout << "============================================" << endl;
    cout << "  PHASE 1: INSERT ALL ELEMENTS INTO HEAP" << endl;
    cout << "============================================" << endl;
    cout << "Insert each element at the end, then percolate UP to maintain max-heap property." << endl;
    cout << endl;
    
    for(int i = 0; i < size; i++){
        cout << "--- Insert Step " << (i + 1) << ": Insert " << input[i] << " into heap ---" << endl;
        
        // Insert at end of heap
        heap[heapSize] = input[i];
        heapSize++;
        
        cout << "   Added " << input[i] << " at index " << (heapSize - 1) << endl;
        cout << "   Heap array after insert: [";
        for(int j = 0; j < heapSize; j++){
            cout << heap[j];
            if(j < heapSize - 1) cout << ", ";
        }
        cout << "]" << endl;
        
        // Percolate up
        cout << "   Percolate UP from index " << (heapSize - 1) << ":" << endl;
        if(heapSize == 1){
            cout << "      First element, no percolation needed." << endl;
        } else {
            MaxHeapPercolateUp(heap, heapSize - 1);
        }
        
        cout << "   Heap after percolation:" << endl;
        PrintArray(heap, heapSize, "Heap ");
        cout << "   Heap tree:" << endl;
        PrintHeapTree(heap, heapSize);
        cout << endl;
    }
    
    cout << "============================================" << endl;
    cout << "       MAX HEAP CONSTRUCTION COMPLETE       " << endl;
    cout << "============================================" << endl;
    PrintArray(heap, heapSize, "Heap ");
    cout << "   Heap tree:" << endl;
    PrintHeapTree(heap, heapSize);
    cout << endl;
    
    // ==================== PHASE 2: EXTRACT INTO SORTED ====================
    cout << "============================================" << endl;
    cout << "  PHASE 2: EXTRACT ALL ELEMENTS (SORTED)   " << endl;
    cout << "============================================" << endl;
    cout << "Remove max (root), place in sorted array (back to front)," << endl;
    cout << "move last heap element to root, then percolate DOWN." << endl;
    cout << endl;
    
    // We'll fill sorted array from the END (largest first)
    int sortedIndex = size - 1;
    
    while(heapSize > 0){
        int extractNum = size - heapSize + 1;
        cout << "--- Extract Step " << extractNum << ": Remove max from heap ---" << endl;
        
        // Get max (root)
        int maxVal = heap[0];
        cout << "   Max value (root): " << maxVal << endl;
        
        // Place in sorted array (from back)
        sorted[sortedIndex] = maxVal;
        sortedIndex--;
        sortedSize++;
        
        cout << "   Placed " << maxVal << " at sorted[" << (sortedIndex + 1) << "]" << endl;
        
        // Move last element to root
        heapSize--;
        if(heapSize > 0){
            heap[0] = heap[heapSize];
            cout << "   Moved last element (" << heap[0] << ") to root" << endl;
            
            cout << "   Heap array after removal: [";
            for(int j = 0; j < heapSize; j++){
                cout << heap[j];
                if(j < heapSize - 1) cout << ", ";
            }
            cout << "]" << endl;
            
            // Percolate down
            cout << "   Percolate DOWN from index 0:" << endl;
            MaxHeapPercolateDown(heap, 0, heapSize);
            
            cout << "   Heap after percolation:" << endl;
            PrintArray(heap, heapSize, "Heap ");
            cout << "   Heap tree:" << endl;
            PrintHeapTree(heap, heapSize);
        } else {
            cout << "   Heap is now empty." << endl;
        }
        
        cout << "   Sorted array: [";
        for(int j = 0; j < size; j++){
            if(j <= sortedIndex){
                cout << "_ ";
            } else {
                cout << sorted[j];
                if(j < size - 1) cout << ", ";
            }
        }
        cout << "]" << endl;
        cout << endl;
    }
    
    cout << "============================================" << endl;
    cout << "           SORTING COMPLETE                 " << endl;
    cout << "============================================" << endl;
    cout << endl;
    
    cout << "Final sorted array: [";
    for(int i = 0; i < size; i++){
        cout << sorted[i];
        if(i < size - 1) cout << ", ";
    }
    cout << "]" << endl;
    
    // Copy back to input if needed
    for(int i = 0; i < size; i++){
        input[i] = sorted[i];
    }
    
    delete[] heap;
    delete[] sorted;
}

int main(){
    int arr[] = {8,1,2,6,5,3,4,7,10,9};
    int size = 10;
    
    HeapSort(arr, size);
    
    return 0;
}