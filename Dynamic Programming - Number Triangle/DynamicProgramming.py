#Alex Moore
#Dynamic Programming Solution
import sys
from datetime import datetime
from random import *


#Variables##########################
numberedFile = False
n = 0
maxProduct = 0

#Timing information
before = datetime.now()
####################################    


def prodDP(totalTree, n, prod, fact):
    #Insert last row of triangle into the product array
    for i in range(n):
        prod[n-1][i] = totalTree[n-1][i]
        
    #Multiply each node with the larger of its two children and iterate up the triangle
    for i in range(n-2, -1, -1):
        for j in range(i, -1, -1):
            prod[i][j] = totalTree[i][j] * max(prod[i+1][j], prod[i+1][j+1])
    
    #Find the index of each factor used to find the max product
    index[0] = 0
    for i in range(1, n-1):
        index[i] = prod[i].index(max(prod[i][index[i-1]], prod[i][index[i-1]+1]))
    
    #Find the last index of sequence for triangles with n > 1
    if(n > 1):
        index[n-1] = prod[n-1].index(max(prod[n-1][index[n-2]], prod[n-1][index[n-2]+1]))
        
    #Retrieve used factors from each row using the indexes we found
    for i in range(n):
        fact[i] = totalTree[i][index[i]]
    
    #Return maximum product found
    return prod[0][0]

##Used for testing purposes############################
#Create number triangle of size (num) of random numbers
num = 25
testFile = open("test.txt", "w")
testFile.write(str(num) + "\n")
for i in range(num):
    for count in range(i+1):
        testFile.write(str(randint(0,99)) + " ")
    testFile.write("\n")
    
testFile.close()

del num, testFile, count

#######################################################

#Input File##########################################
fileName = "test.txt"

#Check for a number in the filename
fileNameSplit = fileName.split('.')
if(fileNameSplit[0][-1].isnumeric()):
    fileNumber = fileNameSplit[0][-1]
    numberedFile = True
    
#Open input file
f = open(fileName)

#Retrieve data from input file
n = (int)(f.readline())

#Initialize variables that require size of n
totalTree = [0]*n
maxFactors = [0]*n
product = [[-1 for i in range(n)]for j in range(n)]
index = [-1]*n

#Create list to represent the input number triangle
for i in range(n):
    row = f.readline()
    rowSplit = row.split()
    totalTree[i] = [0]*len(rowSplit)
    for j in range(len(rowSplit)):
        totalTree[i][j] = (int)(rowSplit[j])

f.close
#####################################################



#Find Max Product from every combination
maxProduct = prodDP(totalTree, n, product, maxFactors)



#Output File#########################################
#Numbered input file creates numbered output file
if(numberedFile == True):
    out = "out" + fileNumber + ".txt"
    outputFile = open(out, "w")
    del out
#Non-numbered input file creates a non-numbered output file
else:
    outputFile = open("out.txt", "w")

#Create string for output sequence
factorString = ""
for i in range(len(maxFactors)):
    factorString += (str(maxFactors[i]) + " ")

#Format output and write to file
output = str(maxProduct) + "\n" + factorString
outputFile.write(output)
outputFile.close()
#####################################################

#Timing information
after = datetime.now()
print(after - before)

#Garbage Collection
del fileName, fileNameSplit, f, row, rowSplit
del after, before, n, i, j
del maxFactors, maxProduct, factorString, product, totalTree
del numberedFile, output, outputFile