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

struct tree_node {
	int cnt;
	char x;
	tree_node* left, * right;

	tree_node() :cnt(0), x(0), left(NULL), right(NULL) {}
	tree_node(pair<long long, int> p) :cnt(p.first), x(p.second), left(NULL), right(NULL) {}
	tree_node(tree_node* cp) :cnt(cp->cnt), x(cp->x), left(cp->left), right(cp->right) {}
};
struct cmp {
	bool operator() (tree_node a, tree_node b) {
		return a.cnt > b.cnt;
	}
};

void make_huffman_tree(vector<pair<long long, int> >& cnt, tree_node** Tree) {
	tree_node* temp;
	priority_queue<tree_node, vector<tree_node>, cmp> pq;
	for (int i = 0; i < 128; ++i)
		if (cnt[i].first && i != 26)
			pq.push(*(new tree_node(cnt[i])));
	while (pq.size() > 1) {
		tree_node min1 = pq.top(); pq.pop();
		tree_node min2 = pq.top(); pq.pop();

		temp = new tree_node();
		temp->cnt = min1.cnt + min2.cnt;
		if (min1.cnt < min2.cnt) temp->left = new tree_node(&min1), temp->right = new tree_node(&min2);
		else temp->left = new tree_node(&min2), temp->right = new tree_node(&min1);

		pq.push(*temp);
	}
	*Tree = new tree_node();
	**Tree = pq.top(); pq.pop();
	if (!(*Tree)->left && !(*Tree)->right) {
		temp = new tree_node();
		temp->left = *Tree;
		*Tree = temp;
	}
}
void make_dic(map<char, string>& dic, tree_node* tree, string& s) {
	if (!tree->left && !tree->right) {
		dic[tree->x] = s;
		return;
	}
	if (tree->left) {
		s.push_back('0');
		make_dic(dic, tree->left, s);
		s.erase(s.size() - 1, 1);
	}
	if (tree->right) {
		s.push_back('1');
		make_dic(dic, tree->right, s);
		s.erase(s.size() - 1, 1);
	}
	return;
}

void compression() {
	long long len, i, j;
	string Input = "", Output = "", str_tmp;
	tree_node* Tree = NULL, * temp;
	vector<pair<long long, int> > cnt(128, pair<long long, int>(0, 0));
	map<char, string> dic;

	while (!cin.eof()) {
		getline(cin, str_tmp);
		Input += str_tmp + "\n";
		for (i = 0; i < str_tmp.size(); ++i) {
			char c = str_tmp[i];
			cnt[c].first++;
		}
		cnt['\n'].first++;
	}
	Input.erase(Input.size() - 1, 1);
	cnt['\n'].first--;
	for (i = 0; i < 128; ++i)
		cnt[i].second = i;

	make_huffman_tree(cnt, &Tree);

	str_tmp.clear();
	make_dic(dic, Tree, str_tmp);

	if (dic['\n'] != "") cout << 1 << '\n';
	else cout << 0 << '\n';

	for (i = 0; i < 128; ++i)
		cout << cnt[i].first << ' ';
	cout << '\n';
	
	for (i = 0; i < Input.size(); ++i) {
		char c = Input[i];
		Output += dic[c];
	}

	len = Output.size();
	cout << len;
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
	char c;
	bool nlcheck;
	long long len, i;
	string tmp_str, Input_dump = "", Input = "", Output = "";
	tree_node* Tree = NULL, * temp;
	vector<pair<long long, int> > cnt(128, pair<long long, int>(0, 0));
	map<char, string> dic;

	cin >> nlcheck;

	for (int i = 0; i < 128; ++i) {
		cin >> cnt[i].first;
		cnt[i].second = i;
	}

	make_huffman_tree(cnt, &Tree);

	cin >> len;
	while (!cin.eof()) {
		getline(cin, tmp_str);
		Input_dump += tmp_str + "\n";
	}
	Input_dump.erase(Input_dump.size() - 1, 1);
	
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

	if (argc != 3) {
		cout << "[Error] - The number of Parameter Error\n[Usage] ./mp3_20141284 -c/d filename\n";
		return 0;
	}
	if (strcmp(argv[1], "-c") && strcmp(argv[1], "-d")) {
		cout << "[Error] - The option is only valid for c or d\n[Usage] ./mp3_20141284 -c/d filename\n";
		return 0;
	}

	IFP = freopen(file_name, "r", stdin);
	if (IFP == NULL) {
		cout << "[Error] - Input file does not exit\n[Usage] ./mp3_20141284 -c/d filename\n";
		return 0;
	}


	if (!strcmp(argv[1], "-c"))
		strcat(result_filename, ".zz"), flag = COMPRESSION;
	else if (!strcmp(argv[1], "-d"))
		strcat(result_filename, ".yy"), flag = DECOMPRESSION;

	freopen(result_filename, "w", stdout);

	if (flag == COMPRESSION) compression();
	else if (flag == DECOMPRESSION) decompression();
}
