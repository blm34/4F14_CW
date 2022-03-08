#include <iostream>
#include <string>
#include <random>
#include <time.h>
#include <thread>

class Item {
public:
	int intVal;
	std::string strVal;
	
	Item() {
		// Random int in range [0, 256]
		intVal = rand() % 256;
		
		// Random str of length [3, 7] of random chars in range [a, z] (ascii range [97, 122])
		int strLen = rand() % 5 + 3;
		strVal.resize(strLen);
		for (int i=0; i<=strLen; i++) {
			strVal[i] = rand() % 26 + 97;
		}
	}
	
	void print() {
		// Output the integer and string of the item
		std::cout << "Int: " << intVal << "   \tStr: " << strVal << std::endl;
	}
};

struct Node {
	// A node in a double linked list
	Item data;
	Node *prev;
	Node *next;
};

class DoubleLinkedList {
public:
	Node *head;
	Node *tail;
	bool reversed;
	
	DoubleLinkedList() {
		// Initialised as empty
		head = NULL;
		tail = NULL;
		reversed = false;
	}
	
	void reverse() {
		// Toggle the reverse flag
		reversed = (!reversed);
	}
	
	bool empty() {
		// Return true if the list is empty
		if (head == NULL && tail == NULL) {
			return true;
		} else {
			return false;
		}
	}
	
	void push() {
		if (reversed == false) { // Add a new item to the end of the list
			Node *newNode = new Node;
			newNode->next = NULL;
			newNode->prev = tail;
			if (tail != NULL) {
				tail->next = newNode;
			}
			tail = newNode;
			if (head == NULL) {
				head = newNode;
			}
		} else { // end is actually at the beginning
			Node *newNode = new Node;
			newNode->prev = NULL;
			newNode->next = head;
			if (head != NULL) {
				head->prev = newNode;
			}
			head = newNode;
			if (tail == NULL) {
				tail = newNode;
			}
		}
	}
	
	Item top() {
		// Return the item at the front of the list
		if (reversed == false) {
			return head->data;
		} else {
			return tail->data;
		}
	}
	
	void pop() {
		// Remove the item from the front of the list
		if ( !empty() ) {
			if (reversed == false) {
				if (head->next != NULL){ // More than one item in the list
					head = head->next;
					delete head->prev;
					head->prev = NULL;
				} else { // Remove the last item in the array
					delete head;
					head = NULL;
					tail = NULL;
				}
			} else { // Front of the list is actually the end
				if (tail->prev != NULL) { // More than one item in the list
					tail = tail->prev;
					delete tail->next;
					tail->next = NULL;
				} else { // Remove the last item in the list
					delete tail;
					head = NULL;
					tail = NULL;
				}
			}
		}
	}
	
	void traverse() {
		// Print the data for each consecutive item in the list
		if (reversed == false) {
			Node* current = head;
			while (current != NULL) {
				current->data.print();
				current = current->next;
			}
		} else {
			Node* current = tail;
			while (current != NULL) {
				current->data.print();
				current = current->prev;
			}
		}
		std::cout << std::endl;
	}
};

int main() {
	// Set the random seed
	srand ( (unsigned)time(NULL) );
	//srand ( 0 );
	
	// Initialise queue with 80 items
	DoubleLinkedList queue;
	for (int i=1; i<=80; i++) {
		queue.push();
	}
	queue.traverse();
	
	return 0;
}