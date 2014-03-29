#! /usr/bin/env python3.3
Template = '''
<html>

<head>
  <title>Report From Lab 7</title>
  <link type="text/css" rel="stylesheet"  href="g30_lab09_report.css">	
</head>

<body>

<h1><center> {Introduction}<center> </h1>
{Intro}<br>
<h2> {Heading[0]} </h2>
{Body[0]}<br>

<h2> {Heading[1]} </h2>
{Body[1]}<br>

<h2> {Heading[2]} </h2>
{Body[2]}<br>

<h2> {Heading[3]} </h2>
{Body[3]}<br>

<h2> {Heading[4]} </h2>
{Body[4]}<br>

</body>

</html>'''
   
Heading=[""]*5
Body=[""]*5
Intro=""
f=open("./doc/g30_prof_report.tex","r")
lines = f.readlines()
i=0
j=0
check=0
done=0
hd=0
count=0
for line in lines: #Changing newline to break
	lines[j]=lines[j].replace("\\newline","<br>")
	j=j+1	

for line in lines:

	if line.find("section{")!= -1 and line.find("%")==-1 and done == 0:
		Introduction = line[line.find('{') + 1 : line.find('}')]
		done = 1
	elif line.find("subsection{") != -1 and hd == 0 and done == 1 :
		hd = 1
	elif hd == 0 and done == 1:
		#print(line)
		Intro = Intro + line

	if line.find("\section{") != -1 and i == 4:
		break
	else:	
		if line.find("includegraphics[") != -1 :
			if i == 0:				 
				Body[i]=Body[i] + "<center><img src='../plots/g30_lab09_plot01.png' width='500' height='500'></center>"
			elif i == 1:				 
				Body[i]=Body[i] + "<center><img src='../plots/g30_lab09_plot02.png' width='500' height='500'></center>"
			elif i == 2 and lines[(lines.index(line) + 1)].find("includegraphics[")!=-1:				 
				Body[i]=Body[i] + "<center><img src='../plots/g30_lab09_plot03.png' width='500' height='500'></center>"
			elif i == 3:				 
				Body[i]=Body[i] + "<center><img src='../plots/g30_lab09_plot04.png' width='500' height='500'></center>"
			elif i == 4:				 
				Body[i]=Body[i] + "<center><img src='../plots/g30_lab09_plot05.png' width='500' height='500'></center>"
		elif line.find("begin{equation*}") != -1 :
			if i == 0:
				Body[i]=Body[i] + "<br><center>AvgStepTime =  <sup>FixedTimeCost</sup>&frasl;<sub>NumOfIterations</sub> + VariableTimeCost</center><br>"
			elif i == 1:
				Body[i]=Body[i] + "<br><center>AvgVelTime > AvgPosTime > AvgColTime</center><br>"
		elif line.find("begin{") != -1 or line.find("end{") != -1 or line.find("caption{") != -1 :
			continue

		else :
			if line.find("subsection{")!=-1 and check == 1:
				i = i + 1
				Heading[i]=line[line.find('{') + 1 : line.find('}')]
				
			elif line.find("subsection{")!=-1 and check == 0:
				Heading[i]=line[line.find('{') + 1 : line.find('}')]
				check = 1
			elif check == 1 :
				Body[i]=Body[i]+line



data = Template.format(**locals())
filename='./doc/g30_lab09_report.html'
output = open(filename,"w")
output.write(data)
output.close()	

