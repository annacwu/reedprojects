def conditional_print(input):

    def evaluate(x):
        if input(x) == True:
            print(x)
    
    return evaluate