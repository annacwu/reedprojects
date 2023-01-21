main = input("Enter a non-negative integer: ")
pos = input("Enter a digit position: ")
output = int(main) // 10 ** int(pos) % 10
print("The " + str(10**int(pos)) + "s digit of that integer is " + str(output) + ".")
