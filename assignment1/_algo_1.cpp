#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstring>
using namespace std;
#define O6algorithm 1
#define O4algorithm 2
#define O3algorithm 3

int O_6_algorithm(vector<vector<int> >&num, int n, int m) {
	/*
	O(n^6) 알고리즘 구현 함수 - Brute Force 알고리즘

	Input : num - 2차원 배열, n - row size, m - column size
	Output : n x m 사이즈의 행렬에서 가장 큰 사각 부분행렬의 합
	Function :
		- n x m 사이즈의 행렬 내에서 임의의 좌측 상단 점, 우측 하단 점을 잡고 해당 범위의 모든 값을 더해 사각 부분행렬의 합을 구한다.

		<시간복잡도>
		- 임의의 좌측 상단 점을 구하는데 O(n*m)
			- 임의의 우측 하단 점을 구하는데 O(n*m)
				- 좌측 상단 ~ 우측 하단 의 행렬 값을 더하는데 O(n*m)
		- 최종 시간 복잡도 - O(n^3*m^3) 이지만 n=m이라는 가정 하에 O(n^6)의 시간복잡도를 갖는다.
	*/

	// return 변수를 int형 범위 내 매우 작은 값으로 초기화
	int max_res = -(1e9 + 7), partial_sum;

	// 사각형의 좌측 상단 시작 점 (UL_i, UL_j)
	for (int UL_i = 0; UL_i < n; ++UL_i) {
		for (int UL_j = 0; UL_j < m; ++UL_j) {
			// 사각형의 우측 하단 끝 점 (DR_i, DR_j)
			for (int DR_i = UL_i; DR_i < n; ++DR_i) {
				for (int DR_j = UL_j; DR_j < m; ++DR_j) {
					// partial_sum = (UL_i, UL_j) ~ (DR_i, DR_j) 의 합
					partial_sum = 0;

					for (int i = UL_i; i <= DR_i; ++i) {
						for (int j = UL_j; j <= DR_j; ++j) {
							partial_sum += num[i][j];
						}
					}

					// max_res 값 갱신
					max_res = max(max_res, partial_sum);
				}
			}
		}
	}

	return max_res;
}

int O_4_algorithm(vector<vector<int> >& num, int n, int m) {
	/*
	O(n^4) 알고리즘 구현 함수 - Dynamic Programming 알고리즘

	Input : num - 2차원 배열, n - row size, m - column size
	Output : n x m 사이즈의 행렬에서 가장 큰 사각 부분행렬의 합
	Function :
		- 1차원(수열) 에서 dynamic programming을 이용해 부분 수열의 합을 O(n)에 구했던 것을 2차원으로 구현하기 위해
		- (i, j) pair를 먼저 구하고, 해당 pair에서 [i][k] ~ [j][k] 값을 그때그때 for문을 통해 O(n)로 처리해준다.

		<시간복잡도>
		- i-th row, j-th row pair를 고르는데 O(n^2)
			- 해당 (i, j) pair별로 column을 읽는데 O(m)
				- certain column에서 i ~ j의 값을 더하는데 O(n)
		- 최종 시간 복잡도 - O(n^3*m) 이지만 n=m이라는 가정 하에 O(n^4)의 시간복잡도를 갖는다.
	*/

	// return 변수를 int형 범위 내 매우 작은 값으로 초기화
	int max_res = -(1e9 + 7);

	// (i, j) - pair에서 1dimension dynamic programming 을 하기 위한 배열
	vector<int> dp(m);
	for (int i = 0; i < n; ++i) {
		for (int j = i; j < n; ++j) {
			// (i, j) - pair에서의 local maximum value
			int tmp_max = 0;

			// 0번째 column의 i~j row 값의 합을 구하고 tmp_max로 설정
			for (int l = i; l <= j; ++l) dp[0] += num[l][0];
			tmp_max = dp[0];

			for (int k = 1; k < m; ++k) {
				// val 은 새롭게 부분배열의 우측에 놓일 항목들의 합 - num[i][k] ~ num[j][k]의 합
				int val = 0;
				for (int l = i; l <= j; ++l) val += num[l][k];

				// dp[k-1] 값이 0보다 작다면 최대가 되지 않으므로 val로 설정
				if (dp[k - 1] < 0) dp[k] = val;
				else dp[k] = dp[k - 1] + val;

				// (i, j) - pair 에서의 local maximum value 갱신
				tmp_max = max(tmp_max, dp[k]);
			}

			// max_res 값 갱신
			max_res = max(max_res, tmp_max);
		}
	}

	return max_res;
}

int O_3_algorithm(vector<vector<int> >& num, int n, int m) {
	/*
	O(n^3) 알고리즘 구현 함수 - 1dimension prefix sum 알고리즘 & Dynamic Programming 알고리즘

	Input : num - 2차원 배열, n - row size, m - column size
	Output : n x m 사이즈의 행렬에서 가장 큰 사각 부분행렬의 합
	Function :
		- n x m 사이즈의 행렬 내에서 column 별로 prefix sum 을 구현한 다음,
		- i-th row ~ j-th row pair를 기준으로 column을 증가시켜가며 dynamic programming 알고리즘을 이용하여 부분배열의 합을 갱신해간다.

		- 1차원(수열) 에서 dynamic programming을 이용해 부분 수열의 합을 O(n)에 구했던 것을 2차원으로 구현하기 위해
		- (i, j) pair를 먼저 구하고, 해당 pair에서 [i][k] ~ [j][k] 값을 column별 prefix sum으로 O(1)로 처리해준다.

		<시간복잡도>
		- column 별로 prefix sum 을 구현하는데 O(n)
		- i-th row, j-th row pair를 고르는데 O(n^2)
			- 해당 (i, j) pair별로 column을 읽는데 O(m)
		- 최종 시간 복잡도 - O(n + n^2*m) 이지만 n=m이라는 가정 하에 O(n^3)의 시간복잡도를 갖는다.
	*/

	// return 변수를 int형 범위 내 매우 작은 값으로 초기화
	int max_res = -(1e9 + 7);

	// column 별로 prefix sum 처리될 임시 배열 - 전처리 된다.
	vector<vector<int> > pxsum(num);
	// column 별 prefix sum 배열 전처리
	for (int i = 1; i < n; ++i) for (int j = 0; j < m; ++j) pxsum[i][j] = pxsum[i][j] + pxsum[i - 1][j];

	// (i, j) - pair 에서 column 별로 이동하며 부분 최대값을 저장
	// dp[k] = num[i][k] ~ dp[j][k]를 부분 배열의 가장 오른쪽 항목으로 넣을 때, 부분배열의 합의 최대값
	vector<int> dp(m);
	// (i, j) - pair
	for (int i = 0; i < n; ++i) {
		for (int j = i; j < n; ++j) {
			// (i, j) - pair에서의 local maximum value
			int tmp_max;

			// dp[0] 은 무조건 num[i][0] ~ num[j][0] 의 합 - i가 첫행이므로 i==0 일 때는 이전의 누적합이 없어 빼줄 값이 없다.
			dp[0] = i ? pxsum[j][0] - pxsum[i - 1][0] : pxsum[j][0];
			tmp_max = dp[0];
			for (int k = 1; k < m; ++k) {
				// val 은 새롭게 부분배열의 우측에 놓일 항목들의 합 - num[i][k] ~ num[j][k]의 합
				int val = i ? pxsum[j][k] - pxsum[i - 1][k] : pxsum[j][k];

				// dp[k-1] 값이 0보다 작다면 최대가 되지 않으므로 val로 설정
				if (dp[k - 1] < 0) dp[k] = val;
				else dp[k] = dp[k - 1] + val;

				// (i, j) - pair 에서 구한 부분배열들의 합 중 최대를 고른다.
				tmp_max = max(tmp_max, dp[k]);
			}

			// max_res 값 갱신
			max_res = max(max_res, tmp_max);
		}
	}

	return max_res;
}

int main(int argc, char* argv[]) {
	cin.tie(NULL); cout.tie(NULL);
	ios_base::sync_with_stdio(false);

	// result 파일 명 처리
	char result[50] = "result_";
	strcat(result, argv[1]);

	// input file open
	freopen(argv[1], "r", stdin);
	// output file open
	freopen(result, "w", stdout);

	int n, m, algoid, res;
	vector<vector<int> > num;

	// algorithm 번호를 int형으로 형변환해준다.
	algoid = atoi(argv[2]);

	// 행렬의 행, 열값을 입력받는다.
	cin >> n >> m;
	num.resize(n);

	// 행렬의 내부 데이터를 입력받는다.
	for (int i = 0; i < n; ++i) {
		num[i].resize(m);
		for (int j = 0; j < m; ++j) {
			cin >> num[i][j];
		}
	}

	// 알고리즘 실행 시작 시간 기록
	clock_t start_time = clock();

	// 알고리즘 번호별 함수 실행
	switch (algoid) {
	case O6algorithm:
		res = O_6_algorithm(num, n, m);
		break;
	case O4algorithm:
		res = O_4_algorithm(num, n, m);
		break;
	case O3algorithm:
		res = O_3_algorithm(num, n, m);
		break;
	}

	// 알고리즘 실행 끝 시간 기록
	clock_t end_time = clock();

	// output
	cout << argv[1] << '\n' << argv[2] << '\n' << n << '\n' << m << '\n' << res << '\n' << (((double)end_time - start_time) / CLOCKS_PER_SEC) * 1000 << '\n';

	return 0;
}
