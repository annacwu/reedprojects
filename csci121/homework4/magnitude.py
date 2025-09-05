def magnitude(n):
    count = 0
    if(n == 0):
        return 0
    elif(n == 1):
        return 1
    elif(n > 1):
        while(n > 0):
            n = n // 2
            count = count + 1
        return count
    