#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <vector>
#include <fstream>
#include <climits>
#include <iomanip>
using namespace std;

const int INF = INT_MAX;

struct Vertex{
    int distance;
    int vertexNum;
    Vertex* predV;
    vector<Vertex*> adjVertices;
};

class QueueException : public exception{
    private:
        string message;
    public:
        QueueException(const string& msg){
            message=msg;   
        }
        const char* what() const noexcept override{
            return message.c_str();   
        }
};

class PQueue{
    private:
        int size;
        int capacity;
        Vertex** pQueue;
    public:
        PQueue(){
            size=0;
            capacity=2;
            pQueue=new Vertex*[capacity];
            for(int i=0;i<capacity;i++){
                pQueue[i]=nullptr;
            }
        }
        ~PQueue(){
            delete[] pQueue;
        }
        int GetSize(){
            return size;
        }
        bool IsEmpty(){
            if(size==0){
                return true;
            }
            return false;
        }
        void MinHeapPercolateUp(){
            int index=size-1;
            while(index>0){
                int parentIndex=(index-1)/2;
                if(pQueue[index]->distance>=pQueue[parentIndex]->distance){
                    return;
                }
                else{
                    Vertex* temp=pQueue[index];
                    pQueue[index]=pQueue[parentIndex];
                    pQueue[parentIndex]=temp;
                    index=parentIndex;
                }
            }
        }
        void MinHeapPercolateDown(){
            int index=0;
            int childIndex=index*2+1;
            int value=pQueue[index]->distance;
            while(childIndex<size){
                int minValue=value;
                int minIndex=-1;
                for(int i=0; i<2 && i+childIndex<size; i++){
                    if(pQueue[i+childIndex]->distance<minValue){
                        minValue=pQueue[i+childIndex]->distance;
                        minIndex=i+childIndex;
                    }
                }
                if(minValue==value){
                    return;
                }
                else{
                    Vertex* temp=pQueue[index];
                    pQueue[index]=pQueue[minIndex];
                    pQueue[minIndex]=temp;
                    index=minIndex;
                    childIndex=index*2+1;
                }
            }
        }
        void Enqueue(Vertex* v){
            if(size==0){
                pQueue[0]=v;
                size++;
                return;
            }
            else{
                if(size<capacity){
                    pQueue[size]=v;
                    size++;
                    MinHeapPercolateUp();
                    return;
                }
                else{
                    capacity=capacity*2;
                    Vertex** tempArr=new Vertex*[capacity];
                    for(int i=0;i<size;i++){
                        tempArr[i]=pQueue[i];
                    }
                    delete[] pQueue;
                    pQueue=tempArr;
                    pQueue[size]=v;
                    size++;
                    MinHeapPercolateUp();
                    return;
                }
            }
        }
        Vertex* Dequeue(){
            if(size==0){
                throw QueueException("Queue Is Empty");
            }
            else if(size==1){
                Vertex* v=pQueue[0];
                pQueue[0]=nullptr;
                size--;
                return v;
            }
            else if(size==2){
                Vertex* v=pQueue[0];
                pQueue[0]=pQueue[1];
                pQueue[1]=nullptr;
                size--;
                return v;
            }
            else{
                Vertex* v=pQueue[0];
                pQueue[0]=pQueue[size-1];
                pQueue[size-1]=nullptr;
                size--;
                MinHeapPercolateDown();
                return v;
            }
        }
        
        // Print contents of priority queue
        void PrintQueue(){
            cout << "   Priority Queue: [";
            for(int i = 0; i < size; i++){
                cout << "V" << pQueue[i]->vertexNum << "(d=" << pQueue[i]->distance << ")";
                if(i < size - 1) cout << ", ";
            }
            cout << "]" << endl;
        }
};

class Graph{
    private:
        PQueue unvisited;
        vector<Vertex*> allVertices;
        
        // Helper to print distance table
        void PrintDistanceTable(vector<bool>& visited, int numVertices){
            cout << "   +--------+----------+----------+" << endl;
            cout << "   | Vertex | Distance |  Visited |" << endl;
            cout << "   +--------+----------+----------+" << endl;
            for(int i = 0; i < numVertices; i++){
                cout << "   |   " << i << "    |";
                if(allVertices[i]->distance == INF){
                    cout << "   INF    |";
                } else {
                    cout << setw(6) << allVertices[i]->distance << "    |";
                }
                cout << (visited[i] ? "   Yes    |" : "   No     |") << endl;
            }
            cout << "   +--------+----------+----------+" << endl;
        }
        
    public:
        void DSP(int start, string filename){
            ifstream input(filename);
            if(!input.is_open()){
                cout << "Error: Could not open file " << filename << endl;
                return;
            }
            
            int size;
            int a;
            int b;
            int weight;
            input>>size;
            vector<vector<int>> matrix;
            matrix.resize(size,vector<int>(size));
            
            while(input>>a){
                input>>b;
                input>>weight;
                matrix[a][b]=weight;
            }
            input.close();
            
            // Print adjacency matrix
            cout << "\n=== ADJACENCY MATRIX ===" << endl;
            cout << "    ";
            for(int i = 0; i < size; i++){
                cout << setw(3) << i << " ";
            }
            cout << endl;
            cout << "   +";
            for(int i = 0; i < size; i++){
                cout << "----";
            }
            cout << endl;
            
            for(int i=0;i<size;i++){
                cout << " " << i << " |";
                for(int j=0;j<size;j++){
                    if(matrix[i][j]!=0){
                        cout << setw(3) << matrix[i][j] << " ";
                    } 
                    else{
                        cout << " -- ";   
                    }   
                } 
                cout<<endl;
            }
            
            // Initialize visited array
            vector<bool> visited(size, false);
            
            // Initialize all vertices
            cout << "\n=== INITIALIZATION ===" << endl;
            cout << "Starting vertex: " << start << endl;
            cout << "Setting distance[" << start << "] = 0, all others = INF" << endl;
            
            for(int i=0;i<matrix.size();i++){
                Vertex* currentV=new Vertex();
                currentV->predV=nullptr;
                currentV->vertexNum=i;
                if(i==start){
                    currentV->distance=0;   
                    unvisited.Enqueue(currentV);
                }
                else{
                    currentV->distance=INF;   
                }
                allVertices.push_back(currentV);
            }
            
            PrintDistanceTable(visited, size);
            unvisited.PrintQueue();
            
            // Dijkstra's algorithm 
            cout << "\n=== DIJKSTRA'S ALGORITHM ===" << endl;
            int step = 1;
            
            while(!unvisited.IsEmpty()){
                Vertex* currentV=unvisited.Dequeue();

                // Skip if already visited 
                if(visited[currentV->vertexNum]){
                    cout << "\n--- Step " << step++ << ": Dequeued V" << currentV->vertexNum << " (already visited, skipping) ---" << endl;
                    continue;
                }
                
                cout << "\n--- Step " << step++ << ": Process V" << currentV->vertexNum << " (distance = " << currentV->distance << ") ---" << endl;
                visited[currentV->vertexNum]=true;
                cout << "   Marked V" << currentV->vertexNum << " as visited" << endl;

                // Find neighbors
                cout << "   Checking neighbors of V" << currentV->vertexNum << ": ";
                bool hasNeighbors = false;
                for(int i = 0; i < matrix.size(); i++){
                    if(matrix[currentV->vertexNum][i] != 0){
                        if(hasNeighbors) cout << ", ";
                        cout << "V" << i << "(weight=" << matrix[currentV->vertexNum][i] << ")";
                        hasNeighbors = true;
                    }
                }
                if(!hasNeighbors) cout << "none";
                cout << endl;

                // Process all adjacent vertices
                for(int i=0;i<matrix.size();i++){
                    if(matrix[currentV->vertexNum][i]!=0 && !visited[i]){
                        Vertex* adjV=allVertices[i];
                        int edgeWeight=matrix[currentV->vertexNum][i];
                        
                        // Only update if current vertex has a valid distance
                        if(currentV->distance != INF){
                            int alternatePathDistance=currentV->distance+edgeWeight;
                            
                            cout << "   -> V" << i << ": ";
                            cout << "current distance = ";
                            if(adjV->distance == INF) cout << "INF";
                            else cout << adjV->distance;
                            cout << ", new distance via V" << currentV->vertexNum << " = " << currentV->distance << " + " << edgeWeight << " = " << alternatePathDistance;
                            
                            if(alternatePathDistance<adjV->distance){
                                cout << " (SHORTER! Updating)" << endl;
                                adjV->distance=alternatePathDistance;
                                adjV->predV=currentV; 
                                unvisited.Enqueue(adjV);
                            }
                            else{
                                cout << " (not shorter, skip)" << endl;
                            }
                        }
                    }   
                }
                
                cout << endl;
                PrintDistanceTable(visited, size);
                unvisited.PrintQueue();
            }
            
            // Print results
            cout << "\n=== FINAL RESULTS ===" << endl;
            cout << "+--------+----------+----------------------+" << endl;
            cout << "| Vertex | Distance | Path                 |" << endl;
            cout << "+--------+----------+----------------------+" << endl;
            
            for(int i=0;i<matrix.size();i++){
                Vertex* current=allVertices[i];
                
                cout << "|   " << current->vertexNum << "    |";
                
                // Handle unreachable vertices
                if(current->distance==INF){
                    cout << "   INF    | No path              |" << endl;
                    continue;
                }

                // Print distance
                cout << "    " << current->distance << "     | ";
                
                // Reconstruct path 
                vector<Vertex*> path;
                Vertex* temp = current;
                
                // Build path from destination to source
                while(temp != nullptr){
                    path.push_back(temp);
                    if(temp->vertexNum == start){
                        break;   
                    }
                    temp = temp->predV;
                }
                
                // Build path string
                string pathStr = "";
                for(int j=path.size()-1; j>=0; j--){
                    pathStr += to_string(path[j]->vertexNum);
                    if(j>0){
                        pathStr += "->";   
                    }
                }
                cout << left << setw(20) << pathStr << " |" << endl;
            }
            cout << "+--------+----------+----------------------+" << endl;
            
            // Cleanup memory
            for(Vertex* v : allVertices){
                delete v;
            }
            allVertices.clear();
        }
};

#endif