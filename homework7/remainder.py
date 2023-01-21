def remainder(n,d):
    x = n - d
    if n < d:
        return n
    return remainder(x, d)
        
def remainder2(n,d):
    if n < d: 
        return n
    return remainder2(n - d, d)