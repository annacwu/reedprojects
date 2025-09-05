def word_count(s):
    d = {}
    sentence = s.split(' ')
    for i in sentence:
        if i in d:
            d[i] += 1
        else:
            d[i] = 1
    return d