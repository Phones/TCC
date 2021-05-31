#include <cstdio>
#include <algorithm>
#include <bits/stdc++.h>
using namespace std;

//-------------------------
#define MAXN 105
#define INFINITO 999999999

int n, m, q;
int mapa[MAXN][MAXN];
int distancia[MAXN][MAXN];
//-------------------------

int main(){
    
    scanf("%d %d %d", &n, &m, &q);
    
    // inicializaremos a matriz para infinito em todas as casas
    for(int i = 0;i < n;i++)
        for(int j = 0;j < n;j++)
            mapa[i][j] = INFINITO;
    
    for(int i = 0;i < m;i++){
        
        int x, y, tempo;
        scanf("%d %d %d", &x, &y, &tempo);
        
        // colocamos como recebendo esse valor mínimo para o caso
        // dois voos entre as mesmas cidades
        mapa[x][y] = min(mapa[x][y], tempo);
        mapa[y][x] = min(mapa[y][x], tempo);
    }
    
    // inicializamos as distâncias
    for(int i = 0;i < n;i++)
        for(int j = 0;j < n;j++)
            distancia[i][j] = mapa[i][j];
    
    // Algoritmo de Floyd-Warshall
    for(int k = 0;k < n;k++)
        for(int i = 0;i < n;i++)
            for(int j = 0;j < n;j++)
                distancia[i][j] = min(distancia[i][j], distancia[i][k] + distancia[k][j]);
    
    for(int i = 0;i < n;i++)
    {    
        for(int j = 0;j < n;j++)
            cout << distancia[i][j] << " ";

        cout << endl;
    }

    return 0;
}