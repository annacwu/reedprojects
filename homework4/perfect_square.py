def perfect_square(number):
    if(number == 0):
        return True
    elif(number == -1):
        return False
    i = 1
    square = 0
    while(square < number):
        square = i ** 2
        i = i + 1
    if(number % square == 0):
        return True
    else: 
        return False
