
quant_tipos_facilidades = 0

def troco(valor, pos):
    if(not valor):
        return True
    
    if(pos >= quant_tipos_facilidades):
        return False

    if(valor >= vet_tipos_facilidadesL[pos]):
        return (troco(valor - vet_tipos_facilidadesL[pos], pos) or troco(valor, pos + 1))

    return troco(valor, pos + 1)



for i in range(1,31):
    arq = open("sai" + str(i) + ".txt", "r")
    string = arq.read().replace(" \n","")
    string = string.split(" ")
    #print(string)
    arq.close()

    vet_tipos_facilidadesL = []
    for j in string:
        vet_tipos_facilidadesL.append(int(j))

    vet_tipos_facilidadesL.sort()
    vet_tipos_facilidadesL.reverse()
    print(vet_tipos_facilidadesL)
    lista_final = []
    for i in range(len(vet_tipos_facilidadesL) - 1 ,-1):
        if(not troco(vet_tipos_facilidadesL[i], i)):
            lista_final.append(vet_tipos_facilidadesL[i])
    print(lista_final)    
    
