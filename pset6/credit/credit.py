from cs50 import get_int
from sys import exit

while True:
    creditNo = get_int("Number: ")
    creditNo = str(creditNo)
    length = len(creditNo)
    if length <= 16 or length >= 13:
        break

def main():
    if validityCheck(creditNo, length) == True:
        print(getBank(creditNo, length))
    else:
        print("INVALID")

def validityCheck(n, length):
    double = []
    single = []
    for i in range(length - 2, -1, -2):
        double.append(int(n[i]) * 2)
    for i in range(length - 1, -1, -2):
        single.append(int(n[i]))
    for ele in double:
        if ele > 0 and ele < 10:
            single.append(ele)
        elif ele >= 10:
            single.append(int(str(ele)[0]))
            single.append(int(str(ele)[1]))

    if sum(single) % 10 == 0:
        return True
    else:
        return False

def getBank(n, length):
    amex = ["37", "34"]
    mastercard = ["51", "52", "53", "54", "55"]

    if length == 15 and n[0:2] in amex:
        return "AMEX"
    elif length == 16 and n[0:2] in mastercard:
        return "MASTERCARD"
    elif (length == 13 or length == 16) and n[0] == "4":
        return "VISA"
    else:
        return "INVALID"

main()