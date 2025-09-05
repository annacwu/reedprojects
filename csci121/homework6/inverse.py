def inverse(d):
    # new dictionary
    d2 = {}
    # list containing values of the items in the new dictionary
    newValues = []
    # for every key in the original dictionary
    for key in d: 
        # add that key to the list of values
        newValues.append(key)
       
        if d[key] in d2:
            d2[d[key]].append(key)
        else:
            d2[d[key]] = newValues
        newValues = []

    return d2