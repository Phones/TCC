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

// GERA UM VETOR COM AS MELHORES FACILIDADES A SEREM ABERTAS
vii gera_vetor_candidatos(int quant_clientes,int t)
{
	vii v;
	////cout << "T: " << t << endl;
	// PERCORRE TODAS AS FACILIDADES
	for(int i = 0;i < quant_clientes;i++)
	{
		int soma = 0;
		for(int j = 0;j < (int)Dt[t].size();j++)
			////cout << i << "," << Dt[t][j] << " mat: " << matriz[i][Dt[t][j]] << endl,
			soma += matriz[i][Dt[t][j]];
		////cout << "soma: " << soma << endl;
		// ARMAZENA O CUSTO DA FACILIDADE PARA ATENDER TODOS OS CLIENTES DESSE INSTANTE, E A FACILIDADE
		v.push_back({soma, i});
	}

	sort(v.begin(), v.end());
	return v;
}

void gera_matriz_candidatos(int quant_intancias_tempo, int quant_clientes)
{
	for(int t = 0;t < quant_intancias_tempo;t++)
	{
		vii vetor_candidatos = gera_vetor_candidatos(quant_clientes, t);
		matriz_canditos_por_instante.push_back(vetor_candidatos);

		//cout << "instante " << t << ": ";
    	for (int j = 0;j < (int)vetor_candidatos.size();j++)
    	{
    		//cout << "[C: " << vetor_candidatos[j].first << ", P: " << vetor_candidatos[j].second << "]";
    	}
    	//cout << endl;
	}
}

// Gera uma facilidade que ainda não tenha sido aberta
int gera_facilidade_aleatoria(vi solucao)
{
	while(1)
	{
		int numero_random = rand() % quant_clientes;

		if(!solucao[numero_random])
			return numero_random;
	}
}	

int calcula_custo_solucao(int t, vi solucao)
{

	int custo_final = 0;
	vector <int> abertas;

	/*
	//cout << "AAAAAAAAAAAAAAAAAA: " << endl;
	for(int i = 0;i < (int)solucao.size();i++)
		//cout << solucao[i] << " ";
	//cout << endl;*/

	for(int i = 0;i < (int)solucao.size();i++)
		if(solucao[i])
			abertas.push_back(i);

	////cout << "TAMANHO DE T: " << t << endl;
	////cout << "Quantidade de clientes: " << (int)Dt[t].size() << endl;
	for(int j = 0;j < (int)Dt[t].size();j++)
	{
		int facilidade_conectada = -1;
		int cliente = Dt[t][j];
		int menor_custo_conexao = INF;

		////cout << "TAMANHO DO VETOR DE FACILIDADES ABERTAS: " << (int)abertas.size() << endl;
		// Conecta o cliente a solução mais proxima a ele
		for(int i = 0;i < (int)abertas.size();i++)
		{

			//linha();
			////cout << "Comparação: " << matriz[cliente][abertas[i]] << " " << menor_custo_conexao << endl;
			if(matriz[cliente][abertas[i]] < menor_custo_conexao)
			{
				menor_custo_conexao = matriz[cliente][abertas[i]],
				facilidade_conectada = abertas[i];
			}
			//linha();
		}
		////cout << "CUSTO QUE CHEGOU DEPOIS DO FOR: " << menor_custo_conexao << endl;
		//if(menor_custo == )
		//printf("Cliente %d -> facilidade -> %d -> Custo -> %d\n", cliente, facilidade_conectada, menor_custo_conexao);
		custo_final += menor_custo_conexao;
	}

	return custo_final;
}

/* Monta uma solução, abrindo os k melhores candidatos para aquele instante de tempo*/
vi monta_solucao_gulosa(int t, float alfa)
{
	// UTILIZADO PARA GERAR NUMERO ALEATORIO ENTRE 0 E 1
	random_device rd;
	default_random_engine eng(rd());
    uniform_real_distribution<float> distr(0, 1);

	vi vet_solucao;
	// Pega o vector que possui os canditatos desse instante de tempo
	vii candidatos = matriz_canditos_por_instante[t];
	////cout << "MONTANDO A SOLUÇÃO PARA O INSTANTE T: " << t << endl;
	//imprime_vector_pair(candidatos);
	// Inicia vector com zero
	for(int i = 0;i < quant_clientes;i++)
		vet_solucao.push_back(0);

	if(K_original == (int)Dt[t].size())
	{
		// COMO A QUANTIDADE DE CLIENTES É IGUAL A QUANTIDADE DE FACILIDADES, ABRE TODAS AS FACILIDADES EM CIMA DOS CLIENTES
		for(int j = 0;j < (int)Dt[t].size();j++)
		{
			int cliente = Dt[t][j];
			vet_solucao[cliente] = 1;
		}
	}
	else
	{
		// Inicia K facilidades de forma gulosa
		for(int i = 0;i < K_original;i++)
		{
			float num_random  = distr(eng);
			if(num_random < alfa)
			{
				int facilidade_random = gera_facilidade_aleatoria(vet_solucao);
				vet_solucao[facilidade_random] = 1;
			}
			else vet_solucao[candidatos[i].second] = 1;
		}
	}
	////cout << "SOLUÇÃO MONTADA: " << endl;
	//imprime_vector_int(vet_solucao);
	return vet_solucao;
}

vector <viii> cria_matriz_gulosa()
{
	vector <viii> final;
	// PERCORRE TODAS AS FACILIDADES
	for(int i = 0;i < quant_clientes;i++)
	{
		viii aux;
		/*
				Cria a linha da matriz, onde cada posição contem um pair, com o número da facilidade,
			e o custo dessa facilidade atender o cliente j
		*/
		for(int j = 0;j < quant_clientes;j++)
			aux.push_back({i, {j, matriz[i][j]}});
		
		// Adiciona a linha da matriz
		sort(aux.begin(), aux.end());
		//cout << "FACILIDADE -> "<< i << ": ";
		for(int o = 0;o < (int)aux.size();o++)
		{
			//cout << "[" << aux[o].first << "," << aux[o].second.first << ",{"<< aux[o].second.second <<"}]";
		}
		//cout << endl;
		final.push_back(aux);
	}

	// RETORNA A MATRIZ FINAL
	return final;
}

vi busca_local_k_median(vi sol, int custo_solucao, int t)
{	
	bool melhora = false;
	vi solucao = copia_vector(sol);
	//cout << "SOLUÇÃO DO K-MEDIAN ANTES DA TENTATIVA DE MELHORA: " << endl;
	imprime_vector_int(solucao);
	for(int i_ = 0;i_ < (int)sol.size();i_++)
	{
		if(sol[i_])
		{
			bool aux_m = false;
			int cont = 0,posicao_alterar = i_;

			printf("TESTANDO TROCA DE FACILIDADE, PARA A FACILIDADE I: %d\n", posicao_alterar);
			while(cont < 5)
			{
				int facilidade_random = gera_facilidade_aleatoria(solucao);
				
				//int i_ = (int)solucao.size() - 1;
				//for(;i_ >= 0 && !solucao[i_];i_--);

				// Abre facilidade nova
				solucao[facilidade_random] = 1;
				// Fecha facilidade antiga
				solucao[posicao_alterar] = 0;
				////cout << "VALOR DA FACILIDADE RANDOM: " << facilidade_random << endl;
				int custo_solucao_alterada = calcula_custo_solucao(t, solucao);
				if (custo_solucao_alterada < custo_solucao) 
				{
					//cout << "CUSTOS: " << custo_solucao << " - " << custo_solucao_alterada << endl;
					cont = 0, melhora = true;
					custo_solucao = custo_solucao_alterada;
					posicao_alterar = facilidade_random;
					linha();
					//cout << "AAAAAAAAAANTES: " << calcula_custo_solucao(t,sol) << endl;
					//cout << "DDEEEEPOIS: " << calcula_custo_solucao(t,solucao) << endl;
					linha();
				}
				else
				{
					cont += 1;
					// Abre facilidade nova
					solucao[facilidade_random] = 0;
					// Fecha facilidade antiga
					solucao[posicao_alterar] = 1;
				}
			}
			
			if(!aux_m)
			{
				cout << "-> N M" << endl;
			}
			else cout << "-> M" << endl;
		}
	}

	if(!melhora)
	{
		//cout << "NÃO MELHOROU" << endl;
		return sol;
	}

	//cout << "MELHOROU" << endl;
	return solucao;
}

void fragmenta_leasing_k_median(int quant_intancias_tempo, float alfa, vvi &matriz_resultados_Kmedian)
{
	// VECTOR QUE IRA ARMAZENAR AS COMBINAÇÕES
	//vector <facilidade_aberta> vector_auxiliar;
	vi valores_finais, valores_finais2;
	int soma_teste = 0, soma_teste_com_busca = 0;

    for(int t = 0;t < quant_intancias_tempo;t++)
    {
    	////cout << "----------------- INICIO DE UM LOOP NOVO, T: "<< t <<" -----------------" << endl;
    	////cout << "CLIENTES DESSE INSTANTE DE TEMPO: " << endl;
    	//imprime_vector_int(Dt[t]);
    	////cout <<"FIM LISTA DE CLIENTES"<< endl << endl;
    	vi solucao_Kmedian = monta_solucao_gulosa(t, alfa); // Solução para o k median do intante t

    	//cout << "DEPOIS DA FUNÇÃO MONTA SOLUÇÃO GULOSA: "<< endl;
    	//imprime_vector_vector_int(matriz_resultados_Kmedian,"P ");

    	int valor = calcula_custo_solucao(t, solucao_Kmedian); 
    	////cout << "#### CUSTO ANTES DA BUSCA -> " << t << ": [" << valor << "]" << endl;
    	soma_teste += valor;
    	valores_finais.push_back(valor);

    	vi solucao_com_busca;
    	if(valor)
    	{
    		// FUNÇÃO DE DENTRO DO IF TEMPORARIAMENTE COMENTADAS

    		// Apenas faz a busca caso, o numero de facilidades seja diferente do numero de clientes nesse instante
	    	// APLICA A BUSCA LOCAL NO K-MEDIAN DESSA SOLUÇÃO
	    	//solucao_com_busca = busca_local_k_median(solucao_Kmedian, valor, t);
	    	//int valor2 = calcula_custo_solucao(t, solucao_com_busca);
	    	////cout << "#### CUSTO DEPOIS DA BUSCA -> " << t << ": [" << valor2 << "]" << endl;
	    	//soma_teste_com_busca += valor2;
	    	//valores_finais2.push_back(valor2);
	    }
	    else
	    {
	    	soma_teste_com_busca += valor;
	    	valores_finais2.push_back(valor);
	    	solucao_com_busca = solucao_Kmedian;
	    }

    	// ARMAZENA AS SOLUÇÕES NO VETOR FINAl
    	matriz_resultados_Kmedian[t] = solucao_Kmedian;
    	//matriz_resultados_Kmedian.push_back(solucao_com_busca);

    }
    ////cout << "Valor final da soma teste: " << soma_teste << endl;
    //imprime_vector_int(valores_finais);
    ////cout << "Valor final da soma teste com busca: " << soma_teste_com_busca << endl;
    //imprime_vector_int(valores_finais2);
}

void prenche_quant_facilidades_abertas(vvi &matriz_resultados_Kmedian)
{
	for(int i = 0;i < (int)matriz_resultados_Kmedian.size();i++)
		for(int j = 0;j < (int)matriz_resultados_Kmedian[i].size();j++)
			if(matriz_resultados_Kmedian[i][j])
				quant_facilidades_abertas[j] ++;
}

void remove_da_solucao(int ini, int fim, int i, vvi &matriz_resultados_Kmedian)
{
	////cout << "ANTES DE REMOVER DO INTERVALO" << endl;
	//imprime_vector_vector_int(matriz_resultados_Kmedian);
	////cout << "DEPOIS DE REMOVER DO INTERVALO" << endl;

	for(int t = ini; t < fim;t++)
	//	if(matriz_resultados_Kmedian[t][i])
			matriz_resultados_Kmedian[t][i] = 0;

	//imprime_vector_vector_int(matriz_resultados_Kmedian);

	//sleep(6000);
}

void recupa_para_a_solucao(int ini, int fim, int i, vvi &matriz_resultados_Kmedian)
{
	// Decrementa o limite do fim, para não passar do limite maximo do vetor
	if(fim == quant_intancias_tempo) fim --;

	////cout << "ANTES DE REMOVER DO INTERVALO" << endl;
	//imprime_vector_vector_int(matriz_resultados_Kmedian);
	////cout << "DEPOIS DE REMOVER DO INTERVALO" << endl;

	for(int t = ini; t <= fim;t++)
			matriz_resultados_Kmedian[t][i] ++;

	//imprime_vector_vector_int(matriz_resultados_Kmedian);

	//sleep(6000);
}

ii retorna_quant_intervalo(int ini, int fim, vvi &matriz_resultados_Kmedian)
{
	// Decrementa o limite do fim, para não passar do limite maximo do vetor
	//if(fim == quant_intancias_tempo) fim --;


	//cout << "ANTES DE VERIFICAR NA FUNÇÃO RETORNA QUANT" << endl;
	//imprime_vector_vector_int(matriz_resultados_Kmedian,"t: ");


	ii final;
	//cout << "Valor ini e fim: " << ini << " " << fim << endl;
	int vet_aux[quant_clientes] = {};
	for(int t = ini;t < fim;t++)
		for(int i = 0;i < (int)matriz_resultados_Kmedian[t].size();i++)
			{////cout<<"t: " << t << " i: " << i << endl;
			 if(matriz_resultados_Kmedian[t][i])
				vet_aux[i] ++;}


	//linha();
	//puts(" ============= VECTOR EM UMA LINHA =============");
	//imprime_vetor_int_uma_linha(vet_aux, quant_clientes);
	//linha();

	//cout << "ANTES DE COLETAR MAIOR" << endl;
	int maior = -100000;
	for(int i = 0;i < quant_clientes;i++)
	{
		//cout << "COMPARAÇÃO: " << vet_aux[i] << " > " << maior << " i: " << i << endl;
		if(vet_aux[i] > maior)
		{
			maior = vet_aux[i];
			final.first = i;
			final.second = maior;
		}
	}

	//cout << ">>>>> FUNÇÃO RETORNA QUANT: " << maior << " i: " << final.first << " " << final.second << endl;
	//cout << "FINAL DA FUNÇÃO QUE RETORNA A QUANT INTERVALO" << endl;
	return final;
}

int pd_escolha_tempo_facilidade(int t, vvi &matriz_resultados_Kmedian)
{
	//cout << "t: " << t << endl;
	if(t >= quant_intancias_tempo)
		return 0;

 	int &ans = DP[t];

 	if(ans != -1)
 		return ans;

 	ans = 0;
	for(int l = 0;l < quant_tipos_facilidades;l++)
	{
		//cout << "vet_tipos_facilidadesL[l]: " << vet_tipos_facilidadesL[l] << endl;
		// Pega o min entra o intervalo o tempo maximo de instantes, para não ultrapassar
		int intervalo = min(quant_intancias_tempo ,t + vet_tipos_facilidadesL[l]);
		// Pega a facilidade que mais aparece nesse instante de tempo, retornando a facilidade, e quantidade que ela aparece
		//cout << "ANTES DA FUNÇÃO DE RETORNA QUANT INTERVALO" << endl;
		ii facilidade_max = retorna_quant_intervalo(t, intervalo, matriz_resultados_Kmedian);
		//P[t][intervalo] = facilidade_max.second;
		//cout << "FACILIDADE: " << facilidade_max.first << " INTERVALO: " << t << " a " << intervalo << endl;
		//cout << "VALOR DO INTERVALO: " << intervalo << endl;
		//marca_intervalo_facilidade(matriz_leasing, t, intervalo, facilidade_max.first);

		//remove_da_solucao(t, intervalo, facilidade_max.first);	
		
		int solucao = pd_escolha_tempo_facilidade(intervalo, matriz_resultados_Kmedian) + facilidade_max.second;
		//cout << "YYYYYYYYYYYYYYYYYY" << endl;
		if(solucao > ans)
		{
			ans = solucao;
			// Armazena a facilidade que foi escolhida, e o intervalo na qual ela vai ficar aberta
			armazena_facilidade_escolhida[t].first = facilidade_max.first;
			armazena_facilidade_escolhida[t].second = vet_tipos_facilidadesL[l];
		}


		//recupa_para_a_solucao(t, intervalo, facilidade_max.second);
	}
	return ans;
}

int calcula_custo_solucao_leasing_vvF(vvF teste)
{
	// Nessa matriz cada linha, representa um instante de tempo
	vvi auxiliar(quant_intancias_tempo);

	for(int kk = 0;kk < K_original;kk++)
		for(int i = 0;i < (int)teste[kk].size();i++)
		{
			int ini = teste[kk][i].t, fim = teste[kk][i].l;
			for(int t = ini;t < fim;t++)
				auxiliar[t].push_back(teste[kk][i].i);
		}

	int soma = 0;
	for(int t = 0;t < quant_intancias_tempo;t++)
	{
		for(int j = 0;j < (int)Dt[t].size();j++)
		{
			int cliente = Dt[t][j];
			int menor_custo = INF;
			for(int i = 0;i < (int)auxiliar[t].size();i++)
			{
				int facilidade_aberta = auxiliar[t][i];
				if(menor_custo > matriz[cliente][facilidade_aberta])
					menor_custo = matriz[cliente][facilidade_aberta];
			}
			soma += menor_custo;
		}
	}

	return soma;
}

void limpa_pd()
{
	for(int t = 0;t < quant_intancias_tempo;t++) DP[t] = -1;
}

void marca_intervalo_facilidade(int ini, int fim, int facilidade, int l)
{
	/*Marca na matriz, posição da linha representa a faciliade, e na linha 
	vai conter um vetor, em que representa, os instantes, pelo qual essa faciliade
	ficou aberta*/
	////cout << "FACILIDADE: " << facilidade << " INI: " << ini << " FIM: " << fim << endl;
	for(int i = ini;i < fim;i++) matriz_leasing_atual[i][facilidade] = l;
	//{
		////cout << "### Marcando posição: " << i << " " << facilidade << endl, 
		//matriz_leasing_atual[i][facilidade] = 1;
	//}
}


/* Remove as facilidades que foram escolhidas desse instante de tempo,
e logo após de remover, coloca essas facilidades em um vetor de struct, 
para assim armazenar a solução.*/
vector <facilidade_aberta> remove_cont(vvi &matriz_resultados_Kmedian)
{
	int index = 0;

	vector <facilidade_aberta> solucao_para_k;

	while(index < quant_intancias_tempo)
	{
		int facilidade = armazena_facilidade_escolhida[index].first;
		int fim = min(quant_intancias_tempo, index + armazena_facilidade_escolhida[index].second);
		remove_da_solucao(index, fim, facilidade, matriz_resultados_Kmedian);
		// i t l
		facilidade_aberta aux(facilidade, index, fim);
		// Marca a solução que foi encontrada, em uma matriz de int com int, para ficar mais facil de calcular a solução
		//puts("MARCAR NO INTERVALO");
		//sleep(6000);
		marca_intervalo_facilidade(index, fim, facilidade, armazena_facilidade_escolhida[index].second);
		////cout << "DEPOIS DE MARCAR NO INTERVALO" << endl;
		solucao_para_k.push_back(aux);
		index = fim;
	}
	return solucao_para_k;
}


vvF testa_combinacoes_tempo_facilidade(vvi &matriz_resultados_Kmedian)
{
	vector <vector<facilidade_aberta> > solucao_final_leasing;
	puts("INICIO DA PD");
	for(int k = 0;k < K_original;k++)
	{
		limpa_pd();
		//cout << "ANTES DA FUNÇÃO DA PD!" << endl;
		int x = pd_escolha_tempo_facilidade(0, matriz_resultados_Kmedian);
		//cout << "DEPOIS DA FUNÇÃO DE PD, PARA O K -> " << k << endl;
		//imprime_vector_vector_int(matriz_resultados_Kmedian, "facilidade");
		linha();

		//imprime_vector_int(DP);
		//linha();
		//imprime_vector_pair(armazena_facilidade_escolhida);	
		//linha();
		//imprime_vector_vector_int(matriz_resultados_Kmedian,  "t");
		solucao_final_leasing.push_back(remove_cont(matriz_resultados_Kmedian)); // Remove as facilidades escolhidas, e salva as mesmas
		//puts("DEPOIS DA REMOÇÃO");
		//imprime_vector_vector_int(matriz_resultados_Kmedian, "t");
		// Armazena a solução para o k atual*/

	}

	return solucao_final_leasing;
}

bool troco(int valor, int pos)
{
    if(!valor)
        return true;
    
    if(pos >= quant_tipos_facilidades)
        return false;

    if(valor >= vet_tipos_facilidadesL[pos])
        return (troco(valor - vet_tipos_facilidadesL[pos], pos) || troco(valor, pos + 1));

    return troco(valor, pos + 1);
}


// Remove as facilidades que podem ser substituidas, por combianções de outras facilidades
vi remove_facilidades_substituiveis()
{
	vi novo_vetor_facilidades;

	// Para por todas facilidades, da maior para a menor
	for(int l = 0;l < quant_tipos_facilidades;l++)
		if(!troco(vet_tipos_facilidadesL[l], l + 1))
			novo_vetor_facilidades.push_back(vet_tipos_facilidadesL[l]);
	
	// Retorna o novo vetor de facilidades
	sort(novo_vetor_facilidades.begin(), novo_vetor_facilidades.end());
	return novo_vetor_facilidades;
}
// --------------------------------------- TEMPORARIO --------------------------------------------
vvi remove_da_solucao2(int ini, int fim, int i, vvi solucao)
{
	for(int t = ini; t < fim;t++) solucao[t][i] = 0;

	return solucao;
}

vvi marca_intervalo_facilidade2(int ini, int fim, int facilidade,vvi solucao,int l)
{
	for(int i = ini;i < fim;i++) solucao[i][facilidade] = l;

	return solucao;
}
// -----------------------------------------------------------------------------------------------

vvF converte_vvi_em_vvF(vvi solucao)
{
	vvF final(K_original);

	for(int t = 0;t < quant_intancias_tempo;t++)
	{
		int k_k = 0;
		for(int i = 0;i < quant_clientes;i++)
		{
			if(solucao[t][i])
			{
				// Achou onde a facilidade foi aberta, coleta informações
				int i_ = i, t_ = t,l = t, l_ = solucao[t][i];
				int fim = min(quant_intancias_tempo, l_ + t_);

				for(;l < fim && solucao[l][i];l++);

				solucao = remove_da_solucao2(t_,l,i_,solucao);

				facilidade_aberta auxx(i_, t_, l);
				final[k_k].push_back(auxx);

				k_k ++;
			}
		}
	}

	return final;
}
 
// Calcula o custo de uma solução de leasing k-median
int calcula_custo_solucao_leasing(vvi solucao)
{
    int soma = 0;

    for(int t = 0;t < quant_intancias_tempo;t++)
    {
    	int custo_ = calcula_custo_solucao(t, solucao[t]);
    	soma += custo_;
    }

    return soma;
}

vi get_facilidades_fechadas_intervalo(vvi solucao, int ini, int fim)
{
	int aux[quant_clientes] = {};
	// Armazena o número das facilidades fechadas nesse intervalo de tempo
	vi facilidades_fechadas;//= inicia_vector_int_zerado(quant_clientes);

	for(int t = ini;t < fim;t++)
		for(int i = 0;i < quant_clientes;i++)
			if(solucao[t][i])
				aux[i] ++;

	for(int i = 0;i < quant_clientes;i++)
		if(!aux[i])
			facilidades_fechadas.push_back(i);

	//imprime_vetor_int_uma_linha(aux, quant_clientes);
	//cout << " FACILIDADES FECHADAS NO INTERVALO: " << ini << " " << fim << endl;
	//imprime_vector_int_um_linha(facilidades_fechadas);
	return facilidades_fechadas;
}

// Gera vizinho testando todas as possiveis trocas para uma facilidade
vvi gera_vizinho_baseado_2opt_ANTIGO(vvi solucao, vvF solucao_em_struct)
{
	// Calcula o custo da solução que será usada para gerar os vizinhos
	int custo_solucao_inicial = calcula_custo_solucao_leasing(solucao);

	/* Essa função seleciona um k por vez, e testa todas as possiveis trocas trocas para esse k,
	e então retorna a melhor encontrada*/
	for(int k_ = 0;k_ < K_original;k_++)
	{
		// Faz a escolha randomica de uma facilidade, para ser alterada
		int num_random = rand() % (int)solucao_em_struct[k_].size();
		facilidade_aberta aux = solucao_em_struct[k_][num_random];

		int fim_intervalo = min(quant_intancias_tempo, aux.l);
		//cout << " FACILIDADE QUE FOI ESCOLHIDA: " << aux.i << " fim intervalo: " << fim_intervalo <<endl;
		// Coleta as opções de facilidades que podem ser abertas no intervalo, da facilidade atual que vai ser fechada
		vi facilidades_fechadas = get_facilidades_fechadas_intervalo(solucao, aux.t, fim_intervalo);
		
		//Fecha a facilidade que foi escolhida
		int melhor_facilidade = aux.i;
		solucao = remove_da_solucao2(aux.t, fim_intervalo, aux.i, solucao);

		int _t_ = aux.t;
		// Percorre todas as facilidades que está fechada
		for(int i = 0;i < (int)facilidades_fechadas.size();i++)
		{
			int nova_facilidade = facilidades_fechadas[i];
			// Abre a nova facilidade que será testada
			solucao = marca_intervalo_facilidade2(aux.t, fim_intervalo, nova_facilidade, solucao,aux.l - aux.t);
			if(custo_solucao_inicial > calcula_custo_solucao_leasing(solucao))
				melhor_facilidade = nova_facilidade;

			// Fecha facilidade, para testar outras facilidades
			solucao = remove_da_solucao2(aux.t, fim_intervalo, nova_facilidade, solucao);
		}
		// Abre a facilidade que obteve melhor desempenho
		solucao = solucao = marca_intervalo_facilidade2(aux.t, fim_intervalo, melhor_facilidade, solucao,aux.l - aux.t);
	}
	return solucao;
}

//****************************************************************
// Gera vizinho testando todas as possiveis trocas para uma facilidade
vvF gera_vizinho_baseado_2opt(vvF solucao_em_struct)
{
	// Calcula o custo da solução que será usada para gerar os vizinhos
	int custo_solucao_inicial = calcula_custo_solucao_leasing_vvF(solucao_em_struct);

	/* Essa função seleciona um k por vez, e testa todas as possiveis trocas trocas para esse k,
	e então retorna a melhor encontrada*/
	for(int k_ = 0;k_ < K_original;k_++)
	{
		// Faz a escolha randomica de uma facilidade, para ser alterada
		int num_random = rand() % (int)solucao_em_struct[k_].size();
		facilidade_aberta aux = solucao_em_struct[k_][num_random];

		int fim_intervalo = min(quant_intancias_tempo, aux.l);
		//cout << " FACILIDADE QUE FOI ESCOLHIDA: " << aux.i << " fim intervalo: " << fim_intervalo <<endl;
		// Coleta as opções de facilidades que podem ser abertas no intervalo, da facilidade atual que vai ser fechada
		vi facilidades_fechadas = get_facilidades_fechadas_intervalo(matriz_leasing_atual, aux.t, fim_intervalo);
		
		//Fecha a facilidade que foi escolhida
		int melhor_facilidade = aux.i;
		matriz_leasing_atual = remove_da_solucao2(aux.t, fim_intervalo, aux.i, matriz_leasing_atual);

		int _t_ = aux.t;
		// Percorre todas as facilidades que está fechada
		for(int i = 0;i < (int)facilidades_fechadas.size();i++)
		{
			int nova_facilidade = facilidades_fechadas[i];
			// Abre a nova facilidade que será testada
			//solucao = marca_intervalo_facilidade2(aux.t, fim_intervalo, nova_facilidade, solucao,aux.l - aux.t);
			solucao_em_struct[k_][num_random].i = nova_facilidade;
			if(custo_solucao_inicial > calcula_custo_solucao_leasing_vvF(solucao_em_struct))
				melhor_facilidade = nova_facilidade;

			// Fecha facilidade, para testar outras facilidades
			//solucao = remove_da_solucao2(aux.t, fim_intervalo, nova_facilidade, solucao);
		}
		// Abre a facilidade que obteve melhor desempenho
		//solucao = marca_intervalo_facilidade2(aux.t, fim_intervalo, melhor_facilidade, solucao,aux.l - aux.t);
		solucao_em_struct[k_][num_random].i = melhor_facilidade;
		matriz_leasing_atual = marca_intervalo_facilidade2(aux.t, fim_intervalo, melhor_facilidade, matriz_leasing_atual,aux.l - aux.t);
	}
	return solucao_em_struct;
}

vvF gera_vizinhos_por_troca_de_facilidades_aleatorias(vvF solucao_em_struct)
{
	// A primeira etapa de geração de vizinhança é alterar as facilidades, sem alterar as durações
	for(int k_ = 0;k_ < K_original;k_++)
	{
		int num_random = rand() % (int)solucao_em_struct[k_].size();

		// Faz a escolha randomica de uma facilidade, para ser alterada
		facilidade_aberta aux = solucao_em_struct[k_][num_random];
		int facilidade_aleatoria = gera_facilidade_aleatoria(matriz_leasing_atual[aux.t]);

		solucao_em_struct[k_][num_random].i = facilidade_aleatoria;
	}

	return solucao_em_struct;
}

// Retorna um tipo de facilidade aleatoria, difirente da que foi passada
int seleciona_tipo_facilidade_aleatoriamente(int l)
{
	int tam_ = (int)vet_tipos_facilidadesL.size();

	while(1)
	{
		int num_random = rand() % tam_;
		if(vet_tipos_facilidadesL[num_random] != l)
			return vet_tipos_facilidadesL[num_random];
	}
}

/*Essa função gera uma vizinhança, selecionando uma facilidade de cada k, então troca o tipo, dessa facilidade,
por outro tipo de facilidade, em que esse outro tipo de facilidade tb é escolhido de forma aleatoria*/
vvF gera_vizinho_por_troca_tipo_facilidade(vvF solucao_em_struct)
{
	int tam = (int)vet_tipos_facilidadesL.size();
	// Verifica se o tamanho do vetor de facilidade tem tamanho 1, caso sim retorna o mesmo tipo de facilidade
	if(tam == 1)
		return solucao_em_struct;

	// A primeira etapa de geração de vizinhança é alterar as facilidades, sem alterar as durações
	for(int k_ = 0;k_ < K_original;k_++)
	{
		// Seleciona de maneira aleatoria a facilidade que vai ter seu tipo alterado
		int num_random = rand() % (int)solucao_em_struct[k_].size();
		int t_ = solucao_em_struct[k_][num_random].t,
			i_ = solucao_em_struct[k_][num_random].i,
			l_ = solucao_em_struct[k_][num_random].l;

		int tipoL_facilidade_aleatoria = seleciona_tipo_facilidade_aleatoriamente(l_);

		// Ajusta solução para nova facilidade
		if(tipoL_facilidade_aleatoria > l_)
		{

		}
	}

	return solucao_em_struct;
}

int gera_facilidade_aleatorio_para_swap(int i, vvF solucao_em_struct, int tam, int k_)
{
	while(1)
	{
		int num_random = rand() % tam;
		if(solucao_em_struct[k_][num_random].i != i)
			return num_random;
	}
}


vvF gera_vizinhos_swap(vvF solucao_em_struct)
{
	// A primeira etapa de geração de vizinhança é alterar as facilidades, sem alterar as durações
	for(int k_ = 0;k_ < K_original;k_++)
	{
		int tamm = (int)solucao_em_struct[k_].size();
		int num_random = rand() % tamm;

		int posicao = gera_facilidade_aleatorio_para_swap(solucao_em_struct[k_][num_random].i, solucao_em_struct, tamm, k_);

		//cout << "teste antes: " << solucao_em_struct[k_][posicao].i << " - " << solucao_em_struct[k_][num_random].i << endl;

		int troca_ = solucao_em_struct[k_][posicao].i;

		// Troca as facilidades
		solucao_em_struct[k_][posicao].i = solucao_em_struct[k_][num_random].i;
		solucao_em_struct[k_][num_random].i = troca_;
		
		//cout << "teste depois: " << solucao_em_struct[k_][posicao].i << " - " << solucao_em_struct[k_][num_random].i << endl;
	}

	return solucao_em_struct;
}


/*
Ideias para a busca local:
	-> Mudar a duração
	-> Reduzir a duração + Estrutura
	-> Função que abre uma facilidade
*/

/* Recebe uma solução corrente, e tenta melhora-lá, quando não for possivel melhora - lá mais, retorna a solução*/
pair <vvF, int> busca_local_VND_leasing(vvF solucao_em_struct, long int tempoIni)
{

	vvF solucao_vizinha;
	int limite = 3, cont = 0;
	pair <vvF, int> solucao_final_busca;
	// Armazena o custo da solução que chega na função
	int custo_solucao = calcula_custo_solucao_leasing_vvF(solucao_em_struct);

	while(cont < limite && (time(NULL) - tempoIni) < 600)
	{
		//cout << "CONT DENTRO DO VND: " << cont << endl;
		//Encontra o melhor vizinho
		if(cont == 0)
			solucao_vizinha = gera_vizinho_baseado_2opt(solucao_em_struct);
		else if(cont == 1)
			solucao_vizinha = gera_vizinhos_swap(solucao_em_struct);		
		else
			solucao_vizinha = gera_vizinhos_por_troca_de_facilidades_aleatorias(solucao_em_struct);
			
		int custo_solucao_vizinha = calcula_custo_solucao_leasing_vvF(solucao_vizinha);
		if(custo_solucao_vizinha < custo_solucao)
			solucao_em_struct = solucao_vizinha, custo_solucao = custo_solucao_vizinha, cont = 0;
		else cont ++;

		//printf("Custo vizinho solução busca: %d\n", custo_solucao_vizinha);
		//printf("Custo melhor solução busca: %d\n", custo_solucao);
	}
	// Armazena a melhor solução e o custo da mesma, encontrado pela busca. Para retornar para o GRASP
	solucao_final_busca = {solucao_em_struct, custo_solucao};
	cout << "FIM VND" << endl;
	return solucao_final_busca;
}

vvF gera_solucao(float alfa)
{
	vvi matriz_resultados_Kmedian = inicia_matriz_int_vector_zerada(quant_intancias_tempo, quant_clientes);

	//imprime_vector_vector_int(matriz_resultados_Kmedian,"U ");

	puts("INICIALIZA A MATRIZ DE VECTOR ZERADA");
    matriz_leasing_atual = inicia_matriz_int_vector_zerada(quant_intancias_tempo, quant_clientes);

    /* Fragmenta a instancia do leasing k-median, em T k-medians, dessa forma seleciona as melhores facilidades por instante,
   	por ter transformado em um k-median, então o Delta, está sendo tratado como 1*/
   	puts("INICIANDO FRAGMENTAÇÃO DO LEASING K-MEDIAN");
    fragmenta_leasing_k_median(quant_intancias_tempo, alfa, matriz_resultados_Kmedian);

    //puts("MATRIZ DE RESULTADOS K-MEDIAN's");
    // Imprimi a solução que foi encontrada para o leasing k-median fragmentado
    //imprime_vector_vector_int(matriz_resultados_Kmedian, "t");

    puts("PREENCHE QUANTIDADE DE FACILIDADES ABERTAS");
    prenche_quant_facilidades_abertas(matriz_resultados_Kmedian);

    puts("TESTA COMBINAÇÕES DE TEMPOS DE FACILIDADES");
    vvF solucao_em_struct = testa_combinacoes_tempo_facilidade(matriz_resultados_Kmedian);

    //puts("SOLUÇÃO LEASING EM MATRIZ DE VECTOR DE INT, INT:");
    //imprime_vector_vector_int(matriz_leasing_atual, "t");

    //imprime_vector_vector_facilidade_aberta(converte_vvi_em_vvF(matriz_leasing_atual));

    cout << ">>>>> SOLUÇÃO GERADA <<<<<" << endl;
   	//cout << "CUSTO TOTAL DESSE SOLUÇÃO: " << calcula_custo_solucao_leasing(matriz_leasing_atual) << endl;

   	return solucao_em_struct;
}


// float calcula_media(vf vet_Q)
// {
// 	//float media = ;    
// }

// float selecionaum_alfa(vf vetor_alfas)
// {

// }




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

    puts("!!! TODOS OS DADOS DE ENTRADA LIDOS COM SUCESSO !!!");

    /* Pré processa as informações do tipo de facilidade, para remover facilidades
	 que podem ser subistituiveis, combinando outros tipos de facilidades*/

    linha();
    puts("INICIA TRATAMENTO FACILIDADES");
    sort(vet_tipos_facilidadesL.begin(), vet_tipos_facilidadesL.end());
  	reverse(vet_tipos_facilidadesL.begin(), vet_tipos_facilidadesL.end());
  	puts("TIPOS DE FACILIDADES ORDENADOS");
  	puts("ANTES:");
  	imprime_vector_int(vet_tipos_facilidadesL);
  	vet_tipos_facilidadesL = remove_facilidades_substituiveis();
  	quant_tipos_facilidades = (int)vet_tipos_facilidadesL.size();
  	puts("DEPOIS: ");
  	imprime_vector_int(vet_tipos_facilidadesL);
  	linha();

    matriz_gulosa = cria_matriz_gulosa();

    /* Nessa matriz é armazenados em ordem crescente por custo, as facilides junto com o custo de cada uma
    atender todos os clients que precisam ser atendidos naquele instante de tempo, supondo um k = 1. Cada linha
    representa um instante de tempo.*/
    gera_matriz_candidatos(quant_intancias_tempo, quant_clientes);


    // -------------------------------------------- GRASP --------------------------------------------
   	
   	float alfa = 0.45;
   	int cont_it = 0;
   	int custo_melhor_solucao = INF;
   	pair <vvF, int> melhor_solucao;
   	vvi melhor_solucao_em_matriz;

   	/*/ Mecanismos para fazer o GRASP reativo
   	int m = 10;
   	float conjunto_alfas[] = {0.2,0.5,0.6,0.9,0.55,0.82,0.44,0.7,0.33,0.15};

   	vector <float> vetor_probabilidadesP, Ms;
   	for(int i = 0;i < m;i++)
   		vetor_probabilidadesP.push_back((1/m)),
   		Ms.push_back(0);

   	int Mi = , iterator_atualizacao = 0;
   	//*/

   	long int tempoIni = time(NULL);
   	puts("INICIANDO GRASP");
   	while((time(NULL) - tempoIni) < 600)
   	{
   		printf("------------------------------ ITERAÇÃO NÚMERO -> %d ------------------------------\n", cont_it);
   		// Gera uma solução para o leasing k-median
   		vvF solucao_gerada = gera_solucao(alfa);

   		puts("INICIO VND");
   		// Aplica a busca local na solução gerada
   		pair <vvF, int> solucao_busca = busca_local_VND_leasing(solucao_gerada, tempoIni);

   		int custo_solucao_gerada_com_busca = solucao_busca.second;
   		if(custo_solucao_gerada_com_busca < custo_melhor_solucao)
   			melhor_solucao = solucao_busca, 
   			custo_melhor_solucao = custo_solucao_gerada_com_busca, 
   			melhor_solucao_em_matriz = matriz_leasing_atual;

   		printf("Custo da solucao atual ---> [%d]\n", custo_solucao_gerada_com_busca);
   		printf("Custo da melhor solução --> [%d]\n", custo_melhor_solucao);

   		cont_it ++;
   	}
   	
   	cout << "CUSTO DA MELHOR SOLUÇÃO: " << custo_melhor_solucao << endl;
    //puts("SOLUÇÃO LEASING EM MATRIZ DE VECTOR DE INT, INT:");
    //imprime_vector_vector_int(matriz_leasing_atual, "t");
    // -----------------------------------------------------------------------------------------------
   	return 0;
}