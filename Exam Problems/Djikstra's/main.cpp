#include <iostream>
#include <fstream>
#include <sstream>
#include "Graph.h"
#include <vector>
#include <string>
using namespace std;

int main() {
   string file;
   cout<<"Enter file name: ";
   cin>>file;
   int start;
   cout<<"Enter start vertex: ";
   cin>>start;
   Graph graph;
   graph.DSP(start,file);
}