# Linked List Project
## Goal
The goal of this program is to implement and test the linked list data structure. The linked list consists of a collection of nodes. Each node consists of two fields: one field has data, and in the second field, the node has an address that keeps a reference to the next node.

## The Program
### General Information
This project implements a singly linked list, each Node containing a string of data and the next Node. The linked list contains additional information such as which Node is the head, which Node is the tail, and the size of the list. The MyLinkedList.java is the implementation of the linked list structure, and the TestList.java is the file used to test the implemented structure.

### Methods
- getFirst() - Returns the data stored in the head Node
- getLast() - Returns the data stored in the tail Node
- addFirst(String e) - Adds a new Node at the beginning of the linked list
- addLast(String e) - Adds a new Node at the end of the linked list
- add(int index, String e) - Adds a new Node at the specified index in the linked list
- removeFirst() - Removes the head Node in the linked list and returns the data in the removed Node
- removeLast() - Removes the tail Node in the linked list and returns the data in the removed Node
- remove(int index) - Removes the Node at the specified index in the linked list and returns the data in the removed Node
- toString() - Returns a string of all of the data in the linked list
- clear() - Clears the entire list
- contains(String e) - Returns true if the specified string is found in the linked list
- set(int index, String e) - Replace the data at the specified index in the linked list with the specified string, and returns the data in the replaced Node