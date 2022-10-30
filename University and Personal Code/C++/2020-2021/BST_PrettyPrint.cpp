#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <ios>
#include <iomanip>


class Node{
    public:
        ~Node();
        int nodeValue;
        Node* left;
        Node* right;
        int column;
        int row;
};

Node::~Node(){
    if(left != nullptr){
        delete left;
    }
    if(right != nullptr){
        delete right;
    }
}




class BST{
    public:
        BST();
        ~BST();
        void insertKey(int newKey){ insertData(newKey, root);}
        bool hasKey(int searchKey);
        std::vector<int> inOrder();
        int getHeight();
        void prettyPrint();
    private:
        Node* root;
        std::vector<int> makeOrder(std::vector<int>& tempOrder, Node* currNode);
        Node* getNewNode(int newKey, int parentColumn);
        Node* insertData(int newKey, Node* currNode);
        bool findKey(int searchKey, Node* currNode, bool& presence);
        int findHeight(int height, Node* currNode);
        void getPrint(int height, int width);
        void setRow(std::vector<int>& orderedList, Node* currNode, int& rowNumber);
        bool whatToPrint(int column, int row, int& numberToPrint, Node* currNode, bool& presence);
};

BST::BST(){
    root = nullptr;
}

BST::~BST(){
    if(root != nullptr){
        delete root;
    }
}

bool BST::hasKey(int searchKey){
    bool presence = 0;
    return findKey(searchKey, root, presence);
}

std::vector<int> BST::inOrder(){
    std::vector<int> tempOrder;
    return makeOrder(tempOrder, root);
}

Node* BST::getNewNode(int newKey, int parentColumn){
    Node* newNode = new Node();
    newNode->nodeValue = newKey;
    newNode->left = nullptr;
    newNode->right = nullptr;
    newNode->column = parentColumn + 1;
    
    return newNode;
}

int BST::getHeight(){
    int height = 0;
    return findHeight(height, root);
}

void BST::setRow(std::vector<int>& orderedList, Node* currNode, int& rowNumber){
    if(currNode != nullptr){
        setRow(orderedList, currNode->left, rowNumber);
        rowNumber++;
        currNode->row = rowNumber;
        setRow(orderedList, currNode->right, rowNumber);
    }
}

void BST::prettyPrint(){
    std::vector<int> orderedList;
    makeOrder(orderedList, root);
    if(orderedList.size() != 0){
        int rowNumber = 0;
        setRow(orderedList, root, rowNumber);
        getPrint(getHeight(), orderedList.size());
    }
}

bool BST::whatToPrint(int currColumn, int currRow, int& numberToPrint, Node* currNode, bool& presence){
    if(currNode != nullptr){
        if(currNode->column == currColumn && currNode->row == currRow){
            numberToPrint = currNode->nodeValue;
            presence = 1;
            return presence;
        }
        whatToPrint(currColumn, currRow, numberToPrint, currNode->left, presence);
        whatToPrint(currColumn, currRow, numberToPrint, currNode->right, presence);
    }
    return presence;
}

void BST::getPrint(int height, int width){
    bool presence = 0;
    int numberToPrint;
    std::cout << std::setfill('-') << std::setw((width * 5) + 1) << "" << std::endl;
    for(int currColumn = 0; currColumn < height; currColumn++){
        std::cout << "|";
        std::cout << std::setfill(' ');
        for(int currRow = 0; currRow < width; currRow++){
            if(whatToPrint(currColumn + 1, currRow + 1, numberToPrint, root, presence)){
                std::cout << std::setw(4) << std::right << numberToPrint << '|';
                presence = 0;
            }
            else{
                std::cout << "    |";
            }
        }
        std::cout <<  std::endl << std::setfill('-') << std::setw((width * 5) + 1) << "" << std::endl;
    }
}

Node* BST::insertData(int newKey, Node* currNode){
    if(root == nullptr){
        root = getNewNode(newKey, 0);
    }
    else if(currNode == nullptr){
        currNode = getNewNode(newKey, currNode->column);
    }
    else if(newKey < currNode->nodeValue){
        if(currNode->left == nullptr){
            currNode->left = getNewNode(newKey, currNode->column);
        }
        else{
            currNode->left = insertData(newKey, currNode->left);
        }
    }
    else if(newKey > currNode->nodeValue){
        if(currNode->right == nullptr){
            currNode->right = getNewNode(newKey, currNode->column);
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
    std::cout << "Enter the numbers to be stored:";
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

    outputOrder(tree);

    tree.prettyPrint();

    return 0;
}