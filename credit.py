import cs50

original_num = cs50.get_string("Number: ")
lenght = len(original_num)
#multiply every other digit by two
x = lenght - 2
result = 0

while x >= 0:
    tmp = str(int(original_num[x]) * 2)
    if len(tmp) > 1:
        result = result + int(tmp[0]) + int(tmp[1])
    else:
        result = result + int(tmp)
    x -=2

#adding remaining digits
x = lenght - 1
while x >= 0:
    result = result + int(original_num[x])
    x -= 2

amex_num = ["34","37"]
mc_num = ["51","52","53","54","55"]
#check if mod = 0 after division by 10
if result % 10 != 0:
    print("INVALID")
#check for credit card type
elif lenght == 15 and original_num[0:2] in amex_num:
    print("AMEX")
elif lenght == 16 and original_num[0:2] in mc_num:
    print("MASTERCARD")
elif lenght == 13 or lenght == 16 and original_num[0:1] == "4":
    print("VISA")
else:
    print("INVALID")
