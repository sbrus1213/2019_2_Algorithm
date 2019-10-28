#include <iostream>
#include <vector>
#include <algorithm>
#include <cstring>
#include <ctime>
#include <cstdio>
#include <cstdlib>
using namespace std;
#define Insertion_Sort 1
#define Quick_Sort 2
#define Merge_Sort 3
#define Improved_Sort 4

void sort_1(vector<int> & num, int l, int r) {
	/*
	O(n^2) 정렬 알고리즘 - Insertion Sort

	Input : num - 정렬 대상 1차원 배열, l - 시작 index, r - 끝 index
	Output : -
	Function :
		Insertion Sort 구현
		Parameter로 구간 대신 사이즈만 넘겨도 되지만 sort_4 에서 작은 사이즈의 경우에 호출하기 위해 구간으로 받는다.
		이를 통해 불필요한 메모리나 재귀호출 등을 피하고자 한다.

		배열을 계속 읽으며 좌측에 Sorted 부분을 늘려간다.
		Unsorted 부분의 첫번째 값을 Sorted 부분의 뒤쪽부터 보면서 맞는 위치로 이동시키기 때문에
		이미 정렬된 배열의 경우엔 O(n) 의 시간복잡도로 정렬이 마무리된다.
		반면에 최악의 경우로는 역정렬 된 상태이다.

		<시간 복잡도>
		- Unsorted Array 부분의 첫 번째 값을 선택 O(n)
			- 선택한 값을 Sorted Array 부분에 맞는 위치에 삽입 O(n)
		최종 시간 복잡도 - O(n^2)
	*/
	for (int i = l; i <= r; ++i) {
		for (int j = i - 1; j >= l; --j) {
			// Unsorted 부분의 값을 Sorted Array의 맞는 위치에 들어갈 때까지 앞의 값과 swap을 해준다.
			if (num[j] > num[j + 1]) swap(num[j], num[j + 1]);
			else break;
		}
	}
}

void sort_2(vector<int>& num, int l, int r) {
	/*
	O(nlogn) 정렬 알고리즘 - Quick Sort

	Input : num - 정렬 대상 1차원 배열, l - 시작 index, r - 끝 index
	Output : -
	Function :
		Quick Sort 구현

		Divide & Conquer 알고리즘을 이용한 정렬 알고리즘
		pivot을 선정하여 배열을 pivot보다 작은 값들, pivot보다 큰 값들 두 부분으로 나눈다.
		각각 두 부분에 대하여 재귀적으로 정렬을 한다.
		Divide & Conquer 을 이용하였으므로 두 부분을 나누는게 정렬에 영향을 미친다.
		역정렬, 또는 정렬 된 상태라면 두 부분이 한쪽에 치우쳐 생기는데
		그 중에 정렬된 상태에서 마지막 값을 피봇으로 잡는다면 호출 때마다 배열을 모두 보게 되고 구간이 한 쪽에 치우치므로
		최악 시간 복잡도인 O(n^2)을 갖는다.

		<시간 복잡도>
		- pivot 선정 O(1)
		- 배열을 두 구간으로 나누는 작업 O(n)
		- 두 구간에 대해 재귀 호출 T(n/2) 최악엔 T(n-1)
			T(n) = 2T(n/2) 일 땐, O(nlogn)
			T(n) = T(n-1) 일 땐, O(n^2)
		최종 시간 복잡도 - O(nlogn) ~ O(n^2)
	*/

	// pivot 을 배열의 마지막 값으로 선정
	// p_index 는 최종에 pivot이 위치할 곳을 결정해주기 위한 변수
	int p_index = l, pivot = num[r];

	if (l < r) {
		for (int i = l; i < r; ++i) {
			// pivot 보다 작은 값을 배열의 앞쪽부터 쌓는다.
			if (num[i] < pivot) {
				swap(num[i], num[p_index]);
				++p_index;
			}
		}
		// pivot 보다 큰 값중 첫번째 값이 num[r]에 위치해 있으므로 그 값을 pivot과 바꿔준다.
		swap(num[r], num[p_index]);

		// pivot 보다 작은 값들에 대해 재귀 호출
		sort_2(num, l, p_index - 1);
		// pivot 보다 큰 값들에 대해 재귀 호출
		sort_2(num, p_index + 1, r);
	}
}

void sort_3(vector<int>& num, vector<int>& temp, int l, int r) {
	/*
	O(nlogn) 정렬 알고리즘 - Merge Sort

	Input : num - 정렬 대상 1차원 배열, temp - 정렬을 위해 값을 임시로 저장하는 1차원 배열 l - 시작 index, r - 끝 index
	Output : -
	Function :
		Merge Sort 구현

		Divide & Conquer 알고리즘을 이용한 정렬 알고리즘
		배열을 절반으로 나눠 재귀호출로 각 부분을 정렬한다.
		정렬이 끝난 두 부분의 맨 앞의 값부터 비교해가며 더 작은 값을 새로운 배열에 담는다.
		한 쪽 부분의 값을 모두 담은 후에는 남은 부분의 값들을 전부 순서대로 새로운 배열에 담아준다.

		<시간 복잡도>
		- 배열을 절반으로 나누어 재귀 호출 T(n/2)
			T(n) = 2T(n/2), O(nlogn)
		- 두 구간을 서로 비교해가면서 정렬된 배열을 생성 O(n)
		최종 시간 복잡도 - O(nlogn)
	*/

	// 배열의 사이즈가 1개 일 경우, 별도의 정렬을 하지 않는다.
	if (l == r) {
		temp[l] = num[l];
		return;
	}

	// 구간을 나누기 위해 중간 index를 잡아준다.
	int mid = (l + r) / 2;

	// 구간의 좌측 부분에 대해 재귀 호출
	sort_3(num, temp, l, mid);
	// 구간의 우측 부분에 대해 재귀 호출
	sort_3(num, temp, mid + 1, r);

	// 양쪽 구간의 값을 임시로 저장
	for (int i = l; i <= r; ++i) temp[i] = num[i];

	// li - 왼쪽 구간을 탐색할 index, ri - 오른쪽 구간을 탐색할 index, i - 최종 정렬된 배열을 생성해줄 index
	int li = l, ri = mid + 1, i = l;
	while (li <= mid && ri <= r) {
		// 좌측 구간의 값이 우측 구간의 값보다 작으면 좌측 구간의 값을 정렬된 배열의 맨 뒤에 넣어준다.
		if (temp[li] < temp[ri]) num[i++] = temp[li++];
		// 좌측 구간의 값이 우측 구간의 값보다 크거나 같으면 우측 구간의 값을 정렬된 배열의 맨 뒤에 넣어준다.
		else num[i++] = temp[ri++];
	}
	// 우측 구간의 값을 먼저 정렬 배열에 다 넣었다면 남은 좌측 구간의 값들을 정렬 배열에 넣어준다.
	while (li <= mid) num[i++] = temp[li++];
	// 좌측 구간의 값을 먼저 정렬 배열에 다 넣었다면 남은 우측 구간의 값들을 정렬 배열에 넣어준다.
	while (ri <= r) num[i++] = temp[ri++];
}

void sort_4(vector<int>& num, int l, int r) {
	/*
	Improved User Sort

	Input : num - 정렬 대상 1차원 배열, l - 시작 index, r - 끝 index
	Output : -
	Function :
		Quick Sort를 pivot 선정 최적화 등으로 시간복잡도를 안정화

		최적화 방법
		1) Quick Sort에서 정렬이 된 배열 등 pivot 선정의 문제로 시간이 O(n^2)으로 걸리는 문제를 해결하기 위해
			pivot을 특정 위치의 값으로 선택하는 것이 아닌 랜덤으로 결정한다.
			=> 최악 시간 복잡도 n^2에서 nlogn으로 감소
		2) 기존에는 Single pivot으로 구현했지만 2개의 pivot을 이용해서 구간을 총 3부분으로 나누었다.
			=> 최악 시간 복잡도 nlogn에서 logn의 밑을 2에서 3으로 변경
				n = 1'000'000 기준 logn = 19 -> 13
		3) 정렬 할 배열 사이즈가 작을 땐, insertion sort 를 실행한다.
			=> 재귀 등으로 생기는 스택 메모리의 낭비 및 시간을 단축한다.

		<시간 복잡도>
		- pivot 선정 O(1)
		- 배열을 세 구간으로 나누는 작업 O(n)
		- 세 구간에 대해 재귀 호출 T(n/3)
			T(n) = 3T(n/3) + cn 일 땐, O(nlog_3(n))
		최종 시간 복잡도 - O(nlog_3(n))
	*/

	// l_pivot - pivot 중 작은 값, r_pivot - pivot 중 큰 값
	// lp_index - l_pivot 이 최종에 위치할 index
	// rp_index - r_pivot 이 최종에 위치할 index
	int lp_index = l + 1, rp_index = r - 1, l_pivot, r_pivot;

	if (l < r) {

		if (r - l <= 32)
			// 구간의 크기가 32 보다 작은 경우엔 insertion sort로 진행
			sort_1(num, l, r);
		else {
			// l_pivot을 랜덤으로 선정
			int rand_idx = rand() % (r - l + 1) + l;
			swap(num[l], num[rand_idx]);
			// r_pivot을 랜덤으로 선정
			rand_idx = rand() % (r - l + 1) + l;
			swap(num[r], num[rand_idx]);

			// 좌측 pivot이 더 작도록
			if (num[l] > num[r]) swap(num[l], num[r]);
			l_pivot = num[l], r_pivot = num[r];

			for (int i = l + 1; i <= rp_index; ++i) {
				// 좌측 pivot 보다 작다면 1번 구간으로
				if (num[i] < l_pivot) swap(num[i], num[lp_index++]);
				// 우측 pivot 보다 크다면 3번 구간으로
				else if (num[i] > r_pivot) swap(num[i--], num[rp_index--]);
			}
			// 좌측 pivot을 맞는 자리에
			swap(num[l], num[--lp_index]);
			// 우측 pivot을 맞는 자리에
			swap(num[r], num[++rp_index]);

			// 각 구간에 대해 재귀 호출
			sort_4(num, l, lp_index - 1);
			sort_4(num, lp_index + 1, rp_index - 1);
			sort_4(num, rp_index + 1, r);
		}
	}
}

int main(int argc, char* argv[]) {
	//int main() {
	cin.tie(NULL); cout.tie(NULL);
	ios_base::sync_with_stdio(false);

	srand(time(NULL));

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

	// 배열 저장
	for (int i = 1; i <= n; ++i) cin >> num[i];

	start = clock();

	algo_id = atoi(argv[2]);
	// 알고리즘별 정렬 수행
	switch (algo_id) {
	case Insertion_Sort:
		sort_1(num, 1, n);
		break;

	case Quick_Sort:
		sort_2(num, 1, n);
		break;

	case Merge_Sort:
		sort_3(num, tmp, 1, n);
		break;

	case Improved_Sort:
		sort_4(num, 1, n);
		break;
	}

	end = clock();

	cout << argv[1] << endl;
	cout << algo_id << '\n';
	cout << n << '\n';
	cout << fixed; cout.precision(6);
	cout << ((double)end - start) / CLOCKS_PER_SEC << '\n';
	for (int i = 1; i <= n; ++i) cout << num[i] << ' ';
	cout << '\n';

	return 0;
}