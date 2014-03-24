import time
import numpy as np
import scipy
import csv
import matplotlib.pyplot as plt
from numpy.random import random
from scipy.sparse.linalg import bicgstab
from scipy.sparse.linalg import gmres
import os, sys

b = (sys.argv[1]) # Functions
d = (sys.argv[2]) # Number of functions

e = (sys.argv[3]) # Size of the problem
f = (sys.argv[4]) # Cache level


file2 = open(b, 'r')
file2_l = file2.readlines()

nb_values = 1
nb_functions = int(d)

X = np.zeros([nb_functions, 1])
Y = np.zeros([nb_functions, 3])

for i in range(0, nb_functions):
    str = file2_l[i]    
    file3 = open("tmp/test_"+str.replace('\n',''), 'r')
    file3_l = file3.readlines()
    j = 0
    tot = 0
    for line in file3_l:
        Y[i, j] += int(line)
        tot += 1
        j = (j+1) % 3
    for j in range(0, 3):
        Y[i, j] = 3 * Y[i, j] / tot


place = []
mots = []

for i in range(0, nb_functions):
    place.append(3*i+1.5)
    mots.append(file2_l[i].replace('\n',''))
print place, mots 

fig = plt.figure()
ax = fig.add_subplot(111)
frame1 = plt.gca()
frame1.axes.get_xaxis().set_ticks([])

plt.title("Statistiques de base")
plt.ylabel("Stats")
i=0
x = np.arange(1)
y = np.arange(1)
x[0] = 3*i+0.5
y[0] = Y[i, 0]
rects1 = ax.bar(x, y, 0.50, color='red', label='Miss')
x[0] = 3*i+1.5
y[0] = Y[i, 1]
rects2 = ax.bar(x, y, 0.50, color='green', label='Hit')
x[0] = 3*i+2.5
y[0] = Y[i, 2]
rects3 = ax.bar(x, y, 0.50, color='blue', label='WB')
for i in range(1, nb_functions):
    x = np.arange(1)
    y = np.arange(1)
    x[0] = 3*i+0.5
    y[0] = Y[i, 0]
    rects1 = ax.bar(x, y, 0.50, color='red')
    x[0] = 3*i+1.5
    y[0] = Y[i, 1]
    rects2 = ax.bar(x, y, 0.50, color='green')
    x[0] = 3*i+2.5
    y[0] = Y[i, 2]
    rects3 = ax.bar(x, y, 0.50, color='blue')

plt.xticks(place, mots)
plt.legend()
plt.savefig("images/stats_"+e+"_"+f+".png")
