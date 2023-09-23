def brick_maker(symb):
    return lambda a, b: (str(symb*a) + "\n") * b