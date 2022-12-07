g++ main.cpp -std=c++11
touch results.txt
> results.txt

for (( i=1; i <= 15; i++ ))
do
    echo $i >> results.txt
    { time ./a.out 1000 1000 100 $i; } 2>> results.txt
    echo "===" >> results.txt
done

python3 plot.py