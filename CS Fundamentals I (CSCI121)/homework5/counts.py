def counts(n,xs):
    L = []
    i = 0

    while(len(L) < n):
        L.append(0)

    while (i < len(xs)):
        L[xs[i]] += 1
        i += 1

    return L
