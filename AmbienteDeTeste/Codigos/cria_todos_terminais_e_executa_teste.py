import os
import random
from time import sleep

arq = open('quant_testes.txt', 'r')
quant_testes = int(arq.read())
arq.close()

arq = open('numero_instancia.txt', 'r')
numero_instancia = int(arq.read())
arq.close()


random.seed(None)
print("Quantidade de terminais que serão executados: ", quant_testes)

def valor_alfa():
	alfa_final = 0.8
	while alfa_final > 0.1:
		alfa_final = random.random()

	
	return alfa_final
	#return random.random()

for i in range(1, quant_testes + 1):
	print("RODANDO TESTE DE NUMERO: ", i)
	comando = "gnome-terminal -e 'python executa_teste_por_terminal.py " + str(valor_alfa()) + " " + str(i) + " " + str(numero_instancia) + "'"
	print("COMANDO PARA INICIAR TERMINAL PRONTO: ", comando)
	os.system(comando)
	sleep(0.5)
