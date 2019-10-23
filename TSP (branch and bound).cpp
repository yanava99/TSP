#include <fstream>
#include <list>
#include <vector>
#include <algorithm>

using namespace std;

const int INF = INT_MAX;
ifstream fin;
ofstream fout;

int n;
int* x;
int* y;
int** dist;

int record = 0;
int* record_cycle;
int* min_f;
int* min_s;


vector<int> indx;
int* curr_cycle;
bool* were;

void branch_and_bound(int number, int curr_length, int curr_bound)
{
	if (number == n)
	{
		curr_length += dist[0][curr_cycle[n - 1]];
		if (record > curr_length)
		{
			record = curr_length;
			for (int i = 0; i < n; ++i)
			{
				record_cycle[i] = curr_cycle[i];
			}
		}
	}
	else
	{
		vector<pair<int, int>> sorted_min_dist;
		for (int i = 0; i < n; ++i)
		{
			if (!were[i])
			{
				sorted_min_dist.emplace_back(-min_f[i] + dist[curr_cycle[number - 1]][i], i);
			}
		}
		sort(sorted_min_dist.begin(), sorted_min_dist.end());

		int k = sorted_min_dist.size();
		for (int i = 0; i < k; ++i)
		{
			int next_curr = sorted_min_dist[i].second;
			if (!were[next_curr])
			{
				int next_curr_bound = curr_bound;
				curr_length += dist[curr_cycle[number - 1]][next_curr];
				if (number == 1)
				{
					next_curr_bound -= (min_f[curr_cycle[number - 1]] + min_f[next_curr] + 1) / 2;
				}
				else
				{
					next_curr_bound -= (min_s[curr_cycle[number - 1]] + min_f[next_curr] + 1) / 2;
				}
				if (curr_length + next_curr_bound < record)
				{
					curr_cycle[number] = next_curr;
					were[next_curr] = true;
					branch_and_bound(number + 1, curr_length, next_curr_bound);
					curr_cycle[number] = -1;
					were[next_curr] = false;
				}
				curr_length -= dist[curr_cycle[number - 1]][next_curr];
			}
		}
	}
}



list<int>* tree;
int all = 1;
bool* mas_was;

void dfs(int curr)
{
	for (int i : tree[curr])
	{
		if (mas_was[i] == false)
		{
			mas_was[i] = true;
			record_cycle[all] = i;
			record += dist[record_cycle[all]][record_cycle[all - 1]];
			++all;
			dfs(i);
		}
	}
}


void tsp_mst()
{
	list<pair<int, pair<int, int>>> l;
	tree = new list<int>[n];
	for (int i = 1; i < n; ++i)
	{
		l.emplace_back(dist[i][0], make_pair(i, 0));
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
			if (it->first > dist[it->second.first][ch])
			{
				it->first = dist[it->second.first][ch];
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
	record_cycle[0] = 0;

	dfs(0);

	record += dist[0][record_cycle[n - 1]];
}



int main()
{
	fin.open("input.txt");
	fout.open("output.txt");

	fin >> n;

	x = new int[n];
	y = new int[n];
	min_f = new int[n];
	min_s = new int[n];
	dist = new int*[n];
	curr_cycle = new int[n];
	were = new bool[n];
	record_cycle = new int[n];

	for (int i = 0; i < n; ++i)
	{
		fin >> x[i] >> y[i];

		indx.push_back(i);

		curr_cycle[i] = -1;
		were[i] = false;
	}
	curr_cycle[0] = 0;
	were[0] = true;


	int first_bound = 0;
	for (int i = 0; i < n; ++i)
	{
		min_f[i] = INF;
		min_s[i] = INF;
		dist[i] = new int[n];
		for (int j = 0; j < n; ++j)
		{
			if (i == j)
			{
				dist[i][j] = INF;
			}
			else
			{
				dist[i][j] = abs(x[i] - x[j]) + abs(y[i] - y[j]);
				if (min_f[i] > dist[i][j])
				{
					min_s[i] = min_f[i];
					min_f[i] = dist[i][j];
				}
				else if (min_s[i] > dist[i][j])
				{
					min_s[i] = dist[i][j];
				}
			}
		}
		first_bound += min_f[i] + min_s[i];
	}
	first_bound = (first_bound + 1) / 2;

	tsp_mst();

	branch_and_bound(1, 0, first_bound);

	fout << record << '\n';
	for (int i = 0; i < n; ++i)
	{
		fout << record_cycle[i] + 1 << ' ';
	}
	fout << 1;

	fin.close();
	fout.close();
	return 0;
}
