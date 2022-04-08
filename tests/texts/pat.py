import sys

file = open("patterns.txt","r")
num = [1,5,25,125,250,500]
so_far = []
n = 0
pt = 0
for line in file:
    so_far.append(line)
    n+=1
    if(num[pt] == n):
        pt += 1
        sf = "pattern" + str(pt) + ".txt"
        out = open(sf,"w")
        for s in so_far:
            out.write(s)
        out.close()
file.close()

