#include <bits/stdc++.h>
#include "gurobi_c++.h"
using namespace std;







int main()
{
    /*
        quant_clientes -> V
        quant_intancias_tempo -> T
        quant_tipos_facilidades -> L
        K -> K
    */

    int quant_clientes, quant_intancias_tempo, quant_tipos_facilidades, K;


    scanf("%d %d %d %d",&quant_clientes,&quant_intancias_tempo,&quant_tipos_facilidades,&K);

    // Matriz de tamanho V X V, onde cada posição contém a distancia de i a j.
    int matriz[quant_clientes + 10][quant_clientes + 10];
    // Vetor que armazena os tipos de facilidades, ou seja o delta
    int  vet_tipos_facilidadesL[quant_tipos_facilidades];
    // Vector para armazenas os clientes que precesam ser atendidos em cada instante de tempo, formando o conjunto Dt
    vector <int> Dt[quant_intancias_tempo]; 

	
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

    // Create an environment
    puts("CRIANDO AMBIENTE VIRTIAL");
    GRBEnv env = GRBEnv();
    env.set("LogFile", "TesteGurobi.log");
    puts("CRIANDO MODELO");
    GRBModel model = GRBModel(env);
    puts("SETANDO NOME DO MODELO");
    model.set(GRB_StringAttr_ModelName, "Leasing K-median");
    model.getEnv().set(GRB_DoubleParam_TimeLimit, 60);
    env.start();

    model.update();
    GRBVar ***x, ***y;

    puts("CRIANDO X");
    // Adiciona as variaveis x e y ao modelo
    x = new GRBVar** [quant_intancias_tempo];
    for (int t = 0;t < quant_intancias_tempo;t++)
    {
        x[t] = new GRBVar *[quant_clientes];
        for (int i = 0;i < quant_clientes;i++)
        {
            x[t][i] = model.addVars((int)Dt[t].size(), GRB_BINARY);
            model.update();
            for (int j = 0;j < (int)Dt[t].size();j++)
            {
                ostringstream nome;
                nome << "x" << t << i << j;
                //x[t][i][j].set(GRB_DoubleAttr_Obj, 0);
                x[t][i][j].set(GRB_StringAttr_VarName, nome.str());
            }
        }
    }

    puts("CRIANDO y");
    // Adiciona as variaveis x e y ao modelo
    y = new GRBVar** [quant_intancias_tempo];
    for (int t = 0;t < quant_intancias_tempo;t++)
    {
        y[t] = new GRBVar *[quant_clientes];
        for (int i = 0;i < quant_clientes;i++)
        {
            y[t][i] = model.addVars(quant_tipos_facilidades, GRB_BINARY);
            model.update();
            for (int l = 0;l < quant_tipos_facilidades;l++)
            {
                ostringstream nome;
                nome << "y" << t << i << l;
                //y[t][i][l].set(GRB_DoubleAttr_Obj, 0);
                y[t][i][l].set(GRB_StringAttr_VarName, nome.str());
            }
        }
    }
    model.update();

   GRBLinExpr obj;
   puts("CRIANDO FUNÇÃO OBJETIVO");
   for(int t = 0;t < quant_intancias_tempo;t++)
   {
        for(int i = 0;i < quant_clientes;i++)
        {
            for(int j = 0;j < (int)Dt[t].size();j++)
            {
                //cout << t << " " << i << " " <<  j << " " <<  quant_clientes <<  endl;
                obj += matriz[i][j] * x[t][i][j];
            }
        }
   }

   puts("SETA FUNÇÃO OBJETIVO");
   model.setObjective(obj, GRB_MINIMIZE);
   puts("DEPOIS DE SETAR A FUNÇÃO");
   model.update();


   puts("CRIANDO PRIMEIRA RESTRIÇÃO");
   for (int t = 0;t < quant_intancias_tempo;t++)
   {
        for (int j = 0;j < (int)Dt[t].size();j++)
        {

            GRBLinExpr LE;
            for(int i = 0;i < quant_clientes;i++)
            {
                LE += x[t][i][j];
            }
            ostringstream name;
            name << "Assign[" << j << "]";
            model.addConstr(LE, GRB_EQUAL, 1, name.str());
        }
   }
   model.update();
   
   puts("CRIANDO SEGUNDA RESTRIÇÃO");
   int plt;
   for(int t = 0;t < quant_intancias_tempo;t++)
   {
       GRBLinExpr LE;
       for(int i = 0;i < quant_clientes;i++)
       {
            for(int l = 0;l < quant_tipos_facilidades;l++)
            {
               // plt = max(t - vet_tipos_facilidadesL[l] + 1, 1);
                if (vet_tipos_facilidadesL[l] <= t)
                        plt = t - vet_tipos_facilidadesL[l] + 1;
                    else
                        plt = 1;

                for(int t_ = plt;t_ < t; t_++)
                {
                    //cout << t_ << " " << i << " " << l << endl;
                    LE += y[t_][i][l];
                }
            }
       }
       ostringstream name;
       name << "Assign[" << t << "]";
       model.addConstr(LE,GRB_LESS_EQUAL, K,name.str());
   }
   model.update();

   puts("CRIANDO ULTIMA RESTRIÇÃO");
   for(int t = 0;t < quant_intancias_tempo;t++)
   {
        for(int i = 0;i < quant_clientes;i++)
        {
            for(int j = 0;j < (int)Dt[t].size();j++)
            {
                GRBLinExpr LE;
                for(int l = 0;l < quant_tipos_facilidades;l++)
                {
                    //plt = max(t - vet_tipos_facilidadesL[l] + 1, 1);
                    if (vet_tipos_facilidadesL[l] <= t)
                        plt = t - vet_tipos_facilidadesL[l] + 1;
                    else
                        plt = 1;

                    for(int t_ = plt;t_ < t; t_++)
                    {
                        //cout << t << " " << t_ << " " << i << " " << l << endl;
                        LE += y[t_][i][l];
                    }
                }
                ostringstream name;
                name << "Assign[" << t << "," << i << "," << "j" << "]";
                model.addConstr(LE,GRB_GREATER_EQUAL, x[t][i][j],name.str());
            }
        }
   }

   model.update();


   puts("INICIANDO OTIMIZAÇÃO");
   model.optimize();

   return 0;


    //for (int i )
    //GRBVar = new GRBVar* [2];



    // Cria as variaveis
    //GRBVar x = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, "x");
    //GRBVar y = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, "y")



    /*puts("====================================");
    for (int i = 0;i < quant_clientes;i++)
    {
        for (int j = 0;j < quant_clientes;j++)   
           printf("%d ",matriz[i][j]);
        
        cout << endl;    
    }
    puts("====================================");
    for (int i = 0;i < quant_tipos_facilidades;i++)
        printf("%d ",vet_tipos_facilidadesL[i]);
    puts("------------------------------------");
    for (int i = 0;i < quant_intancias_tempo;i++)
    {
        cout << "PQP: " << (int)Dt[i].size() << endl;
        for (int j = 0;j < (int)Dt[i].size();j++)   
           printf("%d ",Dt[i][j]);
        
        cout << endl;    
    }*/

    
        




	/*

    
    // Create an empty model
    GRBModel model = GRBModel(env);
    
    GRBVar x = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, "x");
    GRBVar y = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, "y");
    
    model.setObjective(x + y + 2 * z, GRB_MINIMIZE);
    */

    
    
}
