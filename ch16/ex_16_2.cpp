#include <iostream>
using std::cout;
using std::endl;

template <typename T>
int compare(const T &v1, const T &v2) {
	if (v1 < v2) return -1;
	if (v2 < v1) return 1;
	return 0; 
}

int main()
{
	cout << compare(1, 2) << endl;
	return 0;
}