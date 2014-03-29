#!/bin/bash
awk '
BEGIN {
count=1
FS=","
}
{	
	sample=3
	if(NR <= sample*count){
		iteration+=$1
		step+=$3
		}
	else{	
		ORS= ","
		count++
		{print iteration/sample}
		ORS = "\n"
		{print step/sample}
		iteration=$1
		step=$3
	    }
}
END {
ORS= ","
{print iteration/sample}
ORS = "\n"
{print step/sample}
}' <../data_gnuplot/g30_lab05data_random.csv > ../data_gnuplot/temp_random.data	
