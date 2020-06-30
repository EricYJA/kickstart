#include <iostream>
using namespace std;

int main() {
	int v1, v2;
	cin >> v1 >> v2 >> v1;
	cout << "v1: " << v1 << ", v2: " << v2 << endl;

	cout << "/*";
	cout << "*/";
	// This line would not work
	// cout << /* "*/" */;

  return 0;
}

