"""
This script formats Moodle Quiz Result CSV's to Mr. Rahmat's Specification
FWS - Francis Wibisono - 1906350553 - www.franciswibisono.com
"""
import csv

##You can change these
source_file = "source.csv"
output_file = "W01Q02.txt"

masterArray = []
with open(source_file) as source:
    csv_reader = csv.reader(source, delimiter=',')
    for row in csv_reader:
        try:
            slaveArray = []
            slaveArray.append(row[2])
            score = int(row[10][:-3])
            empty = 10 - score
            for i in range(score):
                slaveArray.append("1")
            if empty != 0:
                for i in range(empty):
                    slaveArray.append("0")
            masterArray.append(slaveArray)
        except:
            print("nope")

with open(output_file, 'w') as output:
    for element in masterArray:
        if element[0] != "":
            output_str = ""
            for i in range(len(element)):
                if i != 10:
                    output_str += (element[i] + " ")
                else:
                    output_str += element[i]
            output.write(output_str)
            output.write('\n')
