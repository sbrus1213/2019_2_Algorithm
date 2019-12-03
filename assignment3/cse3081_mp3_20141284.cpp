#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <queue>
#include <map>
#include <algorithm>
#include <cstring>
#include <cstdio>
using namespace std;
const int COMPRESSION = 1;
const int DECOMPRESSION = 2;
#define pii pair<int, int>

// huffman tree를 구성하는 node
// 빈도수(cnt), x(해당문자), 자손의 포인터(left, right)를 갖는다.
struct tree_node {
	int cnt;
	char x;
	tree_node* left, * right;

	tree_node() :cnt(0), x(0), left(NULL), right(NULL) {}
	tree_node(pii p) :cnt(p.first), x(p.second), left(NULL), right(NULL) {}
	tree_node(tree_node* cp) :cnt(cp->cnt), x(cp->x), left(cp->left), right(cp->right) {}
};
// huffman tree를 만들기 위해 priority queue에 노드를 넣을 때, 
// tree_node의 빈도수 멤버인 cnt를 기준으로 정렬하기 위한 비교용 구조체
struct cmp {
	bool operator() (tree_node a, tree_node b) {
		return a.cnt > b.cnt;
	}
};

void make_huffman_tree(vector<pii >& cnt, tree_node** Tree) {
	/*
	Input : cnt - 128 character의 빈도수 배열, Tree - huffman tree를 담을 루트 노드
	Output : -
	Funtion :
		허프만 코드 알고리즘을 이용하여, 빈도수가 작은 문자부터 이진트리 를 만들어준다.
		빈도수가 작은 문자의 노드를 뽑기 위해 priority queue를 이용한다.
		빈도수가 가장 작은 문자의 노드 두개를 뽑고, 두 노드를 자식으로 하는 이진트리를 만든 다음에
		빈도수를 합쳐서 다시 prority queue에 넣어준다.
		priority queue 내부에 노드를 모두 합치면 종료한다.	 
	*/

	tree_node* temp;
	priority_queue<tree_node, vector<tree_node>, cmp> pq;

	// priority queue에 각 문자의 빈도수를 넣어준다.
	// EOF인 26과 나타나지 않은 문자는 제외한다.
	for (int i = 0; i < 128; ++i)
		if (cnt[i].first && i != 26)
			pq.push(*(new tree_node(cnt[i])));

	// priority queue 내부의 노드를 모두 합칠 때까지 반복한다.
	while (pq.size() > 1) {
		// 빈도수가 가장 작은 두개의 노드를 뽑고
		tree_node min1 = pq.top(); pq.pop();
		tree_node min2 = pq.top(); pq.pop();

		// 두개의 노드의 빈도수를 합치고
		// 이진트리를 다시 만들어서
		temp = new tree_node();
		temp->cnt = min1.cnt + min2.cnt;
		if (min1.cnt < min2.cnt) temp->left = new tree_node(&min1), temp->right = new tree_node(&min2);
		else temp->left = new tree_node(&min2), temp->right = new tree_node(&min1);

		// priority queue에 다시 넣어준다.
		pq.push(*temp);
	}

	// 인자로 받은 Tree에 node를 모두 합친 트리를 넣어준다.
	*Tree = new tree_node();
	**Tree = pq.top(); pq.pop();
	// 문자가 1종류만 등장한 경우, tree의 루트가 해당 문자의 노드로
	// binary string을 뽑아낼 때 정상적으로 작동하지 않기 때문에 예외처리해준다.
	if (!(*Tree)->left && !(*Tree)->right) {
		temp = new tree_node();
		temp->left = *Tree;
		*Tree = temp;
	}
}
void make_dic(map<char, string>& dic, tree_node* tree, string& s) {
	/*
	Input : dic - 문자별 binary string을 저장하기 위한 map , tree - 생성된 huffman tree , s - binary string을 위한 임시 string
	Output : - 
	Function :
		백트래킹을 이용하여 huffman tree의 루트부터 시작하여 DFS식으로 leaf까지 내려간다.
		모든 leaf에 접근했을 때, 해당 문자의 binary string이 생성되었으므로 dic에 저장해주고 재귀를 돌아간다.
	*/

	// 자손이 없는 경우, leaf이므로 dic에 저장해주고 종료한다.
	if (!tree->left && !tree->right) {
		dic[tree->x] = s;
		return;
	}
	// 왼쪽 자손이 존재하는 경우, binary string에 0을 추가하고 왼쪽자손에 대하여 재귀호출
	if (tree->left) {
		s.push_back('0');
		make_dic(dic, tree->left, s);
		s.erase(s.size() - 1, 1);
	}
	// 오른쪽 자손이 존재하는 경우, binary string에 1을 추가하고 오른쪽자손에 대하여 재귀호출
	if (tree->right) {
		s.push_back('1');
		make_dic(dic, tree->right, s);
		s.erase(s.size() - 1, 1);
	}
	return;
}

void compression() {
	/*
	Input : -
	Output : -
	Function :
		1. input file을 읽어 Input strig에 저장
		2. Input string에 문자들이 등장한 빈도수를 저장
		3. 2.에서 저장한 빈도수를 이용하여 huffman tree생성 및 각 문자별 binary string 설정
		4. Input string을 읽으며 Output binary string 생성
		5. Output binary string을 bit단위로 압축
	*/
	int len, i, j;
	string Input = "", Output = "", str_tmp;
	tree_node* Tree = NULL, * temp;
	vector<pii > cnt(128, pii(0, 0));
	map<char, string> dic;

	// file의 마지막이 올 때 까지 1줄 단위로 읽어 Input 에 쌓는다.
	while (!cin.eof()) {
		// file로부터 1줄 읽고 Input에 new line과 같이 저장
		getline(cin, str_tmp);
		Input += str_tmp + "\n";
		// 읽은 줄에 있는 문자들의 빈도수를 증가시켜준다.
		for (i = 0; i < str_tmp.size(); ++i) {
			char c = str_tmp[i];
			cnt[c].first++;
		}
		cnt['\n'].first++;
	}
	// 마지막에 임의로 추가한 new line을 지워준다.
	Input.erase(Input.size() - 1, 1);
	cnt['\n'].first--;
	for (i = 0; i < 128; ++i)
		cnt[i].second = i;

	// 빈도수 배열을 이용하여 huffman tree 생성
	make_huffman_tree(cnt, &Tree);

	// huffman tree를 이용하여 등장한 문자들에 대해 binary string 생성
	str_tmp.clear();
	make_dic(dic, Tree, str_tmp);

	// Input sting을 읽으며 Output binary string을 생성
	for (i = 0; i < Input.size(); ++i) {
		char c = Input[i];
		Output += dic[c];
	}

	// 압축된 Output binary string의 사이즈 출력
	len = Output.size();
	cout << len << ' ';

	// 각 문자의 빈도수를 output file에 출력
	for (i = 0; i < 128; ++i)
		cout << cnt[i].first << ' ';
	cout << '\n';

	// Output binary string 을 압축하기 위해 한 문자에 7개의 binary 문자를 담는다.
	// 8개 모두 담았을 때, 26이 되어버리면 EOF가 찍히기 때문에 정상적인 압축이 불가
	// 위와 같은 이유로 7개의 bit을 담고 26을 방지하기 위해 MSB를 1로 설정
	for (i = 0; i < len; i += 7) {
		char c = 1;
		for (j = 0; j < 7 && i + j < len; ++j) {
			c <<= 1;
			if (Output[i + j] == '0');
			else c += 1;
		}
		cout << c;
	}
}

void decompression() {
	/*
	Input : - 
	Output : -
	Function :
 		1. input file 로부터 압축을 풀어야 할 binary string의 사이즈와, 빈도수, 압축 string을 받는다.
		2. huffman tree 를 생성
		3. 압축 string을 읽으며 huffman tree를 이용하여 압축해제
	*/
	char c;
	int len, i, j;
	string tmp_str, Input_dump = "", Input = "", Output = "";
	tree_node* Tree = NULL, * temp;
	vector<pii > cnt(128, pii(0, 0));
	map<char, string> dic;

	// binary string의 사이즈 읽기
	cin >> len;
	// 각 문자의 빈도수 읽기
	for (int i = 0; i < 128; ++i) {
		cin >> cnt[i].first;
		cnt[i].second = i;
	}
	cin.ignore(2, '\n');

	// 빈도수로 huffman tree 읽기
	make_huffman_tree(cnt, &Tree);

	// file의 종료 전까지 한 줄 단위로 압축된 string을 읽기
	while (!cin.eof()) {
		getline(cin, tmp_str);
		Input_dump += tmp_str + "\n";
	}
	Input_dump.erase(Input_dump.size() - 1, 1);
	
	// 압축 스트링은 한 문자당 7bit의 binary 수를 담고 있기 때문에
	// 이를 binary string으로 만들기
	for (i = 0; i < len / 7; ++i) {
		c = Input_dump[i];
		for (int j = 6; j >= 0; --j)
			Input.push_back(c & 1 << j ? '1' : '0');
	}
	if (len % 7) {
		c = Input_dump[len / 7];
		for (int j = len % 7 - 1; j >= 0; --j)
			Input.push_back(c & 1 << j ? '1' : '0');
	}

	// huffman tree를 루트부터 읽으면서 자손이 없을 때까지 읽어 내려간다.
	// 자손이 없으면 해당 노드의 문자를 출력
	// 자손이 아직 있으면 0이면 좌측 자손, 1이면 우측자손으로 이동한다.
	i = 0; temp = Tree;
	while (1) {
		if (!temp->left && !temp->right) {
			Output.push_back(temp->x);
			temp = Tree;
		}
		if (i >= len) break;
		if (Input[i++] == '0') temp = temp->left;
		else temp = temp->right;
	}
	cout << Output;
}

int main(int argc, char* argv[]) {
	FILE* IFP;
	int flag;
	char file_name[200], result_filename[205];
	strcpy(file_name, argv[2]);
	strcpy(result_filename, argv[2]);

	// parameter 개수가 3개가 아닌 경우 error 문구 출력
	if (argc != 3) {
		cout << "[Error] - The number of Parameter Error\n[Usage] ./mp3_20141284 -c/d filename\n";
		return 0;
	}
	// option으로 -c 와 -d 가 아닌 다른 옵션이 들어오는 경우 error 문구 출력
	if (strcmp(argv[1], "-c") && strcmp(argv[1], "-d")) {
		cout << "[Error] - The option is only valid for c or d\n[Usage] ./mp3_20141284 -c/d filename\n";
		return 0;
	}

	IFP = freopen(file_name, "r", stdin);
	// input 파일이 없는 경우 error 출력
	if (IFP == NULL) {
		cout << "[Error] - Input file does not exit\n[Usage] ./mp3_20141284 -c/d filename\n";
		return 0;
	}

	// 옵션별 output file 이름 설정
	if (!strcmp(argv[1], "-c"))
		strcat(result_filename, ".zz"), flag = COMPRESSION;
	else if (!strcmp(argv[1], "-d"))
		strcat(result_filename, ".yy"), flag = DECOMPRESSION;

	freopen(result_filename, "w", stdout);

	// 옵션별 압축 및 압축 해제 수행
	if (flag == COMPRESSION) compression();
	else if (flag == DECOMPRESSION) decompression();
}
