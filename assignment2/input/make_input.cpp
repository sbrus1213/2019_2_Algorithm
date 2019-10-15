#include <iostream>
#include <cstring>
#include <cstdio>
#include <cstdlib>
using namespace std;

int main(int argc, char* argv[]) {
	char file_name[200];
	strcpy(file_name, argv[1]);
	int n = atoi(argv[2]);
	int min_val = atoi(argv[3]);
	int max_val = atoi(argv[4]);
	int seed = atoi(argv[5]);

	srand(seed);

	freopen(file_name, "w", stdout);

	cout << n << ' ';
	for (int i = 0; i < n; ++i) {
		int x = rand() % (max_val - min_val + 1) + min_val;
		cout << x << ' ';
	}

	return 0;
}