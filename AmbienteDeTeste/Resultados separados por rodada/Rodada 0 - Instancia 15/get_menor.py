import os



lista_pasta = os.listdir()
caminho = os.getcwd()

lista_pasta.sort()

print("Quantidade de pastas: ", len(lista_pasta))
lista_todos_custos = []
for pasta in lista_pasta:
	if ".py" in pasta:
		continue

	print("pasta: ", pasta)
	caminho_pasta = caminho + "/" + pasta + "/"

	lista_arquivos = os.listdir(caminho_pasta)
	lista_arquivos.sort()
	print("caminho_pasta: ", lista_arquivos)
	caminho_arquivo = caminho_pasta + str(lista_arquivos[1])

	print(caminho_arquivo)
	arq = open(caminho_arquivo, 'r')
	lista_conteudo = arq.readlines()
	arq.close()

	custo = lista_conteudo[-1]
	lista_todos_custos.append(int(custo))

lista_todos_custos.sort()

for i in lista_todos_custos:
	print(i)