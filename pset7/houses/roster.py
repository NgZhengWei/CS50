from sys import argv, exit
from cs50 import SQL

if len(argv) != 2:
    print("Usage: roster.py <house>")
    exit(1)

house = argv[1]
db = SQL("sqlite:///students.db")

res = db.execute("SELECT first,middle,last,birth FROM students WHERE house = ? ORDER BY last, first", house)

for i in res:
    if i['middle'] != None:
        print(f"{i['first']} {i['middle']} {i['last']}, born {i['birth']}")
    else:
        print(f"{i['first']} {i['last']}, born {i['birth']}")