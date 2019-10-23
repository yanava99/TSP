#include <list>
#include <iterator>

using namespace std;


const int INF = INT_MAX;

int n;
int* mas_x;
int* mas_y;
list<int>* tree;

int all = 1;
bool* mas_was;
int* my_cycle;
int ans = 0;


void dfs(int curr)
{
	for (int i : tree[curr])
	{
		if (mas_was[i] == false)
		{
			mas_was[i] = true;
			my_cycle[all] = i;
			ans += abs(mas_x[my_cycle[all]] - mas_x[my_cycle[all - 1]]) + abs(mas_y[my_cycle[all]] - mas_y[my_cycle[all - 1]]);
			++all;
			dfs(i);
		}
	}
}



int main()
{
	errno_t res;
	FILE *fin, *fout;
	res = fopen_s(&fin, "input.txt", "r");
	res = fopen_s(&fout, "output.txt", "w");



	fscanf_s(fin, "%d", &n);

	mas_x = new int[n];
	mas_y = new int[n];
	for (int i = 0; i < n; ++i)
	{
		fscanf_s(fin, "%d", &mas_x[i]);
		fscanf_s(fin, "%d", &mas_y[i]);
	}

	list<pair<int, pair<int, int>>> l;
	tree = new list<int>[n];
	for (int i = 1; i < n; ++i)
	{
		l.emplace_back(abs(mas_x[i] - mas_x[0]) + abs(mas_y[i] - mas_y[0]), make_pair(i, 0));
	}
	list<pair<int, pair<int, int>>>::iterator choose;
	list<pair<int, pair<int, int>>>::iterator it;
	int ch;
	int short_length;
	int temp;
	for (int i = 1; i < n; ++i)
	{
		short_length = INF;
		for (it = l.begin(); it != l.end(); ++it)
		{
			if (it->first < short_length)
			{
				short_length = it->first;
				choose = it;
			}
		}
		ch = choose->second.first;
		tree[ch].emplace_back(choose->second.second);
		tree[choose->second.second].emplace_back(ch);
		l.erase(choose);
		for (it = l.begin(); it != l.end(); ++it)
		{
			temp = abs(mas_x[it->second.first] - mas_x[ch]) + abs(mas_y[it->second.first] - mas_y[ch]);
			if (it->first > temp)
			{
				it->first = temp;
				it->second.second = ch;
			}
		}
	}

	mas_was = new bool[n];
	mas_was[0] = true;
	for (int i = 1; i < n; ++i)
	{
		mas_was[i] = false;
	}
	my_cycle = new int[n];
	my_cycle[0] = 0;

	dfs(0);

	ans += abs(mas_x[0] - mas_x[my_cycle[n - 1]]) + abs(mas_y[0] - mas_y[my_cycle[n - 1]]);
	fprintf(fout, "%d\n", ans);
	for (int i = 0; i < n; ++i)
	{
		fprintf(fout, "%d ", my_cycle[i] + 1);
	}
	fprintf(fout, "1");

	return 0;
}