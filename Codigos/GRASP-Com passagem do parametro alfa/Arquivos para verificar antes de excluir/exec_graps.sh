for i in $(seq 15 30);
do
    echo "rodando instancia $i";
    ./new_grasp < "inst$i.txt" > "saida$i.txt";
done
