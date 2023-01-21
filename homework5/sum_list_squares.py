def sum_list_squares(n):
    i = 0
    sum = 0
    while (i < len(n)):
        sum = sum + (n[i] ** 2)
        i += 1
    return sum