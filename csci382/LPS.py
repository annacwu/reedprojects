def LPS(char):
    p = [[-1 for _ in range(len(char))] for _ in range(len(char))]
    return LPS_helper(char, 0, len(char) - 1, p)


def LPS_helper(char, i, j, p):

    if p[i][j] != -1: 
        return p[i][j]

    if i == j: 
        p[i][j] = 1
        return 1
    
    if j == i + 1 and char[i] == char[j]:
        p[i][j] = 2
        return 2
    
    if j == i + 1 and char[i] != char[j]:
        p[i][j] = 1
        return 1
    
    if j > i + 1 and char[i] == char[j]:
        res = LPS_helper(char, i + 1, j - 1, p) + 2
        p[i][j] = res
        return res
    
    if j > i + 1 and char[i] != char[j]:
        res = max(LPS_helper(char, i, j - 1, p), LPS_helper(char, i+1, j, p))
        p[i][j] = res
        return res