def remove_duplicates(xs):
    j = 0
    i = 0

    while i < len(xs):
        j = 1
        while i + j < len(xs):
            if(xs[i + j] == xs[i]):
                del xs[i + j]
                j -= 1
            j += 1
        i += 1
    
    return None