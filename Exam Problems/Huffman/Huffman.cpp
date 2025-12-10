#include <iostream>
using namespace std;

struct Character{
    
    char character;
    int frequency=1;
    string code="";
    Character* parent;
    Character* left;
    Character* right;
    
    Character(char c){
        character=c;
        parent=left=right=nullptr;
    }
    Character(){
        parent=left=right=nullptr;
    }
    int GetFreq(){
        return frequency;
    }
    char GetChar(){
        return character;
    }
    void IncFreq(){
        frequency++;
    }
    void SetFreq(int num){
        frequency=num;
    }
    void AppendToCode(char c){
        code+=c;
    }
    string GetCode(){
        return code;
    }
};

class Table{
    private:
        int size;
        int capacity;
        Character** table;
    public:
        Table(){
            size=0;
            capacity=256;
            table=new Character*[capacity];
            for(int i=0;i<capacity;i++){
                table[i]=nullptr;
            }
        }
        ~Table(){
            for(int i=0;i<capacity;i++){
                if(table[i]!=nullptr){
                    delete table[i];
                }
            }
            delete[] table;
        }
        int GetSize(){
            return size;
        }
        int GetCapacity(){
            return capacity;
        }
        Character* GetAt(int index){
            return table[index];
        }
        void Insert(Character c){
            int key=(unsigned char)c.GetChar();
            if(table[key]==nullptr){
                table[key]=new Character(c);
                size++;
            }
            else{
                table[key]->IncFreq();
            }
        }
        void BuildTable(string msg){
            for(int i=0;i<msg.length();i++){
                Character c(msg[i]);
                Insert(c);
            }
        }
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
        Character** pqueue;
    public:
        PQueue(int cap){
            size=0;
            capacity=cap*2;
            pqueue=new Character* [capacity];
            for(int i=0;i<capacity;i++){
                pqueue[i]=nullptr;
            }
        }
        ~PQueue(){
            delete[] pqueue;
        }
        int GetSize(){
            return size;
        }
        void MinHeapPercolateUp(){
            int index=size-1;
            while(index>0){
                int parentIndex=(index-1)/2;
                if(pqueue[index]->GetFreq()>=pqueue[parentIndex]->GetFreq()){
                    return;
                }
                else{
                    Character* temp=pqueue[index];
                    pqueue[index]=pqueue[parentIndex];
                    pqueue[parentIndex]=temp;
                    index=parentIndex;
                }
            }
        }
        void MinHeapPercolateDown(){
            int index=0;
            int childIndex=index*2+1;
            int value=pqueue[index]->GetFreq();
            while(childIndex<size){
                int minValue=value;
                int minIndex=-1;
                for(int i=0; i<2 && i+childIndex<size; i++){
                    if(pqueue[i+childIndex]->GetFreq()<minValue){
                        minValue=pqueue[i+childIndex]->GetFreq();
                        minIndex=i+childIndex;
                    }
                }
                if(minValue==value){
                    return;
                }
                else{
                    Character* temp=pqueue[index];
                    pqueue[index]=pqueue[minIndex];
                    pqueue[minIndex]=temp;
                    index=minIndex;
                    childIndex=2*index+1;
                }
            }
        }
        void Enqueue(Character* c){
            if(size==0){
                pqueue[0]=c;
                size++;
                return;
            }
            else{
                pqueue[size]=c;
                size++;
                MinHeapPercolateUp();
                return;
            }
        }
        Character* Dequeue(){
            if(size==0){
                throw QueueException("Queue is empty");
            }
            else if(size==1){
                Character* c=pqueue[0];
                pqueue[0]=nullptr;
                size--;
                return c;
            }
            else if(size==2){
                Character* c=pqueue[0];
                pqueue[0]=pqueue[1];
                pqueue[1]=nullptr;
                size--;
                return c;
            }
            else{
                Character* c=pqueue[0];
                pqueue[0]=pqueue[size-1];
                pqueue[size-1]=nullptr;
                size--;
                MinHeapPercolateDown();
                return c;
            }
        }
};

class HuffmanTree{
    private:
        Character* root;
        Table freqTable;
        string message;
    public:
        HuffmanTree(string msg){
            root=nullptr;
            message=msg;
            freqTable.BuildTable(message);
        }
        ~HuffmanTree(){
            RemovePostOrder(root);
        }
        void RemovePostOrder(Character* character){
            if(character==nullptr){
                return;
            }
            RemovePostOrder(character->left);
            RemovePostOrder(character->right);
            if(character->left!=nullptr || character->right!=nullptr){
                delete character;
            }
        }
        Character* HuffmanBuildTree(){
            int priorityQueueSize=freqTable.GetSize();
            PQueue characters(priorityQueueSize);
            for(int i=0;i<freqTable.GetCapacity();i++){
                if(freqTable.GetAt(i)!=nullptr){
                    characters.Enqueue(freqTable.GetAt(i));
                }
            }
            if(characters.GetSize()==1){
                root=new Character();
                Character* left=characters.Dequeue();
                root->SetFreq(left->GetFreq());
                root->left=left;
                left->parent=root;
                return root;
            }
            while(characters.GetSize()>1){
                Character* left=characters.Dequeue();
                Character* right=characters.Dequeue();
                Character* parent=new Character();
                parent->SetFreq(left->GetFreq()+right->GetFreq());
                parent->left=left;
                parent->right=right;
                left->parent=parent;
                right->parent=parent;
                characters.Enqueue(parent);
            }
            root=characters.Dequeue();
            return root;
        }
        void HuffmanSetCodes(Character* character, string prefix){
            if(character==nullptr){
                return;
            }
            else if(character->left==nullptr && character->right==nullptr){
                int index=(unsigned char)character->character;
                freqTable.GetAt(index)->code=prefix;
            }
            else{
                HuffmanSetCodes(character->left, prefix+'0');
                HuffmanSetCodes(character->right, prefix+'1');
            }
        }
        string HuffmanGetCode(){
            HuffmanSetCodes(HuffmanBuildTree(), "");
            string result="";
            for(int i=0;i<message.length();i++){
                int index=(unsigned char)message[i];
                result+=freqTable.GetAt(index)->code;
                result+=" ";
            }
            return result;
        }

};

int main(){
    HuffmanTree tree1("abbcccddddeeeeeffffffgggggggghhhhhhhhhhh");
    cout<<tree1.HuffmanGetCode();

    return 0;
}