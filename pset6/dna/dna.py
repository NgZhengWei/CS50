from sys import argv, exit
import csv

if len(argv) != 3:
    print("Usage: python dna.py data.csv sequence.txt")
    exit(1)

with open(argv[2], newline='') as csvfile:
    reader = csv.reader(csvfile)
    for row in reader:
        unknownSeq = row[0] #get seq to identify as a string

with open(argv[1], 'r') as csvfile:
    reader = csv.DictReader(csvfile)
    dnaSequences = reader.fieldnames[1:] #get types of dna strands as array

dnaResult= []

for sequence in dnaSequences:
    dnaLen = len(sequence)
    count = 0 #running counter
    max = 0 #max count
    start = 0 #start index of when counter incremented
    for i in range(len(unknownSeq) - dnaLen + 1):
        if count > max:
                max = count

        if count != 0 and unknownSeq[i:i+dnaLen] == sequence and i == start + (dnaLen*count):
            count += 1
        elif unknownSeq[i:i+dnaLen] == sequence:
            count = 1
            start = i
    dnaResult.append(str(max))

with open(argv[1], newline='') as csvfile:
    reader = csv.reader(csvfile)
    for row in reader:
        if row[1:] == dnaResult: #checks dna count against person
            print(row[0]) #prints name of person
            exit(0)
print("No match")
exit(0)
