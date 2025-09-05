num = input("Integer? ")

if(int(num) % 7 == 0 and int(num) // 10 % 10 == 3):
    print("zap buzz")
elif(int(num) % 7 == 0):
    print("zap")
elif(int(num) // 100 % 10 == 3):
    print("buzz")
elif(int(num) // 10 % 10 == 3):
    print("buzz")
elif(int(num) % 10 == 3):
    print("buzz")
else:
    print(num)
