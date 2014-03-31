#!/bin/bash
if [ ! -e ../data_gnuplot/g30_lab05data_02.csv ]
then
for iteration_value in {1..30}
do
	for rerun_number in {1..60}
	do	
		#echo "$iteration_value" > temp.txt
		tempdata=`../mybins/cs296_30_exe $iteration_value` 
		#test -e temp.txt && rm temp.txt
		echo "$tempdata" | awk '
		BEGIN {	
			ORS=","
			{print '$iteration_value'}
			{print '$rerun_number'}
		}
			
		{	
			if( NR > 1 && NR != 6){
				if ($5 == "is") {
					print $6
				}
				else if ($5 == "updates") {
					print $7
				}	 
				else {
					ORS= "\n"
					print $5
				} 
			}
		}
		' >> ../data_gnuplot/g30_lab05data_02.csv
	done
done
fi

  
