#include <bits/stdc++.h>
using namespace std;
#define tam_vet 4

int vet[] = {22,33,10,11};

bool troco(int valor, int pos)
{
    if(!valor)
        return true;
    
    if(pos >= tam_vet)
        return false;

    if(valor >= vet[pos])
        return (troco(valor - vet[pos], pos) || troco(valor, pos + 1));

    return troco(valor, pos + 1);
}

int main()
{
    reverse(vet, vet + 4);

    cout << "Números: " << endl;
    for(int i = 0;i < tam_vet;i++)
	    cout << vet[i]  << " ";
    cout << endl;


    for(int i = 0;i < 4;i++)
    {
        cout << "Valor para i + 1:"<< i + 1 << " vet: " <<vet[i] << " " << troco(vet[i], i + 1) << endl;
    }
}


