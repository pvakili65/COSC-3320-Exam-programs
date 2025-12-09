#include "TowerOfHanoi.h"

TOH::TOH(int n){
    numDisks=n;
    Start=new Tower();
    A1=new Tower();
    A2=new Tower();
    A3=new Tower();
    Dest=new Tower();
    Start->SetName("Start");
    A1->SetName("A1");
    A2->SetName("A2");
    A3->SetName("A3");
    Dest->SetName("Dest");
    Start->SetNextTower(A1);
    A1->SetNextTower(Dest);
    Dest->SetNextTower(A2);
    A2->SetNextTower(A3);
    A3->SetNextTower(A1);
    for(int i=numDisks;i>0;i--){
        Disk* newDisk=new Disk(i);
        Start->Stack(newDisk);
    }
}

bool TOH::IsValidMove(Tower* t){
    if(t->IsEmpty()){
        return false;
    }
    else if(t->GetNextTower()->IsEmpty()){
        return true;
    }
    else if(t->GetTop()->num<t->GetNextTower()->GetTop()->num){
        return true;
    }
    else{
        return false;
    }

}

void TOH::Move(Tower* t1){
    if(!IsValidMove(t1)){
        return;
    }
    t1->GetNextTower()->Stack(t1->Remove());
    cout<<t1->GetName()<<"-->"<<t1->GetNextTower()->GetName()<<", ";
    numMoves++;
}

void TOH::Hanoi(int numD, Tower* start){
    if(numD<=0){
        return;
    }
    else if(numD==1){
        Move(start);
        Move(start->GetNextTower());
        return;
    }
    else{
        Hanoi(numD-1, start);
        Move(start);
        Hanoi(numD-1, start->GetNextTower()->GetNextTower());
        Move(start->GetNextTower());
        Hanoi(numD-1, start->GetNextTower()->GetNextTower()->GetNextTower()->GetNextTower());
        return;
    }
}

void TOH::Solve(){
    Hanoi(numDisks,Start);
    cout<<endl;
    cout<<"Number of moves: "<<numMoves<<endl;
}