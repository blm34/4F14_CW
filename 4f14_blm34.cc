#include <iostream>
#include <string>
#include <random>
#include <time.h>
#include <thread>
#include <mutex>
#include <chrono>

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
	Item *data = new Item;
	Node *prev;
	Node *next;
	std::mutex m;
};

class DoubleLinkedList {
public:
	Node *head;
	Node *tail;
	bool reversed;
	int size = 0;
	std::mutex m_head;
	std::mutex m_tail;
	std::mutex m_reversed;
	
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
		if (head == NULL) {
			return true;
		} else {
			return false;
		}
	}
	
	void push() {
		if (!reversed) { // Add a new item to the end of the list
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
		size++;
	}
	
	Item* top() {
		// Return the item at the front of the list
		if (!reversed) {
			return head->data;
		} else {
			return tail->data;
		}
	}
	
	void pop() {
		// Remove the item from the front of the list
		if ( !empty() ) {
			if (!reversed) {
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
			size--;
		}
	}
	
	void traverse() {
		// Print the data for each consecutive item in the list
		if (!reversed) {
			Node* current = head;
			while (current != NULL) {
				current->data->print();
				current = current->next;
			}
		} else {
			Node* current = tail;
			while (current != NULL) {
				current->data->print();
				current = current->prev;
			}
		}
		std::cout << std::endl;
	}
};

void thread3(DoubleLinkedList& queue) {
	auto nextRunTime = std::chrono::high_resolution_clock::now() + std::chrono::milliseconds(200);
	while (!queue.empty()) {
		// Wait for 0.2 seconds to have elapsed since last delete
		while (std::chrono::high_resolution_clock::now() < nextRunTime) {}
		
		// Increment the next run time by 0.2 seconds
		nextRunTime = nextRunTime + std::chrono::milliseconds(200);
		
		// Delete a random item from the queue
		if (queue.size == 1) {
			delete queue.head;
			queue.head = NULL;
			queue.tail = NULL;
		} else {
			int index = rand() % queue.size;
			if (!queue.reversed) {
				if (index == 0) {
					// Delete the head
					queue.head = queue.head->next;
					delete queue.head->prev;
					queue.head->prev = NULL;				
				} else if (index == queue.size-1) {
					// Delete the tail
					queue.tail = queue.tail->prev;
					delete queue.tail->next;
					queue.tail->next = NULL;
				} else {
					// Delete the index th element from the head
					Node* delNode = queue.head;
					for (int i=0; i<index; i++) {
						delNode = delNode->next;
					}
					delNode->prev->next = delNode->next;
					delNode->next->prev = delNode->prev;
					delete delNode;
				}
			} else { // The queue is reversed
				if (index == 0) {
					// Delete the tail
					queue.tail = queue.tail->prev;
					delete queue.tail->next;
					queue.tail->next = NULL;
				} else if (index == queue.size-1) {
					// Delete the head
					queue.head = queue.head->next;
					delete queue.head->prev;
					queue.head->prev = NULL;
				} else {
					// Delete the index th element from the tail
					Node* delNode = queue.tail;
					for (int i=0; i<index; i++) {
						delNode = delNode->prev;
					}
					delNode->prev->next = delNode->next;
					delNode->next->prev = delNode->prev;
					delete delNode;
				}
			}
			std::cout << "Deleted index " << index << std::endl << std::endl;
		}
		queue.size--;
		std::cout << "Length = " << queue.size << std::endl;
		queue.traverse();
	}	
}

int main() {
	// Set the random seed
	srand ( (unsigned)time(NULL) );
	//srand ( 0 );
	
	// Initialise queue with 80 items
	
	DoubleLinkedList queue;
	for (int i=1; i<=5; i++) {
		queue.push();
	}
	queue.reverse();
	queue.traverse();
	
	thread3(queue);
	
	return 0;
}