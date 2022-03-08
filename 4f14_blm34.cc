#include <iostream>
#include <string>
#include <stdlib.h>
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
};

int main() {
	// Set the random seed
	srand ( (unsigned)time(NULL) );
	// Generate 10 items and print their values
	for (int i=0; i<10; i++) {
		Item j;
		std::cout << i << " Random num: " << j.intVal << std::endl;
		std::cout << "  String: " << j.strVal << std::endl;
		std::cout << std::endl;
	}
	return 0;
}