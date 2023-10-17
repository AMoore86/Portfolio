
public class TestList {

	public static void main(String[] args) {
		
		MyLinkedList myList = new MyLinkedList();
		
		//add first
		myList.addFirst("elm");
		myList.addLast("oak");
		myList.add(1, "pine");
		myList.add(1, "maple");
		myList.add(2,  "hickory");
		myList.add(3, "ash");
		
		System.out.println("The first item in the list is " + myList.getFirst());
		System.out.println("The last item in the list is " + myList.getLast());
		System.out.println("The entire list is " + myList);
		
		myList.set(2, "birch");
		System.out.println("After replacing the item at index 2, the list is " + myList);
		
		if (myList.contains("beech")) {
			System.out.println("Beech is in the list");
		}
		else {
			System.out.println("Beech is not in the list");
		}
		if (myList.contains("pine")) {
			System.out.println("Pine is in the list");
		}
		else {
			System.out.println("Pine is not in the list");
		}
		
		myList.removeFirst();
		System.out.println("List after removing first:" + myList);
		myList.removeLast();
		System.out.println("List after removing last:" + myList);
		myList.remove(2);
		System.out.println("List after removing item at index 2:" + myList);
		myList.clear();
		System.out.println("List after clearing:" + myList);
		

	}

}

