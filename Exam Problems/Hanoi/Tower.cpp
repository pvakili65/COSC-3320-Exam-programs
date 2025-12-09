#include "Tower.h"
class TowerException : public exception{
    private:
        string message;
    public:
        TowerException(string msg){
            message=msg;
        }
        const char* what() const noexcept override{
            return message.c_str();
        }
};

Tower::Tower(){
    top=nullptr;
}

void Tower::Stack(Disk* disk){
    if(top==nullptr){
        top=disk;
    }
    else{
        if(disk->num < top->num){
            disk->next=top;
            top=disk;
        }
        else{
            cout<<"Tried stacking a larger disk on a smaller disk"<<endl;
            return;
        }
    }
}

Disk* Tower::Remove(){
    if(top==nullptr){
        throw TowerException("Tried to remove from empty tower");
    }
    else{
        Disk* current=top;
        top=top->next;
        current->next=nullptr;
        return current;
    }
}

void Tower::SetName(string n){
    name=n;
}

string Tower::GetName(){
    return name;
}

Disk* Tower::GetTop(){
    return top;
}

void Tower::SetNextTower(Tower* tower){
    next=tower;
}

Tower* Tower::GetNextTower(){
    return next;
}

bool Tower::IsEmpty(){
    if(top==nullptr){
        return true;
    }
    return false;
}