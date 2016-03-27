#!/usr/bin/python
import math

fin = open('triangle.txt','r')
lst = []
for line in fin:
#	convert list of string to list of int
	lst += map(int , line.split()) 
fin.close()

#lst = [ 	1,
#		   1,1,
#		  1,1,1,
#		 1,1,1,1,
#		1,1,0,3,1,
#	   1,1,1,1,1,1]
#lst = [59,
#73, 41,
#52, 40, 9,
#26, 53, 06, 34,
#10, 51, 87, 86, 81,
#61, 95, 66, 57, 25, 68,
#90, 81, 80, 38, 92, 67, 73,
#30, 28, 51, 76, 81, 18, 75, 44]

#get nth element of the series 1,2,2,3,3,3,4,4,4,4,...
def getelement(n):
	x = (1+ math.sqrt(1+8*n))/2
	return math.floor(x) 

length = len(lst)

i = 1	#index
c = 1	#num to be added

#add values as you go down the tree
while(i+c <= length):
	lst[i] += lst[i-c]
	i += 1
	
	for x in xrange(c-1):	
		if(lst[i-c-1] < lst[i-c]):
			lst[i] += lst[i-c-1]
		else:
			lst[i] += lst[i-c]
		i += 1
		
	lst[i] += lst[i-c-1]
	i += 1
	c += 1
lst1 = lst[length-c:length]
print lst1
lst1.sort()
min = lst1[0]
i = lst.index(min,length-c, length)
print i, c
sol = []
sol.append(i)

#find the path up 
while(i-(c-1) > 0):

	l = getelement(i-c)
	r = getelement(i-(c-1))
	if ( l == r):
		i = i-c if (lst[i-c] < lst[i-c-1]) else i-(c-1)
	else:
		i = i-c if (l<r) else i-(c-1)
	c -= 1
	
	sol.append(i)
#print it in reversed order	
print sol[::-1]


