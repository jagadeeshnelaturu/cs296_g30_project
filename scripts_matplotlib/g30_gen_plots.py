#! /usr/bin/env python3.3
import csv
import numpy as np
import matplotlib.pyplot as plt
from random import randint


iteration_number=20
rerun_number=10
roll_number=17
random_sample=3

OX = np.arange(iteration_number)
OY_step=[]
OY_coll=[]
OY_vel=[]
OY_pos=[]
OY_loop=[]
OY_total=[]
OX_cum = np.arange(rerun_number)

f = open("./data_matplotlib/g30_lab09data_01.csv")
text = csv.reader(f)

step = 0
coll=0
vel=0
pos=0
loop=0
count = 0
max_step = []
min_step = []
temp_step= []
temp_count=0
value_roll=[]

for row in text:
	
	temp_step.append(float(row[2].strip()))
	step += float(row[2].strip())
	coll += float(row[3].strip())
	vel += float(row[4].strip())
	pos += float(row[5].strip())
	loop += float(row[6].strip())
	count += 1
	temp_count += 1
	
	if rerun_number*(roll_number-1) < temp_count <= (rerun_number*(roll_number-1) +  rerun_number) :
		value_roll.append( float(row[2].strip()))

	if count%rerun_number == 0:
		OY_step.append(step/rerun_number)
		OY_coll.append(coll/rerun_number)
		OY_vel.append(vel/rerun_number)
		OY_pos.append(pos/rerun_number)
		OY_loop.append(loop/rerun_number)
		OY_total.append( (coll + vel + pos)/rerun_number )
		max_step.append( max(temp_step) - (step/rerun_number) )
		min_step.append(  -min(temp_step)  + (step/rerun_number))
		#print(max(temp_step))
		step = 0
		coll = 0
		vel = 0
		pos = 0
		loop = 0
		count = 0
		temp_step = []

#print( max(OY_loop) )
####GRAPH 1###########
graph_step = plt.bar(OX + 1, OY_step, width = 1, facecolor='red', edgecolor='white', label = "Avg Step Time")
graph_loop = plt.plot(OX + 1, OY_loop, label = "Loop Time", color = 'green')
graph_max = plt.axhline(y = max(OY_loop), xmin = (min(OX) ), xmax = (max(OX) ), color = 'blue', label = "Max Loop Time")
graph_min = plt.axhline(y = min(OY_loop), xmin = (min(OX) ), xmax = (max(OX) ), color = 'purple', label = "Min Loop Time")

plt.xlabel('Iteration Number')
plt.ylabel('Avg Step Time And Loop Time In ms')
plt.title('Avg Step Time And Loop Time Vs Number Of Iterations')
plt.legend(loc='best')
plt.xlim(1,iteration_number)
plt.ylim(0, max(OY_loop) + 5)
#plt.show()	
plt.savefig('./plots_matplotlib/g30_lab09_plot01.png')
plt.clf()
####GRAPH 2###########
graph_step = plt.plot(OX + 1, OY_step, label = "Avg Step Time(S)", color = 'red')
graph_coll = plt.plot(OX + 1, OY_coll, label = "Avg Collision Time(C)", color = 'green')
graph_vel = plt.plot(OX + 1, OY_vel, label = "Avg Velocity Time(V)", color = 'blue')
graph_pos = plt.plot(OX + 1, OY_pos, label = "Avg Position Time(P)", color = 'purple')
graph_total = plt.plot(OX + 1, OY_total, label = "C + V + P", color = 'orange')

plt.xlabel('Iteration Number')
plt.ylabel('S, C, V, P And (C + V + P) In ms')
plt.title('Avg Step, Col, Vel And Pos Time Vs Iterations')
plt.legend(loc='best')
plt.xlim(1,iteration_number)
#plt.show()	
plt.savefig('./plots_matplotlib/g30_lab09_plot02.png')
plt.clf()

####GRAPH 3###########
graph_step = plt.plot(OX + 1, OY_step, label = "Avg Step Time", color = 'red')
graph_error = plt.errorbar(OX + 1, OY_step, yerr=[min_step, max_step], color = 'r', ecolor='g', capthick=0.5, label = "Error In Avg Step Time" )

plt.xlabel('Number Of Iterations')
plt.ylabel('Avg Step Time With Error In ms')
plt.title('Avg Step Time With Error Vs Iterations')
plt.legend(loc='best')
plt.xlim(1,iteration_number)
#plt.show()	
plt.savefig('./plots_matplotlib/g30_lab09_plot03.png')
plt.clf()
#plt.show()

####GRAPH 4###########
#for i in value_roll:
#	print (i)
graph_freq = plt.hist(value_roll, bins = rerun_number//3, label = "Histogram")
value_roll.sort()
graph_cum = plt.plot( value_roll, OX_cum + 1, label = "Cumulative Frequency" ) 
#plt.hist(gaussian_numbers)
#plt.show()
plt.xlabel('Time In ms')
plt.ylabel('Number Of Re-Runs')
plt.title('Frequency Plot Of Step Time')
plt.legend(loc='best')
#plt.show()	
plt.savefig('./plots_matplotlib/g30_lab09_plot04.png')
plt.clf()


f.close()
####GRAPH 5###########

#CODE FOR EXTRACTING RANDOM ITERATION NUMBERS#

f = open("./data_matplotlib/g30_lab09data_01.csv")
text = csv.reader(f)

count=1
step_rand_sum=0
step_rand_avg=[]
rand_val=[]
check = 1

for row in text:
	if count%rerun_number == 1 :
		#print("Hey")
		if count != 1 :
			step_rand_avg.append(step_rand_sum/random_sample)
			#print(check)
			#check += 1
			step_rand_sum=0
			rand_val=[]
		
		for j in range(random_sample) :
			#print("apple")
			#print(check)
			#check += 1
			#print("check")
			rand_val.append(float(randint(1,rerun_number)))
		#print( "CLOSE GROUND" )
		#print( row[0].strip() )
		#print ( rand_val )
	#print("Hi")
	#temp_rerun = 0
	temp_rerun = float(row[1].strip())

	if temp_rerun in rand_val:
		for i in range(rand_val.count(temp_rerun)) :
			step_rand_sum += float(row[2].strip())
			#print( row[0].strip(), row[1].strip() , row[2].strip())
	
	if count == iteration_number*rerun_number :
			step_rand_avg.append(step_rand_sum/random_sample)
	count+=1

function_orig = np.lib.polyfit(OX + 1, OY_step, 1)
Y_line_orig = np.lib.polyval(function_orig, OX + 1) 

plt.scatter(OX + 1, OY_step, color = 'blue', label="Avg Step Time")
plt.plot(OX + 1, Y_line_orig,color='red', label="Best Fit For Original Data")

function_rand = np.lib.polyfit(OX + 1, step_rand_avg, 1) 
Y_line_rand = np.lib.polyval(function_rand, OX + 1) 
plt.scatter(OX + 1, step_rand_avg, color = 'orange', label="Avg Step Time Of Random Sample")
plt.plot(OX + 1, Y_line_rand, 'b--', color= 'black', label="Best Fit For Random Sample")

plt.xlabel("Number Of Iterations")
plt.ylabel("Average Step Time In ms")
plt.title("Avg Step Time Vs Iterations With Best Fit")
plt.legend(loc='best')
#plt.show()
plt.xlim(1,iteration_number)
plt.savefig('./plots_matplotlib/g30_lab09_plot05.png')
#plt.clf()
