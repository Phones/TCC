#include <unistd.h>
#include <bits/stdc++.h>
using namespace std;
#define TAM_MAX 510
#define INF 123456789
typedef vector <int> vi;
typedef vector <vi> vvi;
typedef vector <float> vf;
typedef pair <int, int> ii;
typedef pair <int, ii> iii;
typedef vector <pair<int, int> > vii;
typedef vector <iii> viii;
typedef vector<vii> mat_vii;

// Armazena a organização da solução em cada instante
struct facilidade_aberta
{
	int i, t, l;
	facilidade_aberta(int i, int t, int l)
	{
		this->i = i;
		this->t = t;
		this->l = l;
	}
};

typedef vector<vector<facilidade_aberta> > vvF;

int K_original = 0, K;

// Matriz de tamanho V X V, onde cada posição contém a distancia de i a j.
int matriz[TAM_MAX][TAM_MAX];

// Vector para armazenas os clientes que precesam ser atendidos em cada instante de tempo, formando o conjunto Dt
vector <int> Dt[TAM_MAX];

/* Armazena as facilidades abertas, onde na posição first contém o custo que aquela facilidade tem pra atender
os cliente, e a segunda posição possui, o número da facilidade que está aberta.*/
int vetor_facilidades_abertas[TAM_MAX];

// Vetor que armazena os tipos de facilidades, ou seja o delta
vi vet_tipos_facilidadesL;

/* Nessa matriz, cada linha representa um instante de tempo, e em cada linha ira conter o custo de cada facilidade anteder
todas os clientes desse instante, ordenado em ordem crescente, dessa maneira as que ocupam menos as primeiras posições, 
são os que possuem menor custo*/
mat_vii matriz_canditos_por_instante;

vvi new_matriz_candidatos_por_instante;

vector <viii> matriz_gulosa;

// Fila auxiliar, para controlar
priority_queue <ii,vii,greater<ii> > fila;

int quant_clientes, quant_intancias_tempo, quant_tipos_facilidades;

// Armazena as resoluções que foram encontradas tratando cada posição como um k-median normal
//vector <vi> matriz_resultados_Kmedian;

// Armazena quantas vezes cada facilidade foi aberta no decorrer dos instantes de tempo
vector <int> quant_facilidades_abertas;

vii armazena_facilidade_escolhida;

//Vetor para armazenar os valores da DP
vi DP;

// Matriz que ira armazenar a solucao inicial do leasing k-median
vector <vi> matriz_leasing_atual;


/*
	GRASP APLICADO AO LEASING K-MEDIAN 

    quant_clientes -> V
    quant_intancias_tempo -> T
    quant_tipos_facilidades -> L
    K -> K
*/

// ----------------------------------- FUNÇÕES PARA TESTE -------------------------------------------



void imprime_vector_int(vi teste)
{
	cout << endl;
	for(int i = 0;i < (int)teste.size();i++)
		cout << "i: [" << i << "]-> {" << teste[i] << "}" << endl;
	cout << endl;
}

void imprime_vector_int_um_linha(vi teste)
{
	cout << endl;
	for(int i = 0;i < (int)teste.size();i++)
		cout <<  teste[i] << " ";
	cout << endl;
}

void imprime_vetor_int_uma_linha(int teste[], int tam)
{
	for(int i = 0;i < tam;i++)
		cout << teste[i] << " ";
	cout << endl;
}

void imprime_vector_vector_int(vector <vi> teste, string texto)
{
	for(int i = 0;i < (int)teste.size();i++)
	{
		cout << texto <<": " << i << " -> ";
		for(int j = 0;j < (int)teste[i].size();j++)
			cout << teste[i][j] ;//<< " ";

		cout << endl;
	}
}

void imprime_vector_pair(vii teste)
{
	cout << endl;
	for(int i = 0;i < (int)teste.size();i++)
		cout << "i: [" << i << "]-> {" << teste[i].first << ", " <<teste[i].second << "}" << endl;
	cout << endl;
}

void imprime_matriz_vector_int(vector <vi> teste)
{
	for(int i = 0;i < (int)teste.size();i++)
	{
		for(int j = 0;j < (int)teste[i].size();j++)
			cout << teste[i][j] << " ";

		cout << endl;
	}
}

void imprime_matriz_leasing(vector <vi> M)
{
	for(int i = 0;i < quant_clientes;i++)
	{
		cout << "i: ";
		for(int j = 0;j < quant_intancias_tempo;j++)
			cout << M[i][j] << " ";

		cout << endl;
	}
}

vi copia_vector(vi A)
{
	vi B;
	for(int i = 0;i < (int)A.size();i++)
		B.push_back(A[i]);

	return B;
}

// Cria uma matriz de vector de vector de int, inicializada com 0
vector <vi> inicia_matriz_int_vector_zerada(int ini, int fim)
{
	//cout << "INTERVALO QUE CHEGOU AQUI: "<<ini << " " << fim << endl;
	vector <vi> aux_f;
	aux_f.clear();
	for(int i = 0;i < ini;i++)
	{
		vi aux__;
		aux__.clear();
		for(int j = 0;j < fim;j++)
			aux__.push_back(0);

		aux_f.push_back(aux__);
	}

	return aux_f;
}

vi inicia_vector_int_zerado(int tam)
{
	vi aux;
	for(int i = 0;i < tam;i++)
		aux.push_back(0);

	return aux;
}

void linha()
{
	//cout << "----------------------------------------------------" << endl;
}

void imprime_vector_vector_facilidade_aberta(vvF teste)
{
	for(int k = 0;k < (int)teste.size();k++)
	{
		linha();
		cout << "K -> " << k << endl;
		for(int i = 0;i < (int)teste[k].size();i++)
			cout << "facilidade: " << teste[k][i].i << " t: " 
				<< teste[k][i].t << " l: " << teste[k][i].l << " ,";
		cout << endl;
	}
}

// --------------------------------------------------------------------------------------------------

void inicia_vetores(int quant_clientes)
{
	for(int i = 0;i < quant_intancias_tempo;i++)
		armazena_facilidade_escolhida.push_back({-1, -1});

	for(int i = 0;i <= quant_clientes;i++)
		vetor_facilidades_abertas[i] = -1;

	for(int t = 0;t <= quant_clientes;t++)
		quant_facilidades_abertas.push_back(0);

	for (int i = 0;i < quant_intancias_tempo;i++)
		DP.push_back(-1);
}

int main()
{
	// Inicia a seed para a função de gerar numeros aleatorios
	srand(time(0));

	scanf("%d %d %d %d",&quant_clientes,&quant_intancias_tempo,&quant_tipos_facilidades,&K);

	K_original = K;
	// INICIA O VETOR DE FACILIDADES ABERTAS, COLANDO -1 EM TODAS AS POSIÇÕES
	inicia_vetores(quant_clientes);

	// Inicia um vetor que indica quantas facilidades estãoa abertas em cada instante
	int vet_intervalo_facilidades[quant_intancias_tempo] = {};

    // Faz a leitura de todas os clientes
	for (int i = 0;i < quant_clientes;i++)
	    for (int j = 0;j < quant_clientes;j++)   
           scanf("%d",&matriz[i][j]);

    // Faz a leitura de todos os tipos de facilidades
    for (int i = 0;i < quant_tipos_facilidades;i++)
    {
    	int l;
    	scanf("%d",&l);
    	vet_tipos_facilidadesL.push_back(l);
    }

    int quant_clientes_instante_atual, aux;
    // Percorre todos os instantes de tempo
    for (int i = 0;i < quant_intancias_tempo;i++)
    {
        scanf("%d",&quant_clientes_instante_atual);
        // Para cada linha, temos uma linha todos os clientes que precisam ser atendidos naquele instante de tempo
        for (int j = 0;j < quant_clientes_instante_atual;j++)
        {
            scanf("%d",& aux);
            Dt[i].push_back(aux);
        }

    }

    //puts("!!! TODOS OS DADOS DE ENTRADA LIDOS COM SUCESSO !!!");

    int menor = INF;
    for (int i = 0;i < quant_tipos_facilidades;i++)
    	if (vet_tipos_facilidadesL[i] < menor)
    		menor = vet_tipos_facilidadesL[i];

   	if (menor == 1)
   		puts("Sim");
   	else
   		printf("Não -> %d", menor),
   		puts("");
   		

}