def max_of_funcs(f,g):
    return lambda h: f(h) if f(h) > g(h) else g(h)
