#include <iostream>
#include <vector>
#include <string>
#include <sstream>


class Node{
    public:
        int nodeValue;
        Node* left;
        Node* right; 
};




class BST{
    public:
        BST();
        ~BST();
        void insertKey(int newKey){ insertData(newKey, root);}
        bool hasKey(int searchKey);
        std::vector<int> inOrder();
        int getHeight();
    private:
        Node* root;
        std::vector<int> makeOrder(std::vector<int>& tempOrder, Node* currNode);
        Node* getNewNode(int newKey);
        Node* insertData(int newKey, Node* currNode);
        bool findKey(int searchKey, Node* currNode, bool& presence);
        int findHeight(int height, Node* currNode);
};

BST::BST(){
    root = nullptr;
}

BST::~BST(){
    delete root;
}

bool BST::hasKey(int searchKey){
    bool presence = 0;
    return findKey(searchKey, root, presence);
}

std::vector<int> BST::inOrder(){
    std::vector<int> tempOrder;
    return makeOrder(tempOrder, root);
}

Node* BST::getNewNode(int newKey){
    Node* newNode = new Node();
    newNode->nodeValue = newKey;
    newNode->left = nullptr;
    newNode->right = nullptr;
    
    return newNode;
}

int BST::getHeight(){
    int height = 0;
    return findHeight(height, root);
}

Node* BST::insertData(int newKey, Node* currNode){
    if(root == nullptr){
        root = getNewNode(newKey);
    }
    else if(currNode == nullptr){
        currNode = getNewNode(newKey);
    }
    else if(newKey < currNode->nodeValue){
        if(currNode->left == nullptr){
            currNode->left = getNewNode(newKey);
        }
        else{
            currNode->left = insertData(newKey, currNode->left);
        }
    }
    else if(newKey > currNode->nodeValue){
        if(currNode->right == nullptr){
            currNode->right = getNewNode(newKey);
        }
        else{
            currNode->right = insertData(newKey, currNode->right);
        }
    }
    return currNode;
}

std::vector<int> BST::makeOrder(std::vector<int>& tempOrder, Node* currNode){
    int tempData;
    if(currNode != nullptr){
        makeOrder(tempOrder, currNode->left);
        tempData = currNode->nodeValue;
        tempOrder.push_back(tempData);
        makeOrder(tempOrder, currNode->right);
    }
    return tempOrder;
}

bool BST::findKey(int searchKey, Node* currNode, bool& presence){
    if(currNode == nullptr){
        return presence;
    }
    else if(currNode->nodeValue == searchKey){
        presence = 1;
        return presence;
    }
    
    findKey(searchKey, currNode->left, presence);
    
    findKey(searchKey, currNode->right, presence);
    
    return presence;
}

int BST::findHeight(int height, Node*currNode){
    if(currNode == nullptr){
        return height;
    }
    height++;

    if(findHeight(height, currNode->left) > findHeight(height, currNode->right)){
        return findHeight(height, currNode->left);
    }
    else{
        return findHeight(height, currNode->right);
    }
}



void getInput(BST& tree){
    std::cout << "Enter the numbers to be stored (end with a letter): ";
    std::string input;
    std::getline(std::cin, input);
    std::istringstream tempInput(input);
    int insertData;
    tempInput >> insertData;
    while(!tempInput.fail()){
        tree.insertKey(insertData);
        tempInput >> insertData;
    }
    std::cout << std::endl;
}

void searchInput(BST& tree){
    std::cout << "Which number do you want to look up? ";
    int tempSearchKey;
    std::cin >> tempSearchKey;
    std::cout << std::endl << tempSearchKey << " is in the tree: ";
    if(tree.hasKey(tempSearchKey)){
        std::cout << "yes" << std::endl;
    }
    else{
        std::cout << "no" << std::endl;
    }
}

void outputOrder(BST& tree){
    std::cout << "The numbers in sorted order:";

    std::vector<int> orderedList = tree.inOrder();
    for(int i = 0; i < orderedList.size(); i++){
        std::cout << " " << orderedList.at(i);
    }
    std::cout << std::endl;
}

void outputHeight(BST& tree){
    std::cout << "Height of the tree: " << tree.getHeight() << std::endl;
}


int main(){
    BST tree;

    getInput(tree);

    searchInput(tree);

    outputOrder(tree);

    outputHeight(tree);

    return 0;
}