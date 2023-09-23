def reverse_number(n):
    reverse = ""
    number = 0
    if(n == 0):
        return 0
    else:
        while(n != 0):
            number = n % 10
            reverse = reverse + str(number)
            n = n // 10
        return int(reverse)
