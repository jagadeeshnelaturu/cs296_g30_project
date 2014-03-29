#!/bin/bash
#Assuming Max Roll Number In Class Is 87
awk ' BEGIN {
ORS = "\n"
FS = ","
}
{	
	if( $1 == "7" ){
	print $3
	}
}
' <../data_gnuplot/g30_lab05data_02.csv > ../data_gnuplot/temp_plot4.data	

