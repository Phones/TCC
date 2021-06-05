for i in $(seq 21 30);
do
    echo "rodando instancia $i";
    ./new_grasp < "inst$i.txt" > "saida$i.txt";
done
