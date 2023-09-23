def print_zap_buzz(number):
    if(int(number) % 7 == 0 and int(number) // 10 % 10 == 3):
        print("zap\nbuzz")
    elif(int(number) % 7 == 0):
        print("zap")
    elif(int(number) // 100 % 10 == 3):
        print("buzz")
    elif(int(number) // 10 % 10 == 3):
        print("buzz")
    elif(int(number) % 10 == 3):
        print("buzz")
    else:
        return None