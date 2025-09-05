def collatz_length(n):
    if n == 1:
        return 1
    elif n % 2 != 0:
        return 1 + collatz_length(n * 3 + 1)
    elif n % 2 == 0:
        return 1 + collatz_length (n / 2)
    