import re 
import numpy as np
import matplotlib.pyplot as plt
from datetime import datetime
Eventlog = []

logfile = open("logtext.txt","r")
TimestampList = []
ThreadIdList = []
EventList = []
logcount = 0
for lines in logfile:
	Stamp = re.search(r'\d{1,2}:\d{1,2}:\d{1,2}:\d{1,3}:\d{1,3}:\d{1,3}.\bThread\b', lines)
	if Stamp:
		Thread = re.search(r'(\bThread\b\s\d(?:\s|\d))', lines)
		if Thread:	
			Event = re.search(r'\bEvent\b:.*', lines)
			if Event:
				EventList.append(Event.group())
				ThreadIdList.append(Thread.group())
				TimestampList.append(Stamp.group())
count = -1
for thread in ThreadIdList:
	count+=1
	if thread.find("Thread 0 ") != -1:
		tStamp = TimestampList[count].split(']')
	##	print tStamp[0]				
	##	print thread
	##	print EventList[count]+'\n'
		EventDict = EventList[count]+'\n'
		EventInfo = {'TimeStamp': tStamp[0],'ThreadID': thread,'Event':EventDict}
		Eventlog.append(dict(EventInfo))
timestamp1 = 0
timestamp2 = 0		
eventcount = 0	
startpoint = 0
parallelplotlength = []
parallelplotcolor = []
parallelleftVal = []
serialplotlength = []
serialplotcolor = []
serialleftVal = []
waitingplotlength = []
waitingplotcolor = []
waitingleftVal = []

for event in Eventlog:
	if eventcount < 1:
		timestamp1 = event['TimeStamp']
	timestamp2= event['TimeStamp']
	splittime = timestamp1.split(':')
	splittime2 = timestamp2.split(':')
	nanosecondTime = ((int(splittime[0]) * 3600000000000) + (int(splittime[1])*60000000000) + (int(splittime[2])*1000000000) + (int(splittime[3])*1000000) + (int(splittime[4])*1000) + int(splittime[5]))
	nanosecondTime2 = ((int(splittime2[0]) * 3600000000000) + (int(splittime2[1])*60000000000) + (int(splittime2[2])*1000000000) + (int(splittime2[3])*1000000) + (int(splittime2[4])*1000) + int(splittime2[5]))
	diffInMilliseconds = (float(nanosecondTime2 - nanosecondTime)/1000000)
	#print diffInMilliseconds
	timestamp1=timestamp2
	eventcount+=1
	if eventcount > 1:
		startpoint += diffInMilliseconds
		N = 1
		ind = np.arange(N)    # the x locations for the groups
		width = 0.2       # the width of the bars: can also be len(x) sequence
		if 'ParallelID' in event['Event']:
			serialplotlength.append(diffInMilliseconds)
			serialleftVal.append(startpoint)	
		if 'BarrierEnd' in event['Event']:
			waitingplotlength.append(diffInMilliseconds)
			waitingleftVal.append(startpoint)
		if 'BarrierBegin' in event['Event']:
			parallelplotlength.append(diffInMilliseconds)
			parallelleftVal.append(startpoint)
		
parallelplot = (parallelplotlength)
serialplot = (serialplotlength)
waitingplot = (waitingplotlength)
si = 0
pi = 0
wi = 0
for serialVal in serialplot:
	plot = plt.barh(ind, serialplotlength[si], width, color='r', edgecolor='r', left=serialleftVal[si])
	si+=1
	print 'adding to serial array',si

for parallelVal in parallelplot:	
		plot2 = plt.barh(ind, parallelplotlength[pi], width, color='g', edgecolor='g', left=parallelleftVal[pi])
		pi+=1
		print 'adding to parallel array',pi
for waitingVal in waitingplot:
		plot3 = plt.barh(ind, waitingplotlength[wi], width, color='b',edgecolor='b',  left=waitingleftVal[wi])
		wi+=1
		print 'adding to wait array',wi

print 'finished collecting values'
plt.axis([0,3000000,0,1])
print 'setting up axis'
plt.xlabel('Time in MS')
plt.title('Breakdown of program execution')
print 'adding x ticks'
plt.xticks(np.arange(0, 3000000, 5000000))
print 'adding y ticks'
plt.yticks(ind+0.1, ['Thread 1'])
print 'trying to plot'
plt.legend((plot, plot2, plot3), ('Serial','Parallel','Waiting'))
		#plt.legend((plot, p2[0], p3[0]), ('Serial', 'Parallel', 'Waiting'))

plt.show()

num = 0
if num > 0:
	N = 2
	threadNotCreated = np.array([0, 3000])
	thread1Serial = np.array([3000.145304, 0])

	thread1Parallel = np.array([1200.56136, 1300.45305])
	thread1Waiting = np.array([100.191564408, 200.67336])

	thread2Parallel = np.array([3500.56136, 3400.45305])
	thread2Waiting = np.array([300.191564408, 0])

	thread3Parallel = np.array([5000.56136, 6000.45305])
	thread3Waiting = np.array([700.191564408, 400.67336])

	thread4Parallel = np.array([1800.56136, 3000.45305])
	thread4Waiting = np.array([300.191564408, 700.67336])

	thread5Parallel = np.array([10000.56136, 6000.45305])
	thread5Waiting = np.array([2000.191564408, 200.67336])

	thread6Parallel = np.array([3000.56136, 6001.45305])
	thread6Waiting = np.array([100.191564408, 2000.67336])

	thread7Parallel = np.array([7000.56136, 4000.45305])
	thread7Waiting = np.array([180.191564408, 200.67336])

	thread1Serial2 = np.array([9000, 0])


	p0 = plt.barh(ind, threadNotCreated, width, color='w')
	p1 = plt.barh(ind, thread1Serial, width, color='r')
	p2 = plt.barh(ind, thread1Parallel, width, color='y',
		     left=threadNotCreated+thread1Serial)
	p3 = plt.barh(ind, thread1Waiting, width, color='b',
		     left=threadNotCreated+thread1Serial+thread1Parallel)

	p4 = plt.barh(ind, thread2Parallel, width, color='y',
		     left=threadNotCreated+thread1Serial+thread1Parallel+thread1Waiting)
	p5 = plt.barh(ind, thread2Waiting, width, color='b',
		     left=threadNotCreated+thread1Serial+thread1Parallel+thread1Waiting+thread2Parallel)
	p5 = plt.barh(ind, thread3Parallel, width, color='y',
		     left=threadNotCreated+thread1Serial+thread1Parallel+thread1Waiting+thread2Parallel + thread2Waiting)
	p6 = plt.barh(ind, thread3Waiting, width, color='b',
		     left=threadNotCreated+thread1Serial+thread1Parallel+thread1Waiting+thread2Parallel + thread2Waiting+thread3Parallel)
	p7 = plt.barh(ind, thread4Parallel, width, color='y',
		     left=threadNotCreated+thread1Serial+thread1Parallel+thread1Waiting+thread2Parallel + thread2Waiting+thread3Parallel+thread3Waiting)
	p8 = plt.barh(ind, thread4Waiting, width, color='b',
		     left=threadNotCreated+thread1Serial+thread1Parallel+thread1Waiting+thread2Parallel + thread2Waiting+thread3Parallel+thread3Waiting+thread4Parallel)
	p9 = plt.barh(ind, thread5Parallel, width, color='y',
		     left=threadNotCreated+thread1Serial+thread1Parallel+thread1Waiting+thread2Parallel + thread2Waiting+thread3Parallel+thread3Waiting+thread4Parallel+thread4Waiting)
	p10 = plt.barh(ind, thread5Waiting, width, color='b',
		     left=threadNotCreated+thread1Serial+thread1Parallel+thread1Waiting+thread2Parallel + thread2Waiting+thread3Parallel+thread3Waiting+thread4Parallel+thread4Waiting+thread5Parallel)
	p11 = plt.barh(ind, thread6Parallel, width, color='y',
		     left=threadNotCreated+thread1Serial+thread1Parallel+thread1Waiting+thread2Parallel + thread2Waiting+thread3Parallel+thread3Waiting+thread4Parallel+thread4Waiting+thread5Parallel+thread5Waiting)
	p12 = plt.barh(ind, thread6Waiting, width, color='b',
		     left=threadNotCreated+thread1Serial+thread1Parallel+thread1Waiting+thread2Parallel + thread2Waiting+thread3Parallel+thread3Waiting+thread4Parallel+thread4Waiting+thread5Parallel+thread5Waiting+thread6Parallel)
	p13 = plt.barh(ind, thread7Parallel, width, color='y',
		     left=threadNotCreated+thread1Serial+thread1Parallel+thread1Waiting+thread2Parallel + thread2Waiting+thread3Parallel+thread3Waiting+thread4Parallel+thread4Waiting+thread5Parallel+thread5Waiting+thread6Parallel
	+thread6Waiting)
	p14 = plt.barh(ind, thread7Waiting, width, color='b',
		     left=threadNotCreated+thread1Serial+thread1Parallel+thread1Waiting+thread2Parallel + thread2Waiting+thread3Parallel+thread3Waiting+thread4Parallel+thread4Waiting+thread5Parallel+thread5Waiting+thread6Parallel
	+thread6Waiting+thread7Parallel)
	p15 = plt.barh(ind, thread1Serial2, width, color='r',
		     left=threadNotCreated+thread1Serial+thread1Parallel+thread1Waiting+thread2Parallel + thread2Waiting+thread3Parallel+thread3Waiting+thread4Parallel+thread4Waiting+thread5Parallel+thread5Waiting+thread6Parallel
	+thread6Waiting+thread7Parallel+thread7Waiting)

	
