import sys
import csv
import re

#check only two arguments + name of program
if len(sys.argv) != 3:
    print("Incorrect number of arguments")
    sys.exit()

#open csv file and read it
with open(sys.argv[1], "r") as csv_file:
    dna_df = csv.DictReader(csv_file)
    #move to first line of dictionary
    row = next(dna_df)
    #write keys into STRs list
    STRs = []
    for key in row:
        STRs.append(key)

#remove name
STRs.pop(0)

#open and read txt file
with open(sys.argv[2], "r") as txt_file:
    dna_sequence = txt_file.read()

    #create list to hold results for each STR
    STR_count = []

    dna_length = len(dna_sequence)
    #iterate over STRs
    for STR in STRs:

        count = 0
        num_of_repetitions = []
        num_of_repetitions.append(0)
        #iterate over dna sequence character by character
        for c in range(dna_length):
            #first find first match
            if dna_sequence[c:c+len(STR)] == STR:
                count = 1
                #split remaining string by elements
                step = len(STR)

                s_to_split = dna_sequence[c:dna_length]
                s_splitted = re.findall('.'*step, s_to_split)

                 #count repeating elements
                for i in range(len(s_splitted) - 1):

                    if s_splitted[i] == STR and s_splitted[i+1] != STR:
                        break
                    else:
                        count += 1

                #write count to list at c'th position
                num_of_repetitions.append(count)


        #get max value of repetitions
        max_reps = max(num_of_repetitions)

        #write to result counts
        STR_count.append(str(max_reps))

#here for each of STR we have max number of repetitions
#now its time to check with database
#open csv file and read it
with open(sys.argv[1], "r") as csv_file:
    dna_df = csv.DictReader(csv_file)

    for row in dna_df:
        #write values into STRs list
        STRs_current = []
        for key, value in row.items():
            STRs_current.append(value)
        STRs_current.pop(0)

        if STRs_current == STR_count:
            print(row["name"])
            sys.exit()

print("No match")
