#include <iostream>
#include <atomic>
#include <thread>
#include <windows.h>
using namespace std;

const int N = 20;

atomic <int> arr[N];
atomic <int> counter{ 0 };
int maxVal = 20;
int minVal = -20;
void init()
{
	srand(time(0));
	for (int i = 0; i < 20; i++) {
		arr[i] = minVal + rand() % (maxVal - minVal + 1);
	}
}
int even_num() {
	for (int i = 0; i < 20; i++) {
		if (arr[i] % 2 == 0) {
			counter++;
		}
	}
	return counter;
}
int neg_num() {
	for (int i = 0; i < 20; i++) {
		if (arr[i] < 0) {
			counter++;
		}
	}
	return counter;
}
int main()
{
	init();

	cout << "Array: \n";
	for (int i = 0; i < 20; i++) {
		cout << arr[i] << ' ';
		cout << '\n';
	}
	thread t1(even_num);
	t1.join();
	int even = counter.load();
	thread t2(neg_num);
	t2.join();
	int neg = counter.load();

	cout << "Evens: " << even << endl;
	cout << "Negatives: " << neg << endl;
}