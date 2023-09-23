def primes_list(n):
    primes = []
    x = 2
    y = 1
    check = []

    while len(primes) < n:
        while y < x:
            z = x % y
            if z == 0:
                check.append(y)
            y += 1
        if len(check) < 2:
            primes.append(x)
        x += 1
        y = 1
        check = []

    return primes
 