for i in $(seq 9 10);
do
    echo "rodando instancia $i";
    ./new_grasp < "inst$i.txt" > "saida$i.txt";
done
