from sys import argv, exit
import csv
from cs50 import SQL

if len(argv) != 2:
    print("Usage: import.py file.csv")
    exit(1)

dbfile = open('students.db', 'w')

db = SQL("sqlite:///students.db")

db.execute("""CREATE TABLE students (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    first VARCHAR(255),
    middle VARCHAR(255),
    last VARCHAR(255),
    house VARCHAR(10),
    birth INTEGER
);""")

with open("characters.csv", 'r') as file:
    reader = csv.DictReader(file)

    for row in reader:
        name = row['name'].split()
        house = row['house']
        birth = row['birth']

        if len(name) == 2:
            db.execute("INSERT INTO students (first, last, house, birth) VALUES (?,?,?,?);", name[0], name[1], house, birth)
        elif len(name) == 3:
            db.execute("INSERT INTO students (first, middle, last, house, birth) VALUES (?,?,?,?,?);", name[0], name[1], name[2], house, birth)

dbfile.close()
exit(0)