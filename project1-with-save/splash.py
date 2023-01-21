from random import choice

def greet():
    tiles = ["|x","| "]
    _row = "+-"*20+"+"
    row1 = ''.join([choice(tiles) for _ in range(20)]) + "|"
    row2 = ''.join([choice(tiles) for _ in range(20)]) + "|"
    row3 = ''.join([choice(tiles) for _ in range(20)]) + "|"

    print(_row)
    print(row1)
    print(_row)
    print(row2)
    print(_row)
    print("|x| |x| | Anna Wu Project 1!!! | |x|x| |")
    print(_row)
    print(row3)
    print(_row)

    
