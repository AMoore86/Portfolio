
public class MyLinkedList {
	
	/** Define the node class and node constructor */
	  public static class Node {
	    String element;
	    Node next;
	
	    public Node(String element) {
	      this.element = element;  
	      //next is empty
	    }
	  }
	
	  
  /** Define the list */
	  
  /**instance variables for the list */
  private Node head, tail;
  private int size = 0;

  /** Create an empty list */
  public MyLinkedList() {
	  this.head = null;
	  this.tail = null;
	  size = 0;
  }
  
  /** Return the head element in the list */
  public String getFirst() {
    if (size == 0) {
      return null;
    }
    else {
      return head.element;
    }
  }

  /** Return the last element in the list */
  public String getLast() {
    if (size == 0) {
      return null;
    }
    else {
      return tail.element;
    }
  }

  /** Add an element to the beginning of the list */
  public void addFirst(String e) {
    Node newNode = new Node(e); // Create a new node
    newNode.next = head; // link the new node with the head
    head = newNode; // head points to the new node

    if (tail == null) // the new node is the only node in list
      tail = head;
    
    size++; // Increase list size
  }

  /** Add an element to the end of the list */
  public void addLast(String e) {

    Node current = head;
    //move current to node that will precede the new node
    for (int i = 1; i < size; i++) {
      current = current.next;
    }
    Node newNode = new Node(e);
    newNode.next = current.next;
    current.next = newNode;
    size++;

  }


  /** Add a new element at the specified index 
   * in this list. The index of the head element is 0 */
  public void add(int index, String e) {
    if (index == 0) {
        addFirst(e);
      }
      else if (index >= size) {
        addLast(e);
      }
      else {
        Node current = head;
        //move current to node that will precede the new node
        for (int i = 1; i < index; i++) {
          current = current.next;
        }
        Node newNode = new Node(e);
        newNode.next = current.next;
        current.next = newNode;
        size++;
      }
    }

  /** Remove the head node and
   *  return the object that is contained in the removed node. 
   *  return null if list is empty */
  public String removeFirst() {

      if(size == 0) {
        return null;
      }

      Node originalHead = head;
      if(size > 1) {
        Node newHead = originalHead.next;
        head = newHead;
      }

      size--;

	  return originalHead.element;
  }

  /** Remove the last node and
   * return the object that is contained in the removed node. */
  public String removeLast() {
    if (size == 0) {
      return null;
    }
    else if (size == 1) {
      Node temp = head;
      head = tail = null;
      size = 0;
      return temp.element;
    }
    else {
      Node current = head;

      for (int i = 0; i < size - 2; i++) {
        current = current.next;
      }

      Node temp = tail;
      tail = current;
      tail.next = null;
      size--;
      return temp.element;
    }
  }

  /** Remove the element at the specified position in this 
   *  list. Return the element that was removed from the list. */
  public String remove(int index) {   
    if (index < 0 || index >= size) {
      return null;
    }
    else if (index == 0) {
      return removeFirst();
    }
    else if (index == size - 1) {
      return removeLast();
    }
    else {
      Node previous = head;

      for (int i = 1; i < index; i++) {
        previous = previous.next;
      }

      Node current = previous.next;
      previous.next = current.next;
      size--;
      return current.element;
    }
  }

  @Override /** Override toString() to return elements in the list */
  public String toString() {
    StringBuilder result = new StringBuilder("[");

    Node current = head;
    for (int i = 0; i < size; i++) {
      result.append(current.element);
      current = current.next;
      if (current != null) {
        result.append(", "); // Separate two elements with a comma
      }
    }
    result.append("]"); // Insert the closing ] in the string

    return result.toString();
  }

  /** Clear the list */
  public void clear() {
    size = 0;
    head = tail = null;
  }

  /** Return true if this list contains the element e */
  public boolean contains(String e) {

    boolean containsElement = false;

    Node current = head;
    for (int i = 0; i < size; i++) {
      if(current.element == e) {
        containsElement = true;
        break;
      }
      current = current.next;
    }

    return containsElement;
  }

  /** Replace the element at the specified position 
   *  in this list with the specified element. 
   *  Returns the replaced element 
   *  Returns null if index does not exist */
  public String set(int index, String e) {
    if (index < 0 || index >= size) {
      return null;
    }
    else if (index == 0) {
      Node originalHead = head;
      Node newHead = head;
      newHead.element = e;
      head = newHead;

      return originalHead.element;
    }
    else if (index == size - 1) {
      Node originalTail = tail;
      Node newTail = tail;
      newTail.element = e;

      Node current = head;
      for (int i = 1; i < index - 1; i++) {
        current = current.next;
      }

      current.next = newTail;
      tail = newTail;

      return originalTail.element;
    }
    else {
      Node current = head;
      Node previous = head;

      for (int i = 1; i < index; i++) {
        current = current.next;
        if(i < index - 1) {
          previous = previous.next;
        }
      }

      Node newCurrent = current;
      newCurrent.element = e;
      previous.next = newCurrent;

      return current.element;
    }
  }

  
  
}
