
public class LinkList {

	public static void main(String[] args) {
		
		// creating a new List
		List l = new List();
		insertAtEnd(l, 13);
		insertAtEnd(l, 5);
		insertAtEnd(l, 10);
		insertAtEnd(l, 11);
		insertAtStart(l, 4);
		insertAtStart(l, 9);
		insertAtStart(l, 1);
		insertAtStart(l, 10);
		
		System.out.println(" Printing out generated List:");
		printList(l);
		
		System.out.println("\n Applying partition to List");
		partition(l);
		printList(l);

		System.out.println("\n Now sorting the List:");
		sort(l);
		printList(l);
		
		System.out.println("\n Now reversing List:");
		reverse(l);
		printList(l);
		
		System.out.println("\n Now applying min to front:");
		minToFront(l);
		printList(l);

	}

	public static void insertAtStart(List l, int value) {
		Link newLink = new Link(value);
		newLink.next = l.head;
		l.head = newLink;
	}

	public static void insertAtEnd(List l, int value) {
		Link newLink = new Link(value);

		Link curr = l.head;
		if (curr == null){
			l.head = newLink;
		} else {
			while (curr.next != null){
				curr = curr.next;
			}
			curr.next = newLink;
		}

	}

	public static void deleteFirst(List l) {
		if (l.head != null){
			l.head = l.head.next;
		}
	}

	public static void printList(List l) {
		if (l != null && l.head != null){
			Link curr = l.head;
			while (curr.next != null){
				System.out.print(curr.value + " -> ");
				curr = curr.next;
			}
			System.out.println(curr.value);
		}
	}
   
   // finds the minimum value in the list and moves it to the front of
   // the list. If more than one, moves the first one
	static void minToFront(List l){
		Link curr = l.head;

		// keeping track of index of each node
		int index;

		// variables to store the min value and index it is at
		int min;
		int minIndex;

		// only checking min if List isn't empty
		if (curr != null){
			minIndex = 0;
			index = 0;
			min = curr.value;

			// cycling through whole List to find location of min
			while (curr.next != null){
				index++;
				if (curr.next.value < min){
					min = curr.next.value;
					minIndex = index;
				}
				curr = curr.next;
			}

			//printf("min of %d at index %d\n",min,minIndex);

			// now getting the Link to the index
			index = 0;
			curr = l.head;
			Link prev = null;
			while (index < minIndex){
				prev = curr;
				curr = curr.next;
				index++;
			}

			// breaking min out of chain and Linking
			Link temp = curr;
			prev.next = curr.next;

			// connecting min to front
			temp.next = l.head;
			l.head = temp;

		}
	}

	static void reverse(List l){
		if (l != null && l.head != null && l.head.next != null){
			// tracker of nodes through the source List
			Link curr = l.head;
			
			// virtual List head to store the nodes in sorted order
			Link newHead = l.head;

			// putting the first node from the source in the virtual List
			l.head = curr.next;
			newHead.next = null;
			curr = l.head;

			while (curr != null){
				l.head = curr.next;
				curr.next = newHead;
				newHead = curr;
				curr = l.head;
			}
			
			l.head = newHead;
		}

	}
	
	static void sort(List l){
		assert(l != null);
		if (l.head != null){
			// tracker of nodes through the source List
			Link curr = l.head;
			
			// virtual List head to store the nodes in sorted order
			Link newHead = l.head;

			// putting the first node from the source in the virtual List
			l.head = curr.next;
			newHead.next = null;
			curr = l.head;

			while (curr != null){
				Link newHeadCurr = newHead;
				Link newHeadPrev = newHead;

				// getting the two nodes with values on either side of curr value
				while (newHeadCurr != null && (curr.value > newHeadCurr.value)){
					newHeadPrev = newHeadCurr;
					newHeadCurr = newHeadCurr.next;
				}

				if (newHeadCurr == newHeadPrev){
					// if it should be put at the start
					l.head = curr.next;
					curr.next = newHeadCurr;
					newHead = curr;
				} else {
					// otherwise
					l.head = curr.next;
					curr.next = newHeadCurr;
					newHeadPrev.next = curr;
				}
				curr = l.head;
			}
			l.head = newHead;
		}
	}

	static void partition(List l){
		if (l.head != null){
			if (l.head.next != null){
				// storing the key integer as the benchmark
				int benchmark = l.head.value;
				
				// a virtual List for the smaller integers
				Link newHead = null;
				
				Link curr = l.head.next;
				Link prev = l.head;
				
				while (curr != null){
					if (curr.value >= benchmark){
						prev = curr;
						curr = curr.next;
					} else {
						if (newHead == null){
							newHead = curr;
							prev.next = curr.next;
							curr.next = null;
						} else {
							Link newHeadCurr = newHead;
							// getting the pointer to the last node
							while (newHeadCurr.next != null){
								newHeadCurr = newHeadCurr.next;
							}
							newHeadCurr.next = curr;
							prev.next = curr.next;
							curr.next = null;
						}
						curr = prev.next;
					}
				}
				
				if (newHead != null){
					Link newHeadCurr = newHead;
					// getting the pointer to the last node
					while (newHeadCurr.next != null){
						newHeadCurr = newHeadCurr.next;
					}
					newHeadCurr.next = l.head;
					l.head = newHead;
				}
				
			}
		}
	}
}

class Link {
	// fields of the object 'Link'
	public int value;
	public Link next;

    // constructor of the object which sets the parameters
	public Link(int value){
		this.value = value;
		this.next = null;
	}
}

class List {
	// parameter 'head'
   public Link head;

   // constructor
	List(){
		head = null;
	}
}