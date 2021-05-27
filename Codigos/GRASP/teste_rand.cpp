#include <bits/stdc++.h>
using namespace std;

int main()
{

	vector <pair<int,pair<int, int> > > a;

	a.push_back({1,{1,2}});

	cout << a[0].first << " " << a[0].second.first << " " << a[0].second.second << endl;

	return 0;
	cout << "Inicio do teste!" << endl;
	srand(time(0));

	while(1)
	{
		cout << "Num: " << rand() % 10 << endl;
	}
}
