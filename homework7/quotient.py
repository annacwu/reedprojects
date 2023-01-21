def quotient(n,d):
    if n < d:
        return 0
    return 1 + quotient(n-d, d)
