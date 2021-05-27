#include <bits/stdc++.h>
using namespace std;
#define TAM_MAX 510
#define INF 123456789
typedef vector <int> vi;
typedef pair <int, int> ii;
typedef vector <pair<int, int> > vii;


int aux_K = 0;
// Matriz de tamanho V X V, onde cada posição contém a distancia de i a j.
int matriz[TAM_MAX][TAM_MAX];

// Vector para armazenas os clientes que precesam ser atendidos em cada instante de tempo, formando o conjunto Dt
vector <int> Dt[TAM_MAX];

/* Armazena as facilidades abertas, onde na posição first contém o custo que aquela facilidade tem pra atender
os cliente, e a segunda posição possui, o número da facilidade que está aberta.*/
int vetor_facilidades_abertas[TAM_MAX];

// Vetor que armazena os tipos de facilidades, ou seja o delta
int vet_tipos_facilidadesL[TAM_MAX];

// Fila auxiliar, para controlar
priority_queue <ii,vii,greater<ii> > fila;

/*
	GRASP APLICADO AO LEASING K-MEDIAN

    quant_clientes -> V
    quant_intancias_tempo -> T
    quant_tipos_facilidades -> L
    K -> K
*/

void inicia_vetor_facilidades(int quant_clientes)
{
	for(int i = 0;i <= quant_clientes;i++)
		vetor_facilidades_abertas[i] = -1;
}

// Retorna o número das facilidades que estão abertas
vi facilidades_abertas(int quant_clientes)
{
	vi auxiliar;
	for(int i = 0;quant_clientes;i++)
	{
		if(vetor_facilidades_abertas[i])
			auxiliar.push_back(i);
	}

	return auxiliar;
}

/* Decrementa um na duração das facilidades abertas a cada instante de tempo, quando o tempo de duração de uma
facilidade chega a zero, significa então que ela foi fechada, logo incrementa um nó k, para indicar que mais uma facilidade
pode ser aberta*/
void decrementa_um_instante(int &k, int quant_clientes)
{
	for(int i = 0;i < quant_clientes;i++)
	{
		cout << "vetor_facilidades_abertas[i]: " << vetor_facilidades_abertas[i] << endl;
		if(vetor_facilidades_abertas[i] != -1)
		{
			if(!vetor_facilidades_abertas[i]) vetor_facilidades_abertas[i] = -1, k++;
			else vetor_facilidades_abertas[i] -= 1;

			if(!vetor_facilidades_abertas[i]) vetor_facilidades_abertas[i] = -1, k++;
		}
	}
}


// Conecta o cliente que precisa ser atendido, a facilidade que possui menor custo a esse cliente
pair <int, int> menor_valor(int num_cliente, vi facilidades_abertas)
{
	int menor = INF;
	pair <int, int> facilidade_custo;
	for(int i = 0;i < (int)facilidades_abertas.size();i++)
	{
		// PASSA POR TODOS OS CUSTOS DE ATENDER ESSE CLIENTE, COM AS FACILIDADES QUE ESTÃO ABERTAS ATUALMENTE
		if(menor < matriz[num_cliente][facilidades_abertas[i]])
		{
			menor = matriz[num_cliente][facilidades_abertas[i]];
			facilidade_custo.first = menor;
			facilidade_custo.second = facilidades_abertas[i];
		}
	}

	return facilidade_custo;
}

void gera_solucao(vii facilidades_candidatas)
{
	
}

void busca_local()
{

}

// PEGA AS FACILIDADADES COM MENOR CUSTO, E ABRE ELAS
void abre_facilidades(int &k, vii auxiliar)
{
	int cont_auxilar = 0;
	for(;k > 0;k--)
	{
		cout << "ABRE FACILIDADES K: " << k << endl;
		int posicao_facilidade = auxiliar[cont_auxilar].second;
		int custo_facilidade = auxiliar[cont_auxilar].first;

		vetor_facilidades_abertas[posicao_facilidade] = vet_tipos_facilidadesL[0];
		cont_auxilar ++;
	}
}

bool ordena(pair<int, int> A, pair<int, int> B)
{
	return A.second < B.second;
}

vector <vii> cria_matriz_gulosa(int quant_clientes)
{
	vector <vii> final;
	// PERCORRE TODAS AS FACILIDADES
	for(int i = 0;i < quant_clientes;i++)
	{
		vii aux;
		/*
				Cria a linha da matriz, onde cada posição contem um pair, com o número da facilidade,
			e o custo dessa facilidade atender o cliente j
		*/
		for(int j = 0;j < quant_clientes;j++)
			aux.push_back({i, matriz[i][j]});
		
		// Adiciona a linha da matriz
		sort(aux.begin(), aux.end());
		cout << "FACILIDADE -> "<< i << ": ";
		for(int o = 0;o < (int)aux.size();o++)
		{
			cout << "[" << aux[o].first << "," << aux[o].second << "]";
		}
		cout << endl;
		final.push_back(aux);
	}

	// RETORNA A MATRIZ FINAL
	return final;
}

// GERA UM VETOR COM AS MELHORES FACILIDADES A SEREM ABERTAS
vii gera_vetor_candidatos(int quant_clientes,int t)
{
	vii v;
	//cout << "T: " << t << endl;
	// PERCORRE TODAS AS FACILIDADES
	for(int i = 0;i < quant_clientes;i++)
	{
		int soma = 0;
		for(int j = 0;j < (int)Dt[t].size();j++)
			//cout << i << "," << Dt[t][j] << " mat: " << matriz[i][Dt[t][j]] << endl,
			soma += matriz[i][Dt[t][j]];
		//cout << "soma: " << soma << endl;
		// ARMAZENA O CUSTO DA FACILIDADE PARA ATENDER TODOS OS CLIENTES DESSE INSTANTE, E A FACILIDADE
		v.push_back({soma, i});
	}

	sort(v.begin(), v.end());
	return v;
}


int retorna_custo(int t)
{
	/*Retorna 0, pois se o k é igual ao numero de clientes, basta abrir uma facilidade em cada cliente*/
	if(aux_K == (int)Dt[t].size())
		return 0;
}


int main()
{
	int quant_clientes, quant_intancias_tempo, quant_tipos_facilidades, K;

	scanf("%d %d %d %d",&quant_clientes,&quant_intancias_tempo,&quant_tipos_facilidades,&K);

	aux_K = K;
	// INICIA O VETOR DE FACILIDADES ABERTAS, COLANDO -1 EM TODAS AS POSIÇÕES
	inicia_vetor_facilidades(quant_clientes);

	vector <vii> matriz_gulosa;

    // Faz a leitura de todas os clientes
	for (int i = 0;i < quant_clientes;i++)
	    for (int j = 0;j < quant_clientes;j++)   
           scanf("%d",&matriz[i][j]);

    // Faz a leitura de todos os tipos de facilidades
    for (int i = 0;i < quant_tipos_facilidades;i++)
        scanf("%d",&vet_tipos_facilidadesL[i]);

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
    puts("!!! TODOS OS DADOS DE ENTRADA LIDOS COM SUCESSO !!!");

    /* Matriz que armaza as facilidas, que quando abertas, possuem o o somatorio do menor custo de atendimento, 
    para todos os clientes que precisam ser atendidos naquele instante de tempo*/
    cout << "QUANT CLIENTES: " << quant_clientes << endl;
    matriz_gulosa = cria_matriz_gulosa(quant_clientes);

    /* Nessa matriz, cada linha representa um instante de tempo, e em cada linha ira conter o custo de cada facilidade anteder
    todas os clientes desse instante, ordenado em ordem crescente, dessa maneira as que ocupam menos as primeiras posições, 
    são os que possuem menor custo*/
    vector <vii> matriz_canditos_por_instante;
    for(int t = 0;t < quant_intancias_tempo;t++)
    {
    	vii vetor_candidatos = gera_vetor_candidatos(quant_clientes, t);

    	matriz_canditos_por_instante.push_back(vetor_candidatos);
    	cout << "instante " << t << ": ";
    	for (int j = 0;j < (int)vetor_candidatos.size();j++)
    	{
    		cout << "[C: " << vetor_candidatos[j].first << ", P: " << vetor_candidatos[j].second << "]";
    	}

    	cout << endl;
    }
	
    // INICIA GRASP
    while (true)
    {
    	int soma_final = 0;
    	// INICIA FRASE CONSTRUÇÃO DA SOLUÇÃO
    	for(int t = 0;t < quant_intancias_tempo;t++)
    	{ // instante t
    		printf("INSTANTE %d\n", t);
    		puts("----------clientes---------------");
    		for(int i = 0;i < (int)Dt[t].size();i++)
    			cout << Dt[t][i] << " ";
    		puts("\n---------------------------------");
    	
    		// Abre facilidades
    		cout << "K ->  " << K << endl;
    		if(K) abre_facilidades(K, matriz_canditos_por_instante[t]);
    		cout << "K ->  " << K << endl;

    		soma_final += retorna_custo();
    		cout << "K =>  " << K << endl;
    		decrementa_um_instante(K, quant_clientes);
			cout << "K =>  " << K << endl;
    		//return 0;
    	}
    	return 0;
    }

}