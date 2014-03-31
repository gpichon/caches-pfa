# /*
# * Copyright (c) 2014 Nicolas Dubois, Pierre Goudet, Nicolas Heng,
# * Alexandre Honorat, Gilles Marait, Grégoire Pichon.
# *
# * This Source Code Form is subject to the terms of the Mozilla Public
# * License, v. 2.0. If a copy of the MPL was not distributed with this
# * file, You can obtain one at http://mozilla.org/MPL/2.0/.
# */

import time
import numpy as np
import scipy
import csv
import matplotlib.pyplot as plt
from numpy.random import random
from scipy.sparse.linalg import bicgstab
from scipy.sparse.linalg import gmres
import os, sys

a = (sys.argv[1]) # Values
b = (sys.argv[2]) # Functions
c = (sys.argv[3]) # Number of values
d = (sys.argv[4]) # Number of functions

file1 = open(a, 'r')
file2 = open(b, 'r')
file1_l = file1.readlines()
file2_l = file2.readlines()

nb_values = int(c)
nb_functions = int(d)

X = np.zeros([nb_values, 1])
for i in range(0, nb_values):
    X[i] = file1_l[i]

Y = np.zeros([nb_values, nb_functions])

for i in range(0, nb_functions):
    str = file2_l[i]    
    file3 = open("tmp/test_"+str.replace('\n',''), 'r')
    file3_l = file3.readlines()
    for j in range(0, nb_values):
        Y[j, i] = int(file3_l[j])

plt.title("Etude de la scalabilite")
plt.xlabel("Taille du probleme")
plt.ylabel("Temps de calcul ms)")
for i in range(0, nb_functions):
    plt.plot(X, Y[:,i], label=file2_l[i])
#plt.legend(loc='left')
plt.savefig("images/scalability.png")
