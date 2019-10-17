#include <iostream>
#include <vector>
#include <algorithm>
#include <cstring>
#include <ctime>
#include <cstdio>
using namespace std;

void sort_1(vector<int>& num, int n) {
	for (int i = 1; i <= n; ++i) {
		for (int j = i - 1; j > 0; --j) {
			if (num[j] > num[j + 1]) swap(num[j], num[j + 1]);
			else break;
		}
	}
}

void sort_2(vector<int>& num, int l, int r) {
	int pivot = l;

	if (l < r) {
		for (int i = l; i < r; ++i) {
			if (num[i] < num[r]) {
				swap(num[i], num[pivot]);
				++pivot;
			}
		}
		swap(num[r], num[pivot]);

		sort_2(num, l, pivot - 1);
		sort_2(num, pivot + 1, r);
	}
}

void sort_3(vector<int>& num, vector<int>& temp, int l, int r) {
	if (l == r) {
		temp[l] = num[l];
		return;
	}

	int mid = (l + r) / 2;

	sort_3(num, temp, l, mid);
	sort_3(num, temp, mid + 1, r);

	for (int i = l; i <= r; ++i) temp[i] = num[i];

	int li = l, ri = mid + 1, i = l;
	while (li <= mid && ri <= r) {
		if (temp[li] < temp[ri]) num[i++] = temp[li++];
		else num[i++] = temp[ri++];
	}
	while (li <= mid) num[i++] = temp[li++];
	while (ri <= r) num[i++] = temp[ri++];
}

void sort_4(vector<int>& num, int n) {}

int main(int argc, char* argv[]) {
	//int main() {
	cin.tie(NULL); cout.tie(NULL);
	ios_base::sync_with_stdio(false);

	freopen(argv[1], "r", stdin);
	char res_file[50] = "result_";
	strcat(res_file, argv[2]);
	strcat(res_file, "_");
	strcat(res_file, argv[1]);
	freopen(res_file, "w", stdout);

	int n, algo_id;
	vector<int> num, tmp;

	clock_t start, end;

	cin >> n;
	num.resize(n + 1);
	tmp.resize(n + 1);

	for (int i = 1; i <= n; ++i) cin >> num[i];

	start = clock();

	algo_id = atoi(argv[2]);

	switch (algo_id) {
	case 1:
		sort_1(num, n);
		break;

	case 2:
		sort_2(num, 1, n);
		break;

	case 3:
		sort_3(num, tmp, 1, n);
		break;

	case 4:
		sort_4(num, n);
		break;
	}

	end = clock();

	cout << argv[1] << endl;
	cout << algo_id << '\n';
	cout << n << '\n';
	cout << ((double)end - start) / CLOCKS_PER_SEC << '\n';
	for (int i = 1; i <= n; ++i) cout << num[i] << ' ';
	cout << '\n';

	return 0;
}

