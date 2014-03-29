#!/bin/bash
awk '
BEGIN {
count=1
FS=","
}
{	
	iter = 20
	rerun = 10
	if( NR ==1 ){
	min_step = $3
	max_step = $3
	}
	if(NR <= rerun*count){
		if($3 < min_step){
		min_step=$3
		}
		if($3 > max_step){
		max_step=$3
		}
		iteration+=$1
		step+=$3
		collision+=$4
		velocity+=$5
		position+=$6
		loop+=$7
		}
	else{	
		ORS= ","
		count++
		{print iteration/rerun}
		{print step/rerun}
		{print collision/rerun}
		{print velocity/rerun}
		{print position/rerun}
		{print loop/rerun}
		
		{print min_step}
		ORS = "\n"
		{print max_step}
		min_step=$3
		max_step=$3
		iteration=$1
		step=$3
		collision=$4
		velocity=$5
		position=$6
		loop=$7
	    }
}
END {
ORS= ","
{print iteration/rerun}
{print step/rerun}
{print collision/rerun}
{print velocity/rerun}
{print position/rerun}
{print loop/rerun}
{print min_step}
ORS = "\n"
{print max_step}
}' <../data_gnuplot/g30_lab05data_01.csv > ../data_gnuplot/temp.data	

