total = 0
for i in range(1,31):
    arq = open("sai" + str(i) + ".txt", "r")
    string = arq.read().replace(" \n","")
    string = string.split(" ")
    #print(string)
    arq.close()

    vet_tipos_facilidadesL = []
    for j in string:
        vet_tipos_facilidadesL.append(int(j))

    if 1 in vet_tipos_facilidadesL:
        total += 1
        print("instancia " + str(i) + " tem o 1 como tipo de facilidade")
        

print("Total: ", total)
    
