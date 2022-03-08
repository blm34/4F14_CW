#include <iostream>
#include <thread>
#include <vector>
#include <functional>
#include <algorithm>

void work_function(unsigned int i) {
	std::cout << "Doing Work " << i << std::endl;
}

void distributed_work_function(unsigned int n) {
	std::vector<std::thread> threads;
	for (unsigned int i = 0; i < n; i++) {
		threads.push_back(std::thread(work_function, i));
	}
	std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));
}

int main() {
	int n = std::thread::hardware_concurrency();
	std::cout << n << " cores" << std::endl;
	distributed_work_function(n);
}