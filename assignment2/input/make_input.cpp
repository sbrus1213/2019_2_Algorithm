#include <iostream>
#include <vector>
#include <algorithm>
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
	int order = 0;
	if(argc == 7) order = atoi(argv[6]);
	vector<int> arr(n);

	srand(seed);

	freopen(file_name, "w", stdout);

	for (int i = 0; i < n; ++i)
		arr[i] = rand() % (max_val - min_val + 1) + min_val;
	
	switch(order) {
	case 0:
		break;	
	case 1: 
		sort(arr.begin(), arr.end(), less<int>());
		break;
	case -1:
		sort(arr.begin(), arr.end(), greater<int>());	
	}

	cout << n << ' ';
	for(int i = 0;i < n;++i)
		cout<<arr[i] <<' ';

	return 0;
}
