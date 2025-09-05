def average(f,start,end):
    divisor = end - start
    sum = 0
    i = 0
    while i <= divisor: 
        sum += f(start + i)
        i += 1
    return sum / (divisor + 1)