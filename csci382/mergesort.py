def merge(lst, start, mid, end):
    left = lst[start:mid]
    right = lst[mid:end]
    i, j, k = 0, 0, start
    while i < len(left) and j < len(right):
        if left[i] <= right[j]:
            lst[k] = left[i]
            i += 1
        else: 
            lst[k] = right[j]
            j += 1
        k += 1
    if i < len(left):
        for x in range(i, len(left)):
            lst[k] = left[x]
            k += 1
    if j < len(right):
        for y in range(j, len(right)):
            lst[k] = right[y]
            k += 1
    return lst

# it wouldn't alter the list in place unless we returned the result of merge
# so this doesn't create a new list but it does return
def mergesorthelp(lst):
    end = len(lst)
    start = 0

    if start >= end - 1: 
        return lst
    
    mid = (start + end) // 2

    lst[start:mid] = mergesorthelp(lst[start:mid])
    lst[mid:end] = mergesorthelp(lst[mid:end])

    lst = merge(lst, start, mid, end)
    return lst

# so this just runs the above but without returning 
def mergesort(lst):
    lst = mergesorthelp(lst)



