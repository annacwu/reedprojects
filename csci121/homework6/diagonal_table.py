def diagonal_table(size):
    
    columns = []
    i = 0
    while i < size:
        rows = []
        j = 0
        while j < size:
            rows.append(0)
            j += 1
        columns.append(rows)
        i += 1

    i = 0
    while i < size:
        columns[i][i] = 1
        i += 1
    return columns