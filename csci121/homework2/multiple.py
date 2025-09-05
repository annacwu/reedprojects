num1 = input("Enter an integer: ")
num2 = input("Enter another integer: ")
if(int(num2) == 0 and int(num1) == 0):
    print(str(num1) + " is a multiple of " + str(num2) + ".")
elif(int(num2) == 0):
    print(str(num1) + " is not a multiple of " + str(num2) + ".")
elif (int(num1) % int(num2) == 0):
    print(str(num1) + " is a multiple of " + str(num2) + ".")
else: 
    print(str(num1) + " is not a multiple of " + str(num2) + ".")