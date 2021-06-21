import sys
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.ticker import ScalarFormatter, FormatStrFormatter
import math



with open('currvstime.csv') as f1:

    lines1 = f1.readlines()
    
    x1 = [float((line.split()[0])[:-1]) for line in lines1] #first column is x var for hv data

with open('currvstime_nohv.csv') as f2:

    lines2 = f2.readlines()
    
    x2 = [float((line.split()[0])[:-1]) for line in lines2] #first column is x var for no hv data
    #the number of plots to be made is the number of columns minus 1 because the first column is the x var
    numplots = (len(lines1[0].split())-1)

    #let's say we want a max of 12 rows of 8 cols, we can figure out how many rows to show by dividing the total number of figures by 8 and rounding up the remainder
    rows = math.ceil(numplots / 8)

    fig, axes = plt.subplots(ncols = 8, nrows = rows, figsize=(50,50))
    
    ax = axes.ravel()

    for col in range(numplots):

        y1 = [float(line.split()[col+1][:-1]) for line in lines1] #each column is y var

        y2 = [float(line.split()[col+1][:-1]) for line in lines2]

        minlen = min(len(y1),len(y2))

        ydiff= []

        for i in range(0,minlen):

            ydiff.append(y1[i]-y2[i])
        
        #red - with high voltage, blue - baseline, green - difference
        ax[col].set_ylabel("nA")   
        ax[col].set_xlabel("seconds") 
        ax[col].plot(x1, y1, 'r-')
        ax[col].plot(x2, y2, 'b-')
        #ax[col].plot(x2[:minlen], ydiff, 'g-')
        
plt.show()  
