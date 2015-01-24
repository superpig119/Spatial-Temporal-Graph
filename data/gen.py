#!/usr/bin/python

import random

data = open('data', 'w')

nnode=1000
tperiod=10000
r=random

data.write(str(nnode) + '\n')
for id in range(nnode):
	t1 = 0
	count = 0
	td = r.randint(1,3000)
	line = str(id)
	linetmp = ""
	while t1 + td < tperiod:
		count += 1
		t2 = t1+td
		x = r.randint(0,10000)
		y = r.randint(0,10000)
		linetmp += '\t' + str(x) + '\t' + str(y) + '\t' + str(t1) + '\t' + str(t2) 
		t1 = t2 + 1
		td = r.randint(1, 3000)
	line += '\t' + str(count) + linetmp + '\n'
	data.write(line)

list = range(nnode)
for id in range(nnode):
	line = str(id)
	enum = r.randint(400,700)
	slice = r.sample(list, enum)
	slice.sort()
	line += '\t' + str(enum)
	for e in slice:
		t1 = r. randint(1, tperiod)
		t2 = r. randint(1, tperiod)
		while t1 == t2:
			t1 = r. randint(1, tperiod)
			t2 = r. randint(1, tperiod)
		if(t1 > t2):
			tmp = t1
			t1 = t2
			t2 = tmp
		line += '\t' + str(e) + '\t' + str(t1) + '\t' + str(t2)
	line += '\n'
	data.write(line)

data.close()

