for i in $(seq 1 30);
do 
    echo "teste $i"; 
    ./new_grasp < "inst$i.txt" > "sai$i".txt
done
