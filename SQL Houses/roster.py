import sys
from cs50 import SQL

if len(sys.argv) != 2:
    print("error")
    exit()

#put house in the var
input_house = sys.argv[1]

#put students.db in a variable
db = SQL("sqlite:///students.db")

#query the db
students_list = db.execute("SELECT * FROM students WHERE house = ? ORDER BY last" , input_house)

#print names
for row in students_list:
    first = row["first"]
    last = row["last"]
    birth = row["birth"]
    if row["middle"] != None:
        middle = row["middle"]
        print(f"{first} {middle} {last}, born {birth}")
    else:
        print(f"{first} {last}, born {birth}")
