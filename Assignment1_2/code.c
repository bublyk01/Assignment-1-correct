#include <stdio.h>
#include <iostream>
using namespace std;
void func(int* ptr) {
	*ptr = 5;
}
int main() {
	int x = 10;
	func(&x);
	cout << x << endl;
	return 0;
}