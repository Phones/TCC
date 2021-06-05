for i in $(seq 11 20);
do
    echo "rodando instancia $i";
    ./new_grasp < "inst$i.txt" > "saida$i.txt";
done
