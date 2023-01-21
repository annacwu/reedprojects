def divisors(number):
    stringStart = "The divisors of "  + str(number) + " are "
    divmany = ""

    n = 1
    count = 1

    if(number == 1):
        return "The divisor of 1 is 1."
    elif(number == 0):
        return "The divisor of 0 is 0."
    else:
        while (n < number):
            if(number % n == 0):
                divmany = divmany + str(n) + ", "
                count = count  + 1
            n += 1
        if(count == 2):
            return stringStart + "1 and " + str(number) + "."
        else:
            return stringStart + divmany + "and " + str(number) + "."