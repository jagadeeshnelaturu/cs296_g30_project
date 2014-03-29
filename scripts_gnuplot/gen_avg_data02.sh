#!/bin/bash
awk '
BEGIN {
count=1
FS=","
}
{	
	rerun=10
	if(NR <= rerun*count){
		iteration+=$1
		step+=$3
		}
	else{	
		ORS= ","
		count++
		{print iteration/rerun}
		ORS = "\n"
		{print step/rerun}
		iteration=$1
		step=$3
	    }
}
END {
ORS= ","
{print iteration/rerun}
ORS = "\n"
{print step/rerun}
}' <../data_gnuplot/g30_lab05data_02.csv > ../data_gnuplot/temp_avg_data02.data	
