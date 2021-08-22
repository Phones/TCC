#!/bin/bash

declare -i quant_testes

quant_testes=`cat quant_testes.txt`

echo "Quantidade de teste -> $quant_testes"

echo $1

if [ "$1" == "rm" ]
then

	echo "Delentando pastas de teste!"

	for i in $(seq 1 $quant_testes);
	do
		echo "Delentando pasta de número: $i";
		rm -rf "`pwd`/new_grasp$i"
	done

else

	echo "Iniciando copia da pasta teste!"

	for i in $(seq 1 $quant_testes);
	do
		echo "Copiando pasta de número: $i";
		cp -r "new_grasp" "`pwd`/new_grasp$i"
	done

fi

echo "Fim do programa!"