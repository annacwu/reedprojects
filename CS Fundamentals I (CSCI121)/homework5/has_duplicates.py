def has_duplicates(xs):
    seen = 0
    j = 0
    i = 0

    while i < len(xs):
        x = xs[i]
        j = 1
        while j + i < len(xs):
            if(xs[i + j] == x):
                seen += 1
            j += 1
        i += 1

    if seen > 0:
        return True
    else: 
        return False