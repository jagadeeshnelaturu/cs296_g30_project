#! /usr/bin/env python3.3
import subprocess

iteration_number=20
rerun_number=10
final=""
for i in range(1,iteration_number + 1):
	for j in range(1,rerun_number + 1):
		#print(i)
		command = [ './mybins/cs296_30_exe' , str(i) ]
		output = subprocess.Popen( command, stdout= subprocess.PIPE )
		it_num = (output.stdout.readline().decode('utf-8'))[22:]
		step = (output.stdout.readline().decode('utf-8'))[25:-4]
		col = (output.stdout.readline().decode('utf-8'))[31:-4]
		vel = (output.stdout.readline().decode('utf-8'))[37:-4]
		pos = (output.stdout.readline().decode('utf-8'))[37:-4]
		empty = (output.stdout.readline().decode('utf-8'))
		loop = (output.stdout.readline().decode('utf-8'))[19:-4]
		final = final + str(i) + ", " + str(j) + ", " + step + ", " + col + ", " + vel + ", " + pos + ", " + loop + "\n"
		#print(final)
f = open('./data_matplotlib/g30_lab09data_01.csv','w')
f.write(final)
