import os
import sys
from time import sleep

valor_alfa = str(sys.argv[1])
numero_teste = str(sys.argv[2])
numero_instancia = str(sys.argv[3])


print("Valor de alfa: ", valor_alfa)
print("Valor de numero_teste: ", numero_teste)
print("Valor de numero_instancia: ", numero_instancia)

# ---- CRIA PASTA PARA ARMAZENAR O RESULTADO ---------
caminho_pasta_resultados = '/home/paulohenrique/Área de Trabalho/Volta pro TCC/TCC/AmbienteDeTeste/Resultados/'
lista_pastas = os.listdir(caminho_pasta_resultados)
os.mkdir(caminho_pasta_resultados + str(len(lista_pastas)))
# ----------------------------------------------------

# MONTA CAMINHO BASE ATÉ A PASTA DO TESTE
caminho_base_pasta_teste = '"/home/paulohenrique/Área de Trabalho/Volta pro TCC/TCC/AmbienteDeTeste'
caminho_base_pasta_teste_programa = caminho_base_pasta_teste + "/Codigos/teste" + numero_teste
print("caminho base: ", caminho_base_pasta_teste)

# ADICIONA CHAMADA DO PYTHON, O ARQUIVO QUE SERÁ EXECUTADO E OS PARAMETROS
comando_pronto = "make run alfa=" + valor_alfa + " num_instancia=" + numero_instancia 

# ADICIONA CAMINHO PARA ARQUIVO ONDE SAIDA SERA ARMAZENADA
comando_pronto = comando_pronto + ' > "' + caminho_pasta_resultados + str(len(lista_pastas)) + '/saida_terminal_' + str(numero_teste) + '.txt"'

# DIRECIONA SAIDA DO ERRO PARA UM ARQUIVO DE TEXTO SEPARADO
comando_pronto = comando_pronto + ' 2> "' + caminho_pasta_resultados + str(len(lista_pastas)) + '/erros.txt"'

print("Comando pronto: ", comando_pronto)

print("INICIANDO EXECUÇÃO DO TESTE NUMERO: ", numero_teste)
os.system(comando_pronto)

print("Fim da execução do teste " + numero_teste + " deste terminal!")
#sleep(600)
