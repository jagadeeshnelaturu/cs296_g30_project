#!/bin/bash
if [ ! -e ../data_gnuplot/g30_lab05data_random.csv ]
then
NUM=9 # rerun number - 1

for iteration_value in {1..20} # actual iteration number
do
	for random_line in {1..3} # how many random sample one wants
	do
		let X=${RANDOM}
		let " X %= $NUM "
		#echo $X
		let " X = X + 1 "
		#echo $X	
		let "Line= 10*($iteration_value-1) + $X" # first argument is rerun number
		sed -n ${Line}p ../data_gnuplot/g30_lab05data_02.csv >> ../data_gnuplot/g30_lab05data_random.csv 
	done



done
fi
