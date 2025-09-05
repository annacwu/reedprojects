def coins(x):
    quart = x // 25 
    dime = x % 25 // 10
    nick = x % 25 % 10 // 5
    pen = x % 25 % 10 % 5 
    num = quart + dime + nick + pen
    return num
