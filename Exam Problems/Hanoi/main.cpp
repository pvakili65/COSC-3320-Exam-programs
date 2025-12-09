#include "TowerOfHanoi.h"

int main(){
    int numDisks;
    cout<<"Enter number of Disks: ";
    cin>>numDisks;
    TOH toh(numDisks);
    toh.Solve();

    return 0;
}