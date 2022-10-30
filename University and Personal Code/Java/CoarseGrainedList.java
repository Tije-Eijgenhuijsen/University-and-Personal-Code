package data_structures.implementation;

import java.util.ArrayList;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

import data_structures.Sorted;

// Replace the exceptions in the code below with the actual code for the assignment.
// Leave the public API the same so it can be tested with the existing evaluation framework.
// Document your design/implementation choices in comments.

public class CoarseGrainedList<T extends Comparable<T>> implements Sorted<T> {

    public class Node {
        T data;
        Node next;

        Node(T d){
            data = d;
            next = null;
        }
    }

    Lock lock = new ReentrantLock();
    Node head = null;

    public void add(T t) {
        lock.lock();
        try {
        Node nodeToAdd = new Node(t);
            //if there is no head, new node is the head
            if(head == null){
                head = nodeToAdd;
            }else{
                Node currNode = head;
                //If nodeToAdd < head, nodeToAdd is new head
                if(nodeToAdd.data.compareTo(head.data) < 0){
                    nodeToAdd.next = head;
                    head = nodeToAdd;
                    return;
                }
                //Starts looping through the list till at most the last node
                while(currNode.next != null){
                    //If our node is larger than (or equal to) the current node
                    //And smaller than (or equal to) the next node
                    //Insert Node in between said nodes
                    if(nodeToAdd.data.compareTo(currNode.data) >= 0 && nodeToAdd.data.compareTo(currNode.next.data) <= 0){
                        nodeToAdd.next = currNode.next;
                        currNode.next = nodeToAdd;
                        return;
                    }
                    //Move onto the next node
                    currNode = currNode.next;
                }
                //If we reach the last node, nodeToAdd is added to the end of the list
                currNode.next = nodeToAdd;
            }
        } finally {
            lock.unlock();
        }
    }

    public void remove(T t) {
        lock.lock();
        try {
            //If list is empty, return
            if(head.data == null){
                return;
            }
            //If the head has the value t, head.next is the new head
            if(head.data.compareTo(t) == 0){
                head = head.next;
                return;
            }
            //Else loop through the list
            Node currNode = head;
            while(currNode.next != null){
                if(currNode.next.data.compareTo(t) == 0){
                    currNode.next = currNode.next.next;
                    return;
                }
                currNode = currNode.next;
            }
        } finally {
            lock.unlock();
        }
    }

    public ArrayList<T> toArrayList() {
        ArrayList<T> temp = new ArrayList<T>();
        Node currNode = head;
        //Loop through linked list and add all elements to the temporary array
        while(currNode != null){
            temp.add(currNode.data);
            currNode = currNode.next;
        }
        return temp;
    }
}
