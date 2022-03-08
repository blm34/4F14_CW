#include <iostream>
#include <string>
#include <random>
#include <time.h>

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
		std::cout << "Int: " << intVal << " \tStr: " << strVal << std::endl;
	}
};

struct Node {
	Item data;
	Node *prev;
	Node *next;
};

class DLL {
public:
	Node *head;
	Node *tail;
	bool reversed;
	
	DLL() {
		head = NULL;
		tail = NULL;
		reversed = false;
	}
	
	void reverse() {
		reversed = (!reversed);
	}
	
	bool empty() {
		if (head == NULL && tail == NULL) {
			return true;
		} else {
			return false;
		}
	}
	
	void push() {
		if (reversed == false) {
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
		} else {
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
	
	void traverse() {
		Node* current = head;
		while (current != NULL) {
			current->data.print();
			current = current->next;
		}
		std::cout << std::endl;
	}
};

int main() {
	// Set the random seed
	srand ( (unsigned)time(NULL) );
	
	DLL dll;
	
	dll.traverse();
	dll.push();
	dll.traverse();
	dll.push();	
	dll.traverse();	
	dll.push();
	dll.traverse();
	dll.push();	
	dll.traverse();
	std::cout << "Reverse\n" << std::endl;
	dll.reverse();	
	dll.push();
	dll.traverse();
	dll.push();	
	dll.traverse();
	
	return 0;
}