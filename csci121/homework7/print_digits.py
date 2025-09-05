def print_digits(n):
    q = n % 10
    print (q)
    if n // 10 > 0:
        print_digits(n // 10)