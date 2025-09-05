integer = input("Enter a 3-digit integer: ")
firstdigit = int(integer) % 10
seconddigit = int(integer) // 10 % 10
thirddigit = int(integer) // 10 // 10 % 10
print(str(firstdigit)+ str(seconddigit) + str(thirddigit))