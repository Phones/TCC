#include <bits/stdc++.h>

using namespace std;

#define maxn 500
#define instancia

const int inf = INT_MAX/2;

typedef pair<int ,int> ii;
typedef pair<int, ii> iii;
typedef pair<char, iii> ci;


struct facilidade
{
    int i; //Indice da facilidade
    int t; //Instante em que foi aberta
    int l; //Duracao

    facilidade() {}
    facilidade(int _i, int _t, int _l)
    {
        i = _i;
        t = _t;
        l = _l;
    }
    bool operator < (facilidade &another) const
    {
        return t < another.t;
    }
};

long int L;
int V, T, k, d[maxn][maxn], delta[maxn];
int pd[maxn], somaPrefixos[maxn][maxn], iSlot, **fa, **subs;
vector<int> D[maxn];
vector<iii> solucaoParcial;
vector<facilidade> solInicial[maxn], solAtual[maxn];

void leInstancia(const char *); //Le o conteudo de um arquivo de instancia
int pdSelecionaF(int t); //Programacao dinamica para selecionar as facilidades e as durações
void reconstroiSolucao(int t); //Armazena as facilidades e as durações da melhor solução encontrada pela programacao dinamica
void buscaLocalArya(vector<int> &Dt, int *solucao); //Realiza uma busca local para o k-median original
void solucaoInicial(); //Gera uma solucao inicial
int custos[500][500];
int avaliacao(vector<facilidade> *solucao);
int buscaLocal(vector<facilidade> *solucao); //Realiza uma busca local em uma solucao do leasing k-median, mudando uma facilidade por vez, mantendo sua duracao e instante de aberta
void geraVizinho(vector<facilidade> *solucao, int kV);

int main()
{

    int i, custoMelhor, custoAtual, numV;
    time_t tempoIni;
    vector<facilidade> *solucao, *melhorSol;

    //Leitura do arquivo da instância
    string arquivoInstancia = "Instancias Usadas\\inst21.txt";
    leInstancia(arquivoInstancia.c_str());
    printf("Lendo: %s\n", arquivoInstancia.c_str());
    printf("V: %d T: %d L: %ld k: %d\n", V, T, L, k);

    fa = new int *[maxn];
    subs = new int *[maxn];
    for(i=0; i<maxn; i++)
    {
        fa[i] = new int [maxn];
        subs[i] = new int [maxn];
    }



    tempoIni = time(NULL);
    solucaoInicial();
    puts("Iniciando a busca local da solucao inicial");
    melhorSol = solInicial;
    custoMelhor = buscaLocal(melhorSol);
    printf("Solucao inicial: %d\n", custoMelhor);
    return 0;
    solucao = solAtual;
	

    while(difftime(time(NULL), tempoIni) < 600)
    {
        numV = 1;
        while(numV <= 5 && difftime(time(NULL), tempoIni) < 600)
        {
            for(i=0; i<k; i++)
                solucao[i] = melhorSol[i];
            geraVizinho(solucao, numV);
            custoAtual = buscaLocal(solucao);
            printf("Otimo local: %d\n", custoAtual);

            if(custoAtual < custoMelhor)
            {
                swap(solucao, melhorSol);
                custoMelhor = custoAtual;
                numV = 1;
            }
            else
                numV++;
			printf("Melhor custo: %d\n\n", custoMelhor);
        }
    }
    printf("Melhor custo: %d\n\n", custoMelhor);



    for(i=0; i<maxn; i++)
    {
        delete[] fa[i];
        delete[] subs[i];
    }
    delete[] fa;
    delete[] subs;
    return 0;
}


void leInstancia(const char * inst)
{
    int i, j, l, aux;

    //freopen(inst, "r", stdin);

    //Quantidade de pontos, quantidade de instantes de tempo e quantidade de tipos de centro
    scanf("%d %d %ld %d", &V, &T, &L, &k);

    //Matriz de custos
    for(i=0; i<V; i++)
    {
        for(j=0; j<V; j++)
            scanf("%d", &d[i][j]);
        //d[i][i] = 0;
    }

    //Tipos de centro
    for(i=1; i<=L; i++)
        scanf("%d", &delta[i]);
    sort(delta+1, delta+L+1);

    //Conjunto de clientes (D)
    for(i=0; i<T; i++)
    {
        D[i].clear();
        scanf("%d", &l);

        for(j=0; j<l; j++)
        {
            scanf("%d", &aux);
            D[i].push_back(aux);
        }
    }
}


//Realiza uma busca local considerando um instante de tempo como uma instância do k-median
void buscaLocalArya(vector<int> &Dt, int *solucao)
{
    int i, j, l, m, aux, aux1, custoAtual, custoNovo;
    bool op;

    //Solucao inicial aleatoria
    for(i=0; i<k; i++)
    {
        j = -1;
        while(i!=j)
        {
            solucao[i] = rand()%V;
            for(j=0; j<i&&solucao[i]!=solucao[j]; j++);
        }
    }

    //Calcula o custo da solução inicial
    custoAtual = 0;
    for(i=0; i<(int)Dt.size(); i++)
    {
        fa[0][i] = solucao[0];
        aux = d[Dt[i]][solucao[0]];
        for(j=1; j<k; j++)
        {
            if(d[Dt[i]][solucao[j]] < aux)
            {
                aux = d[Dt[i]][solucao[j]];
                fa[0][i] = solucao[j];
            }
        }
        custoAtual += aux;
    }

    //Buscal local
    op = false;
    while(!op)
    {
        op = true;
        for(i=0; i<k && op; i++)
        {
            aux = solucao[i];
            for(j=0; j<V && op; j++)
            {
                //Calcula o custo de solucao - {solucao[i]} + {j}
                custoNovo = 0;
                solucao[i] = j;
                for(l=0; l<(int)Dt.size(); l++)
                {
                    if(fa[0][l] != aux)
                    {
                        if(d[Dt[l]][j] < d[Dt[l]][fa[0][l]])
                        {
                            custoNovo += d[Dt[l]][j];
                            subs[0][l] = j;
                        }
                        else
                        {
                            custoNovo += d[Dt[l]][fa[0][l]];
                            subs[0][l] = fa[0][l];
                        }
                    }
                    else
                    {
                        aux1 = d[Dt[l]][solucao[0]];
                        subs[0][l] = solucao[0];
                        for(m=1; m<k; m++)
                        {
                            if(d[Dt[l]][solucao[m]] < aux1)
                                aux1 = d[Dt[l]][solucao[m]], subs[0][l] = solucao[m];
                        }
                        custoNovo += aux1;
                    }
                }
                if(custoNovo < custoAtual)
                {
                    custoAtual = custoNovo;
                    op = false;
                    swap(fa[0], subs[0]);
                }
            }
            if(op)
                solucao[i] = aux;
        }
    }
}

//Programação dinâmica para selecionar as facilidades e as durações
int pdSelecionaF(int t)
{
    if(t>=T)
        return 0;
    if(pd[t] != -1)
        return pd[t];

    int ans = -1;
    for(int i=0; i<V; i++)
        for(int l=1; l<=L; l++)
            ans = max(ans, somaPrefixos[i][min(t+delta[l],T)]-somaPrefixos[i][t]+pdSelecionaF(t+delta[l]));
    return pd[t] = ans;
}

//Armazena as facilidades e as durações da melhor solução
void reconstroiSolucao(int t)
{
    if(t>=T)
        return;
    int i, l, iMax = 0, lMax, ans = -1;
    for(i=0; i<V; i++)
    {
        for(l=1; l<=L; l++)
        {
            if(ans < somaPrefixos[i][min(t+delta[l],T)]-somaPrefixos[i][t]+pdSelecionaF(t+delta[l]))
            {
                ans = somaPrefixos[i][min(t+delta[l],T)]-somaPrefixos[i][t]+pdSelecionaF(t+delta[l]);
                iMax = i, lMax = l;
            }
        }
    }
    solInicial[iSlot].push_back(facilidade(iMax,t,lMax));
    reconstroiSolucao(t+delta[lMax]);
}

//Gera a solucao inicial
void solucaoInicial()
{
    int i, j, l, t, delta1, a, aux;
    int solArya[k]; //Armazena a solucao da busca local de Arya

    //Busca local para cada instante de tempo
    for(i=0; i<T; i++)
    {
        buscaLocalArya(D[i], solArya);
        //for(int j = 0;j < k;j++)
          //  cout << solArya[j] << " " ;

        //return ;
        for(j=0; j<k; j++)
            somaPrefixos[solArya[j]][i+1]++;
    }
    
    for(int i = 0;i <= V;i++)
    {
		cout << "teste "<< i << ": ";
		for(int j = 1;j <= T;j++)
		{
			cout << somaPrefixos[i][j] << " ";
		}
		cout << endl;
	}

    //Pre-processa os resultados da busca local
    for(i=0; i<V; i++)
        for(j=1; j<=T; j++)
            somaPrefixos[i][j]+=somaPrefixos[i][j-1];

    for(int i = 0;i <= V;i++)
    {
        cout << "Depois "<< i << ": ";
        for(int j = 1;j <= T;j++)
        {
            cout << somaPrefixos[i][j] << " ";
        }
        cout << endl;
    }
    return ;
    for(iSlot=0; iSlot<k; iSlot++)
    {
        //Programacao dinamica para selecionar as facilidades e as duracoes
        memset(pd, -1, sizeof(pd));
        pdSelecionaF(0);
        reconstroiSolucao(0);

        //Atualiza a soma de prefixos
        for(j=0; j<(int)solInicial[iSlot].size(); j++)
        {
            t = solInicial[iSlot][j].t;
            delta1 = delta[solInicial[iSlot][j].l];
            a = solInicial[iSlot][j].i;

            for(l=t+1; l<t+delta1; l++)
                somaPrefixos[a][l] = somaPrefixos[a][l-1];
            aux = somaPrefixos[a][l]-somaPrefixos[a][l-1];
            for(; l<=T; l++)
                somaPrefixos[a][l] -= aux;
        }
    }
}

//Realiza uma busca local em uma solucao do leasing k-median, mudando uma facilidade por vez, mantendo sua duracao e instante de aberta
int buscaLocal(vector<facilidade> *solucao)
{
    int i, j, l, t, m, o, x, lim, custo, aux, novoCusto;
    bool op;

    facilidade f;
    custo = 0;

    //Calcula o custo da solucao
    for(j=0; j<(int)solucao[0].size(); j++)
    {
        f = solucao[0][j];
        lim = min(T, f.t+delta[f.l]);
        for(t=f.t; t<lim; t++)
        {
            for(m=0; m<(int)D[t].size(); m++)
            {
                fa[t][m] = f.i;
                custo += d[D[t][m]][f.i];
            }
        }
    }
    for(i=1; i<k; i++)
    {
        for(j=0; j<(int)solucao[i].size(); j++)
        {
            f = solucao[i][j];
            lim = min(T, f.t+delta[f.l]);
            for(t=f.t; t<lim; t++)
            {
                for(m=0; m<(int)D[t].size(); m++)
                {
                    if(d[D[t][m]][f.i] < d[D[t][m]][fa[t][m]])
                    {
                        custo = custo - d[D[t][m]][fa[t][m]] + d[D[t][m]][f.i];
                        fa[t][m] = f.i;
                    }
                }
            }
        }
    }


    op = false;
    while(!op)
    {
        op = true;
        for(i=0; i<k; i++)
        {
            for(j=0; j<(int)solucao[i].size() /*&& op*/; j++)
            {
                aux = solucao[i][j].i;
                //Calcula o custo de solucao - {solucao[i][j].i} + {l}
                for(l=0; l<V /*&& op*/; l++)
                {
                    novoCusto = custo;
                    solucao[i][j].i = l;
                    f = solucao[i][j];
                    lim = min(T, f.t+delta[f.l]);
                    for(t=f.t; t<lim; t++)
                    {
                        for(m=0; m<(int)D[t].size(); m++)
                        {

                            if(fa[t][m] != aux)
                            {
                                if(d[D[t][m]][fa[t][m]] > d[D[t][m]][l])
                                {
                                    subs[t][m] = l;
                                    novoCusto = novoCusto - d[D[t][m]][fa[t][m]] + d[D[t][m]][l];
                                }
                                else
                                    subs[t][m] = fa[t][m];
                            }
                            else
                            {
                                x = (upper_bound(solucao[0].begin(), solucao[0].end(), facilidade(0,t,0)) - solucao[0].begin()) -1;
                                subs[t][m] = solucao[0][x].i;

                                for(o=1; o<k; o++)
                                {
                                    x = (upper_bound(solucao[o].begin(), solucao[o].end(), facilidade(0,t,0)) - solucao[o].begin()) -1;
                                    if(d[D[t][m]][subs[t][m]] > d[D[t][m]][solucao[o][x].i])
                                        subs[t][m] = solucao[o][x].i;
                                }
                                novoCusto = novoCusto - d[D[t][m]][aux] + d[D[t][m]][subs[t][m]];
                            }
                        }
                    }
                    if(novoCusto < custo)
                    {
                        for(t=f.t; t<lim; t++)
                            swap(fa[t], subs[t]);
                        custo = novoCusto;
                        op = false;
                        aux = solucao[i][j].i;
                    }
                }
				solucao[i][j].i = aux;
            }
        }
    }

    return custo;
}

//Calcula o valor da solucao
int avaliacao(vector<facilidade> *solucao)
{
    int i, j, l, m, a, b, c, res;

    for(i=0; i<T; i++)
        for(j=0; j<V; j++)
            custos[i][j] = inf;

    for(m=0; m<k; m++)
    {
        for(i=0; i<(int)solucao[m].size(); i++)
        {
            a = solucao[m][i].i;
            b = delta[solucao[m][i].l];
            c = solucao[m][i].t;
            for(j=c; j<c+b && j<T; j++)
            {
                for(l=0; l<(int)D[j].size(); l++)
                    custos[j][D[j][l]] = min(custos[j][D[j][l]], d[D[j][l]][a]);
            }
        }
    }

    res = 0;
    for(i=0; i<T; i++)
    {
        for(j=0; j<V; j++)
        {
            if(custos[i][j]!=inf)
                res += custos[i][j];
        }
    }
    return res;
}

void geraVizinho(vector<facilidade> *solucao, int kV)
{
    int i, j, l, m, o, p, x, dur, pos, pos1, slot[kV], melhorE, melhorL, melhorQt;
    vector<facilidade> aux;
    kV = min(k, kV);

    //Seleciona os slots que serao alterados
    for(i=0; i<kV; i++)
    {
        slot[i] = rand()%k;
        j = -1;
        while(j!=i)
        {
            for(j=0; j<i && slot[j]!=slot[i]; j++);
            if(j!=i)
                slot[i] = rand()%k;
        }
    }

    for(i=0; i<kV; i++)
    {
        m = slot[i];
        pos1 = pos = rand()%((int)solucao[m].size()); //Facilidade que tera a duracao alterada
        l = (rand()%L)+1; //Nova duracao

        if(pos == (int)solucao[m].size()-1)
        {
            if(T-solucao[m][pos].t<=delta[1])
            {
                if(solucao[m].size()>1)
                    pos--, pos1--;
                else
                    continue;
            }
            else
            {
                l = rand()%(lower_bound(delta+1, delta+L+1, T-solucao[m][pos].t)-delta-1)+1;
                solucao[m][pos].l = l;
                solucao[m].push_back(facilidade(solucao[m][pos].i, solucao[m][pos].t+delta[l], L));
                continue;
            }
        }
        if(solucao[m][pos].l == l)
        {
            if(l>1)
                l--;
            else if(l<L)
                l++;
        }
        solucao[m][pos].l = l;

        while(pos < (int)solucao[m].size()-1)
        {
            x = solucao[m][pos].t + delta[solucao[m][pos].l];
            for(j=pos+1; j<(int)solucao[m].size() && min(T,solucao[m][j].t+delta[solucao[m][j].l]) <= x; j++);
            if(j == (int)solucao[m].size())
                break;
            dur = solucao[m][j].t+delta[solucao[m][j].l]-x;

            melhorE = 3*delta[L];
            
            

            for(o=min(L,upper_bound(delta+1, delta+L+1, dur)-delta); o >= 1; o--)
            {
                p = max(1,(int)round(dur/delta[o]));
                if(melhorE > abs(dur-p*delta[o]))
                {
                    melhorE = abs(dur-p*delta[o]);
                    melhorL = o;
                    melhorQt = p;
                }
            }

            for(o=0; o<melhorQt; o++)
            {
                aux.push_back(facilidade(solucao[m][j].i, x, melhorL));
                x += delta[melhorL];
            }
            pos = j;
            solucao[m][pos].t = x-delta[melhorL];
            solucao[m][pos].l = melhorL;
        }

        while((int)solucao[m].size()-1 != pos1)
            solucao[m].pop_back();

        for(j=0; j<(int)aux.size(); j++)
            solucao[m].push_back(aux[j]);
            
        x = solucao[m][(int)solucao[m].size()-1].t + delta[solucao[m][solucao[m].size()-1].l];
        while(x < T)
        {
            solucao[m].push_back(facilidade(solucao[m][(int)solucao[m].size()-1].i, x, L));
            x = solucao[m][(int)solucao[m].size()-1].t + delta[solucao[m][(int)solucao[m].size()-1].l];
        }
        aux.clear();
    }
}
