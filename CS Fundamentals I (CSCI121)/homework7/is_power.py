def is_power(n,b):
    if n == b or n == 1:
        return True
    if b > n or b == 1:
        return False
    return is_power(n / b, b)