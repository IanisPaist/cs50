import sys
import csv
from cs50 import SQL

if len(sys.argv) != 2:
    print("Incorrect nomber of command line arguments")
    sys.exit()

#put students.db in a variable
db = SQL("sqlite:///students.db")

#clear db
db.execute("DELETE FROM students")

#open the csv file
with open(sys.argv[1], "r") as csv_file:

    #make a reader of csv file
    reader = csv.DictReader(csv_file)

    #iterate
    for row in reader:
        name = row["name"].split()

        #check if middle name exists
        #if not - put None
        if len(name) == 2:
            name.append(name[1])
            name[1] = None

        #assign variables to insert into db
        first = name[0]
        middle = name[1]
        last = name[2]
        house = row["house"]
        birth = row["birth"]

        #insert into database
        db.execute("INSERT INTO students (first, middle, last, house, birth) VALUES (?,?,?,?,?)",
            first, middle, last, house, birth)