def prefixMax(lst, i):
    if i > 0:
        j = prefixMax(lst, i - 1)
        if lst[i] < lst[j]:
            return j
    return i

def selectionSortHelp(lst, i):
    if i is None: 
        i = len(lst) - 1
    if i > 0:
        j = prefixMax(lst, i)
        lst[i], lst[j] = lst[j], lst[i]
        selectionSortHelp(lst, i-1)

def selectionsort(lst):
    i = None
    selectionSortHelp(lst, i)