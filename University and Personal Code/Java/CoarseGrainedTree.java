package data_structures.implementation;

import java.util.ArrayList;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

import data_structures.Sorted;

// Replace the exceptions in the code below with the actual code for the assignment.
// Leave the public API the same so it can be tested with the existing evaluation framework.
// Document your design/implementation choices in comments.

public class CoarseGrainedTree<T extends Comparable<T>> implements Sorted<T> {

    public class Node {
        T data;
        Node left;
        Node right;
        Node parent;

        Node(T d){
            data = d;
            left = null;
            right = null;
            parent = null;
        }
    }

    Lock lock = new ReentrantLock();
    Node root = null;

    public void insertNode(Node nodeToAdd, Node currNode){
        if(nodeToAdd.data.compareTo(currNode.data) <= 0 && currNode.left == null){
            currNode.left = nodeToAdd;
            nodeToAdd.parent = currNode;
            return;
        }else if(nodeToAdd.data.compareTo(currNode.data) <= 0 && currNode.left != null){
            insertNode(nodeToAdd, currNode.left);
            return;
        }else if(nodeToAdd.data.compareTo(currNode.data) > 0 && currNode.right == null){
            currNode.right = nodeToAdd;
            nodeToAdd.parent = currNode;
            return;
        }else if(nodeToAdd.data.compareTo(currNode.data) > 0 && currNode.right != null){
            insertNode(nodeToAdd, currNode.right);
            return;
        }
        return;
    }

    public void add(T t) {
        lock.lock();
        try {
            Node nodeToAdd = new Node(t);
            if(root == null){
                root = nodeToAdd;
            }else{
                insertNode(nodeToAdd, root);
            }

        } finally {
            lock.unlock();
        }
    }

    public Node searchTree(T t, Node currNode){
        if(currNode == null || currNode.data == null){
            return null;
        }
        if(t.compareTo(currNode.data) == 0){
            return currNode;
        }else if(t.compareTo(currNode.data) < 0){
            return searchTree(t, currNode.left);
        }else if(t.compareTo(currNode.data) > 0){
            return searchTree(t, currNode.right);
        }
        return null;
    }

    public Node findSuccessor(Node currNode){
        while(currNode.left != null){
            currNode = currNode.left;
        }
        return currNode;
    }

    public void shiftTree(Node nodeToRemove, Node nodeReplacement){
        if(nodeToRemove.parent == null){
            root = nodeReplacement;
        }else if(nodeToRemove == nodeToRemove.parent.left){
            nodeToRemove.parent.left = nodeReplacement;
        }else{
            nodeToRemove.parent.right = nodeReplacement;
        }
        if(nodeReplacement != null){
            nodeReplacement.parent = nodeToRemove.parent;
        }
    }

    public void remove(T t) {
        lock.lock();
        try {
            Node nodeToRemove = searchTree(t, root);

            if(nodeToRemove == null){ return; }
            if(nodeToRemove.left == null){
                shiftTree(nodeToRemove, nodeToRemove.right);
            }else if(nodeToRemove.right == null){
                shiftTree(nodeToRemove, nodeToRemove.left);
            }else{
                Node successor = findSuccessor(nodeToRemove.right);
                if(successor.parent != nodeToRemove){
                    shiftTree(successor, successor.right);
                    successor.right = nodeToRemove.right;
                    successor.right.parent = successor;
                }
                shiftTree(nodeToRemove, successor);
                successor.left = nodeToRemove.left;
                successor.left.parent = successor;
            }

        } finally {
            lock.unlock();
        }
    }

    public ArrayList<T> createArray(ArrayList<T> theArray, Node currNode){
        if(currNode.left != null){ createArray(theArray, currNode.left); }
        theArray.add(currNode.data);
        if(currNode.right != null){ createArray(theArray, currNode.right); }

        return theArray;
    }

    public ArrayList<T> toArrayList() {

        ArrayList<T> temp = new ArrayList<T>();
        if(root != null){
            temp = createArray(temp, root);
        }
        return temp;
    }
}
