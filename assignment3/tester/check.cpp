#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include <cstring>
using namespace std;

int main(int argc, char* argv[]) {
	char file1[200], file2[200];
	strcpy(file1, argv[1]);
	strcpy(file2, argv[1]);
	strcat(file2, ".zz.yy");

	string Input1 = "", Input2 = "", tmp;

	ifstream f1(file1);

	while(!f1.eof()) {
		getline(f1, tmp);
		Input1 += tmp + "\n";
	}
	Input1.erase(Input1.size() - 1, 1);

	ifstream f2(file2);

	while(!f2.eof()) {
		getline(f2, tmp);
		Input2 += tmp + "\n";
	}
	Input2.erase(Input2.size() - 1, 1);

	if(!Input1.compare(Input2)) cout << "\tSuccess !!" << file1 << " and " << file2 << " are same!\n";
	else cout << "\tFail !!" << file1 << " and " << file2 << " are different!\n";
}
