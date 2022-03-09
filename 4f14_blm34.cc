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
	int size;
	
	std::mutex m_reversed;
	std::mutex m_head;
	std::mutex m_tail;
	
	DoubleLinkedList() {
		// Initialised as empty
		head = NULL;
		tail = NULL;
		reversed = false;
		size = 0;
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
		
		{ // New scope so locked mutexs are unlocked after deletion is complete
			std::lock_guard<std::mutex> g_reversed(queue.m_reversed); // Lock direction
			
			// Delete a random item from the queue
			if (queue.size == 1) {
				// Lock relevant nodes
				std::lock_guard<std::mutex> g_head(queue.m_head);
				std::lock_guard<std::mutex> g_tail(queue.m_tail);
				std::lock_guard<std::mutex> g_delNode(queue.head->m);
				
				// Delete final node
				delete queue.head;
				queue.head = NULL;
				queue.tail = NULL;
			} else {
				int index = rand() % queue.size;
				if (!queue.reversed) {
					if (index == 0) {
						// Lock relevant nodes
						std::lock_guard<std::mutex> g_head(queue.m_head);
						std::lock_guard<std::mutex> g_delNode(queue.head->m);
						std::lock_guard<std::mutex> g_nextNode(queue.head->next->m);						
						
						// Delete the head
						queue.head = queue.head->next;
						delete queue.head->prev;
						queue.head->prev = NULL;				
					} else if (index == queue.size-1) {
						// Lock relevant nodes
						std::lock_guard<std::mutex> g_tail(queue.m_tail);
						std::lock_guard<std::mutex> g_prevNode(queue.tail->prev->m);
						std::lock_guard<std::mutex> g_delNode(queue.tail->m);
						
						// Delete the tail
						queue.tail = queue.tail->prev;
						delete queue.tail->next;
						queue.tail->next = NULL;
					} else {
						// Traverse list to the node to delete
						Node* delNode = queue.head;
						for (int i=0; i<index; i++) {
							delNode = delNode->next;
						}
						
						// Lock relevant nodes
						std::lock_guard<std::mutex> g_prevNode(delNode->prev->m);
						std::lock_guard<std::mutex> g_delNode(delNode->m);
						std::lock_guard<std::mutex> g_nextNode(delNode->next->m);
						
						// Delete the index th element from the head
						delNode->prev->next = delNode->next;
						delNode->next->prev = delNode->prev;
						delete delNode;
					}
				} else { // The queue is reversed
					if (index == 0) {
						// Lock relevant nodes
						std::lock_guard<std::mutex> g_tail(queue.m_tail);
						std::lock_guard<std::mutex> g_delNode(queue.tail->m);
						std::lock_guard<std::mutex> g_nextNode(queue.tail->prev->m);
						
						// Delete the tail
						queue.tail = queue.tail->prev;
						delete queue.tail->next;
						queue.tail->next = NULL;
					} else if (index == queue.size-1) {
						// Lock relevant nodes
						std::lock_guard<std::mutex> g_head(queue.m_head);
						std::lock_guard<std::mutex> g_prevNode(queue.head->next->m);
						std::lock_guard<std::mutex> g_delNode(queue.head->m);
						
						// Delete the head
						queue.head = queue.head->next;
						delete queue.head->prev;
						queue.head->prev = NULL;
					} else {
						// Traverse list to the node to delete
						Node* delNode = queue.tail;
						for (int i=0; i<index; i++) {
							delNode = delNode->prev;
						}
						
						// Lock relevant nodes
						std::lock_guard<std::mutex> g_prevNode(delNode->next->m);
						std::lock_guard<std::mutex> g_delNode(delNode->m);
						std::lock_guard<std::mutex> g_nextNode(delNode->prev->m);
						
						// Delete the index th element from the tail
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
}

class thread_guard {
	std::thread& t;
public:
	explicit thread_guard(std::thread& t_): t(t_){}
	~thread_guard() {
		if (t.joinable()) {
			t.join();
		}
	}
	thread_guard(thread_guard const&)=delete;
	thread_guard& operator=(thread_guard const&)=delete;
};

int main() {
	// Set the random seed
	srand ( (unsigned)time(NULL) );
	//srand ( 0 );
	
	// Initialise queue with 80 items	
	DoubleLinkedList queue;
	for (int i=1; i<=6; i++) {
		queue.push();
	}
	
	// Print initialised queue
	queue.traverse();
	
	// Start the deleting thread
	std::thread t3(thread3, std::ref(queue));
	thread_guard g3(t3);
	
	return 0;
}