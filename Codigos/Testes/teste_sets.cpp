#include <bits/stdc++.h>
using namespace std;
typedef set <int> si;
typedef set <int>::iterator it_;


void imprime(si aux)
{
    it_ it;
    
    puts("---------------------------------");
    for (it = aux.begin();it != aux.end();it++)
    {
        cout << *it << endl;
    }
    puts("---------------------------------");
}




int main()
{
    
    si A, B, C, D;
    
    A.insert(1);
    A.insert(2);
    A.insert(4);
    A.insert(7);
    
    B.insert(1);
    B.insert(2);
    B.insert(8);
    B.insert(10);

    //imprime(A);

    set_intersection(A.begin(), A.end(), B.begin(), B.end(),
                 std::inserter(C, C.begin()));
    
    imprime(C);
    
    set_difference(A.begin(), A.end(), B.begin(), B.end(),
                 std::inserter(D, D.begin()));
                 
    imprime(D);
}

