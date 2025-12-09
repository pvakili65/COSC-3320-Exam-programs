#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <queue>
using namespace std;

struct Node{
    int value;
    int height;
    Node* left;
    Node* right;
    Node* parent;
    Node(int v){
        value=v;
        height=0;
        left=right=parent=nullptr;
    }
};

class AVLTree{
    private:
        Node* root;
        
        void PrintTreeHelper(Node* node, string prefix, bool isLeft){
            if(node == nullptr){
                return;
            }
            
            cout << prefix;
            cout << (isLeft ? "+--L " : "+--R ");
            cout << node->value << " (h=" << node->height << ", bal=" << GetBalance(node) << ")" << endl;
            
            string newPrefix = prefix + (isLeft ? "|    " : "     ");
            PrintTreeHelper(node->left, newPrefix, true);
            PrintTreeHelper(node->right, newPrefix, false);
        }
        
    public:
        AVLTree(){
            root=nullptr;
        }
        ~AVLTree(){
            DeletePostOrder(root);
        }
        void DeletePostOrder(Node* node){
            if(node==nullptr){
                return;
            }
            DeletePostOrder(node->left);
            DeletePostOrder(node->right);
            delete node;
        }
        void UpdateHeight(Node* node){
            int leftHeight=-1;
            int rightHeight=-1;
            if(node->left!=nullptr){
                leftHeight=node->left->height;
            }
            if(node->right!=nullptr){
                rightHeight=node->right->height;
            }
            // Height = 1 + max of children's heights
            // A leaf has children heights of -1, so height = 1 + (-1) = 0
            if(leftHeight>rightHeight){
                node->height=leftHeight + 1;
            }
            else{
                node->height=rightHeight + 1;
            }
        }
        bool SetChild(Node* parent, string whichChild, Node* child){
            if(whichChild!="left" && whichChild!="right"){
                return false;
            }
            if(whichChild=="left"){
                parent->left=child;
            }
            else{
                parent->right=child;
            }
            if(child!=nullptr){
                child->parent=parent;
                UpdateHeight(child);  // Update child's height too
            }
            UpdateHeight(parent);
            return true;
        }
        bool ReplaceChild(Node* parent, Node* currentChild, Node* newChild){
            if(parent->left==currentChild){
                return SetChild(parent,"left",newChild);
            }
            else if(parent->right==currentChild){
                return SetChild(parent,"right",newChild);
            }
            else{
                return false;
            }
        }
        int GetBalance(Node* node){
            int leftHeight=-1;
            int rightHeight=-1;
            if(node->left!=nullptr){
                leftHeight=node->left->height;
            }
            if(node->right!=nullptr){
                rightHeight=node->right->height;
            }
            return leftHeight-rightHeight;
        }
        void RotateRight(Node* node){
            Node* leftRightChild=node->left->right;
            Node* newRoot = node->left;
            if(node->parent!=nullptr){
                ReplaceChild(node->parent,node,newRoot);
            }
            else{
                root=newRoot;
                root->parent=nullptr;
            }
            SetChild(newRoot,"right",node);
            SetChild(node,"left",leftRightChild);
            // Update heights bottom-up
            if(leftRightChild != nullptr) UpdateHeight(leftRightChild);
            UpdateHeight(node);
            UpdateHeight(newRoot);
        }
        void RotateLeft(Node* node){
            Node* rightLeftChild=node->right->left;
            Node* newRoot = node->right;
            if(node->parent!=nullptr){
                ReplaceChild(node->parent,node,newRoot);
            }
            else{
                root=newRoot;
                root->parent=nullptr;
            }
            SetChild(newRoot,"left",node);
            SetChild(node,"right",rightLeftChild);
            // Update heights bottom-up
            if(rightLeftChild != nullptr) UpdateHeight(rightLeftChild);
            UpdateHeight(node);
            UpdateHeight(newRoot);
        }
        void Rebalance(Node* node){
            UpdateHeight(node);
            
            if(GetBalance(node)==-2){
                if(GetBalance(node->right)==1){
                    cout << ">> Right-Left rotation at node " << node->value << endl;
                    RotateRight(node->right);
                    RotateLeft(node);
                }
                else{
                    cout << ">> Left rotation at node " << node->value << endl;
                    RotateLeft(node);
                }
            }
            else if(GetBalance(node)==2){
                if(GetBalance(node->left)==-1){
                    cout << ">> Left-Right rotation at node " << node->value << endl;
                    RotateLeft(node->left);
                    RotateRight(node);
                }
                else{
                    cout << ">> Right rotation at node " << node->value << endl;
                    RotateRight(node);
                }
            }
        }
        void Insert(int num){
            cout << "\n=== Inserting " << num << " ===" << endl;
            if(root==nullptr){
                Node* newNode=new Node(num);
                root=newNode;
                PrintTree();
                return;
            }
            else{
                Node* current=root;
                while(current!=nullptr){
                    if(current->value==num){
                        cout << "Value already exists!" << endl;
                        PrintTree();
                        return;
                    }
                    else if(num<current->value){
                        if(current->left==nullptr){
                            Node* newNode=new Node(num);
                            current->left=newNode;
                            newNode->parent=current;
                            break;
                        }
                        else{
                            current=current->left;
                        }
                    }
                    else{
                        if(current->right==nullptr){
                            Node* newNode=new Node(num);
                            current->right=newNode;
                            newNode->parent=current;
                            break;
                        }
                        else{
                            current=current->right;
                        }
                    }
                }
                while(current!=nullptr){
                    Rebalance(current);
                    current=current->parent;
                }
            }
            PrintTree();
        }
        void Remove(int num){
            cout << "\n=== Removing " << num << " ===" << endl;
            if(root==nullptr){
                cout << "Tree is empty!" << endl;
                return;
            }
            Node* current=root;
            while(current!=nullptr){
                if(current->value==num){
                    break;
                }
                else if(num<current->value){
                    current=current->left;
                }
                else{
                    current=current->right;
                }
            }
            if(current==nullptr){
                cout << "Value not found!" << endl;
                PrintTree();
                return;
            }
            else{
                Node* parent=current->parent;
                if(current->left==nullptr && current->right==nullptr){
                    if(current==root){
                        root=nullptr;
                        delete current;
                        cout << "Tree is now empty." << endl;
                        return;
                    }
                    else{
                        if(current->parent->left==current){
                            current->parent->left=nullptr;
                            delete current;
                            while(parent!=nullptr){
                                Rebalance(parent);
                                parent=parent->parent;
                            }
                            PrintTree();
                            return;
                        }
                        else{
                            current->parent->right=nullptr;
                            delete current;
                            while(parent!=nullptr){
                                Rebalance(parent);
                                parent=parent->parent;
                            }
                        }
                    }
                }
                else if(current->left!=nullptr && current->right==nullptr){
                    if(current==root){
                        root=current->left;
                        current->left->parent=nullptr;
                        delete current;
                        PrintTree();
                        return;
                    }
                    else{
                        if(current->parent->left==current){
                            current->parent->left=current->left;
                            delete current;
                            while(parent!=nullptr){
                                Rebalance(parent);
                                parent=parent->parent;
                            }
                            PrintTree();
                            return;
                        }
                        else{
                            current->parent->right=current->left;
                            delete current;
                            while(parent!=nullptr){
                                Rebalance(parent);
                                parent=parent->parent;
                            }
                            PrintTree();
                            return;
                        }
                    }
                }
                else if(current->left==nullptr && current->right!=nullptr){
                    if(current==root){
                        root=current->right;
                        current->right->parent=nullptr;
                        delete current;
                        PrintTree();
                        return;
                    }
                    else{
                        if(current->parent->left==current){
                            current->parent->left=current->right;
                            current->right->parent=current->parent;
                            delete current;
                            while(parent!=nullptr){
                                Rebalance(parent);
                                parent=parent->parent;
                            }
                            PrintTree();
                            return;
                        }
                        else{
                            current->parent->right=current->right;
                            current->right->parent=current->parent;
                            delete current;
                            while(parent!=nullptr){
                                Rebalance(parent);
                                parent=parent->parent;
                            }
                            PrintTree();
                            return;
                        }
                    }
                }
                else{
                    Node* current2=current->right;
                    while(current2->left!=nullptr){
                        current2=current2->left;
                    }
                    if(current2->parent==current){
                        if(current2->right==nullptr){
                            current->value=current2->value;
                            current->right=nullptr;
                            parent=current2->parent;
                            delete current2;
                            while(parent!=nullptr){
                                Rebalance(parent);
                                parent=parent->parent;
                            }
                            PrintTree();
                            return;
                        }
                        else{
                            current->value=current2->value;
                            current->right=current2->right;
                            current2->right->parent=current2->parent;
                            parent=current2->parent;
                            delete current2;
                            while(parent!=nullptr){
                                Rebalance(parent);
                                parent=parent->parent;
                            }
                            PrintTree();
                            return;
                        }
                    }
                    else{
                        if(current2->right==nullptr){
                            current->value=current2->value;
                            parent=current2->parent;
                            current2->parent->left=nullptr;
                            delete current2;
                            while(parent!=nullptr){
                                Rebalance(parent);
                                parent=parent->parent;
                            }
                            PrintTree();
                            return;
                        }
                        else{
                            current->value=current2->value;
                            parent=current2->parent;
                            current2->parent->left=current2->right;
                            current2->right->parent=current2->parent;
                            delete current2;
                            while(parent!=nullptr){
                                Rebalance(parent);
                                parent=parent->parent;
                            }
                            PrintTree();
                            return;
                        }

                    }
                }
            }
            PrintTree();
        }
        void PrintInOrder(Node* node){
            if(node==nullptr){
                return;
            }
            PrintInOrder(node->left);
            cout<<node->value<<" ";
            PrintInOrder(node->right);
        }
        
        int GetHeight(Node* node){
            if(node == nullptr) return -1;
            return node->height;
        }
        
        int GetTreeDepth(Node* node){
            if(node == nullptr) return 0;
            int left = GetTreeDepth(node->left);
            int right = GetTreeDepth(node->right);
            return 1 + (left > right ? left : right);
        }
        
        void PrintTree(){
            if(root == nullptr){
                cout << "(empty tree)" << endl;
                return;
            }
            
            int depth = GetTreeDepth(root);
            
            // BFS to collect nodes level by level
            vector<vector<Node*>> levels;
            vector<Node*> currentLevel;
            currentLevel.push_back(root);
            
            for(int level = 0; level < depth; level++){
                levels.push_back(currentLevel);
                vector<Node*> nextLevel;
                for(Node* node : currentLevel){
                    if(node != nullptr){
                        nextLevel.push_back(node->left);
                        nextLevel.push_back(node->right);
                    } else {
                        nextLevel.push_back(nullptr);
                        nextLevel.push_back(nullptr);
                    }
                }
                currentLevel = nextLevel;
            }
            
            // Calculate spacing based on depth
            int baseWidth = 4;
            int maxWidth = baseWidth * (1 << depth); // 2^depth * baseWidth
            
            // Print each level
            for(int level = 0; level < depth; level++){
                int nodesInLevel = levels[level].size();
                int cellWidth = maxWidth / nodesInLevel;
                
                // Print nodes
                string line = "";
                for(int i = 0; i < nodesInLevel; i++){
                    int pos = cellWidth * i + cellWidth / 2 - 1;
                    while(line.length() < pos) line += " ";
                    if(levels[level][i] != nullptr){
                        string val = to_string(levels[level][i]->value);
                        line += val;
                    }
                }
                cout << line << endl;
                
                // Print branches (except for last level)
                if(level < depth - 1){
                    string branchLine = "";
                    int nextCellWidth = cellWidth / 2;
                    int branchOffset = nextCellWidth / 2;
                    if(branchOffset < 1) branchOffset = 1;
                    
                    for(int i = 0; i < nodesInLevel; i++){
                        int pos = cellWidth * i + cellWidth / 2 - 1;
                        
                        if(levels[level][i] != nullptr){
                            // Left branch
                            if(levels[level][i]->left != nullptr){
                                int leftPos = pos - branchOffset;
                                if(leftPos < 0) leftPos = 0;
                                while(branchLine.length() < leftPos) branchLine += " ";
                                branchLine += "/";
                            }
                            // Right branch
                            if(levels[level][i]->right != nullptr){
                                int rightPos = pos + branchOffset + 1;
                                while(branchLine.length() < rightPos) branchLine += " ";
                                branchLine += "\\";
                            }
                        }
                    }
                    cout << branchLine << endl;
                }
            }
            cout << endl;
        }
        
        Node* GetRoot(){
            return root;
        }
};

int main(){
    AVLTree tree;
    tree.Insert(8);
    tree.Insert(1);
    tree.Insert(2);
    tree.Insert(6);
    tree.Insert(5);
    tree.Insert(3);
    tree.Insert(4);
    tree.Insert(7);
    tree.Insert(10);
    tree.Insert(9);
    tree.Remove(1);
    tree.Remove(8);
    tree.Remove(2);
    tree.Remove(5);
    
    cout << "\n=== Final In-Order Traversal ===" << endl;
    tree.PrintInOrder(tree.GetRoot());
    cout << endl;

    return 0;
}