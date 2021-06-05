for i in $(seq 1 10);
do
    echo "rodando instancia $i";
    ./new_grasp < "inst$i.txt" > "saida$i.txt";
done
