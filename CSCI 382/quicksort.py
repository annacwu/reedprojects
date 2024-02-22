def partition(lst, start, end):
    piv = lst[end]
    i = start - 1
    for j in range(start, end):
        if lst[j] <= piv:
            i += 1
            lst[i], lst[j] = lst[j], lst[i]
    lst[i + 1], lst[end] = lst[end], lst[i + 1]
    return i + 1

def quicksorthelper(lst, start, end):
    if start < end:
        pivot = partition(lst, start, end)
        quicksorthelper(lst, start, pivot - 1)
        quicksorthelper(lst, pivot + 1, end)

def quicksort(lst):
    start = 0
    end = len(lst) - 1
    quicksorthelper(lst, start, end)