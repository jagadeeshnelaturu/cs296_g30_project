#!/bin/bash
if [ ! -e ../data_gnuplot/g30out-1-1.txt ]
then
cd ../mybins
for iteration_value in {1..30}
do
	for rerun_number in {1..60}
	do
	#echo $iteration_value > temp.txt
	./cs296_30_exe $iteration_value > ../data_gnuplot/g30out-$iteration_value-$rerun_number.txt
	#test -e temp.txt && rm temp.txt 
	done
done
fi
