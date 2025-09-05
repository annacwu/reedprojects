def down_up(n):
    x = [n]
    y = n
    i = n
    while (i > 1):
        i = y - 1
        x.append(i)
        y = y - 1
    while (i < n):
        i = y + 1
        x.append(i)
        y = y + 1
    return x
