#!/bin/bash

echo "Quantas iteracoes?"
read quantidade_iteracoes;
echo

eval "rm algori*"
eval "gcc n-Queens-Prize-Backtracking/n-queens-prize-backtracking.c -Ofast -o n-Queens-Prize-Backtracking/n-Queens-Prize-Backtracking"
eval "gcc n-Queens-Prize-BranchAndBound-1/n-queens-prize-branchAndBound-1.c -Ofast -o n-Queens-Prize-BranchAndBound-1/n-Queens-Prize-BranchAndBound-1"
eval "gcc n-Queens-Prize-BranchAndBound-2/n-queens-prize-branchAndBound-2.c -Ofast -o n-Queens-Prize-BranchAndBound-2/n-Queens-Prize-BranchAndBound-2"

#instancias=( nqp005.txt nqp008.txt nqp010.txt nqp020.txt nqp030.txt nqp040.txt nqp050.txt nqp060.txt nqp070.txt nqp080.txt nqp090.txt nqp100.txt nqp200.txt )
instancias=( nqp005.txt )

algoritmos=( n-Queens-Prize-Backtracking n-Queens-Prize-BranchAndBound-1 n-Queens-Prize-BranchAndBound-2 )
#algoritmos=( n-Queens-Prize-Backtracking  )

for algoritmo in "${algoritmos[@]}"
do
	echo $algoritmo

  for instancia in "${instancias[@]}"
  do
  	echo $instancia

    for (( i = 0; i < "$quantidade_iteracoes"; i++ )); do
      echo "$i"

      cmd="./$algoritmo/$algoritmo $instancia $i*1234 60 1"
      date
      echo $cmd
      $cmd
    done
    echo

  done
  echo

done
