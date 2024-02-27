import argparse #docs: https://docs.python.org/3/library/argparse.html
import time #docs: https://docs.python.org/3/library/time.html
import random #docs: https://docs.python.org/3/library/random.html
from mergesort import *
from selectionsort import *
from quicksort import *

####################
#   starter code   #
####################

# helper function for generating inputs
def inputGenerator(n):
    lst = []
    for i in range(n):
        lst.append(random.randint(0, n-1))
    return lst

# define positional arguments
parser = argparse.ArgumentParser()
parser.add_argument('sizeMin', type=int, help="Minimum input size to test")
parser.add_argument('sizeIncr', type=int, help="Increment for input size")
parser.add_argument('sizeMax', type=int, help="Maximum input size to test")
parser.add_argument('numTrials', type=int, help="Number of trials for each input size")

# define optional arguments
parser.add_argument('--whichsorts', '-w',
                    nargs='+',
                    type=str,
                    help="One or more sorts to test. Default is to test all sorts."
                    )

# parse command line arguments based on definitions
args = parser.parse_args()

# uncomment the lines below for an example of how to access the parsed arguments
#   (comment them out again when you're ready to proceed)

# print("parsing arguments from command line...")
# print(f'args.sizeMin: {args.sizeMin}')
# print(f'args.sizeIncr: {args.sizeIncr}')
# print(f'args.sizeMax: {args.sizeMax}')
# print(f'args.numTrials: {args.numTrials}')
# print(f'args.whichsorts: {args.whichsorts}')


#############################
#   write your code below   #
#############################

def computeTime(lst, fctn):
    initialTime = time.time()
    fctn(lst)
    return time.time() - initialTime

def runSorts(min, inc, max, run, fctns):
    resultMatrix = []
    nRow = ['N']
    mergeTimes = ['Merge Sort']
    quickTimes = ['Quick Sort']
    selectTimes = ['Selection Sort']
    arrInd = 1

    for i in range(min, max + inc, inc):
        nRow.append(i)
        for j in range(0,run):

            trial = inputGenerator(i)

            if fctns is None:
                # might give error 
                if len(mergeTimes) < arrInd + 1:
                    mergeTimes.append(computeTime(trial, mergesort))
                    quickTimes.append(computeTime(trial, quicksort))
                    selectTimes.append(computeTime(trial, selectionsort))
                else: 
                    mergeTimes[arrInd] += computeTime(trial, mergesort)
                    quickTimes[arrInd] += computeTime(trial, quicksort)
                    selectTimes[arrInd] += computeTime(trial, selectionsort)

            else:
                for func in fctns:
                    if func == 'mergesort':
                        if len(mergeTimes) < arrInd + 1:
                            mergeTimes.append(computeTime(trial, mergesort))
                        else: 
                            mergeTimes[arrInd] += computeTime(trial, mergesort)
                    if func == 'quicksort':
                        if len(quickTimes) < arrInd + 1:
                            quickTimes.append(computeTime(trial, quicksort))
                        else: 
                            quickTimes[arrInd] += computeTime(trial, quicksort)
                    if func == 'selectionsort':
                        if len(selectTimes) < arrInd + 1:
                            selectTimes.append(computeTime(trial, selectionsort))
                        else: 
                            selectTimes[arrInd] += computeTime(trial, selectionsort)

        if len(mergeTimes) > 1:
            mergeTimes[arrInd] = round(mergeTimes[arrInd] / run, 5)
        if len(quickTimes) > 1:
            quickTimes[arrInd] = round(quickTimes[arrInd] / run, 5)  
        if len(selectTimes) > 1:
            selectTimes[arrInd] = round(selectTimes[arrInd] / run, 5)    

        arrInd += 1

    resultMatrix.append(nRow)
    arrInd = 1
    if len(mergeTimes) > 1:
            resultMatrix.append(mergeTimes)
            arrInd += 1   
    if len(quickTimes) > 1:
        resultMatrix.append(quickTimes) 
        arrInd += 1    
    if len(selectTimes) > 1:
        resultMatrix.append(selectTimes) 
        arrInd += 1  

    return resultMatrix

def printTable(matrix):
    table = ""
    for i in range(0, len(matrix[0])):
        for list in matrix:
            table += str(list[i]) + "   \t"
        table += "\n"
    print(table)

m = runSorts(args.sizeMin, args.sizeIncr, args.sizeMax, args.numTrials, args.whichsorts)
printTable(m)
