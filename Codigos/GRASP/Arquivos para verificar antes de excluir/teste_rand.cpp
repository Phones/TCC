#include <bits/stdc++.h>
using namespace std;


void t()
{
		random_device rd;

	default_random_engine eng(rd());
    	uniform_real_distribution<float> distr(0, 1);
    	cout << "Num: " << distr(eng) << endl;
}


int main()
{

	cout << "Iniciando teste de número aleatorio entre 0 e 1" << endl;

	random_device rd;

	default_random_engine eng(rd());
    	uniform_real_distribution<float> distr(0, 1);

	for(int i = 0;i < 10;i++)
		t();


	return 0;
	cout << "Inicio do teste!" << endl;
	srand(time(0));

	while(1)
	{
		cout << "Num: " << rand() % 10 << endl;
	}
}
