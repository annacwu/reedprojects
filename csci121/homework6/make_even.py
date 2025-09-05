def make_even(xs):
    i = 0
    while i < len(xs):
        j = xs[i]
        if j % 2 != 0:
            j -= 1
            xs[i] = j
        i += 1
    return None
