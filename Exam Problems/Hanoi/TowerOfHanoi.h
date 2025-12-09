#include "Tower.h"

class TOH{
    private:
        Tower* Start;
        Tower* A1;
        Tower* A2;
        Tower* A3;
        Tower* Dest;
        int numDisks;
        int numMoves=0;
    public:
        TOH(int n);
        void Move(Tower* t1);
        void Hanoi(int numD, Tower* start);
        void Solve();
        bool IsValidMove(Tower* t);
};