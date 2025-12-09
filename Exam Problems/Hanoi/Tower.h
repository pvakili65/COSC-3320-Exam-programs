#include "Disk.h"
#include <string>

struct Tower{
        string name;
        Disk* top;
        Tower* next;
        Tower();
        void Stack(Disk* disk);
        Disk* Remove();
        void SetName(string n);
        string GetName();
        Disk* GetTop();
        void SetNextTower(Tower* tower);
        Tower* GetNextTower();
        bool IsEmpty();
};