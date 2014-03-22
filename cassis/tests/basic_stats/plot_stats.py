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

a=""
for i in range(0, nb_functions):
    a = a + file2_l[i].replace('\n','')
    for j in range(0, 15):
        a = a + " " 
 

plt.title("Statistiques de base")
plt.xlabel(a)
plt.ylabel("Stats")
for i in range(0, nb_functions):
    plt.plot(3*i,   Y[i,0], 'o', color='r', label='Miss')
    plt.plot(3*i+1, Y[i,1], 'o', color='b', label='Hits')
    plt.plot(3*i+2, Y[i,2], 'o', color='g', label='WB')

plt.legend()
plt.savefig("images/stats.png")
