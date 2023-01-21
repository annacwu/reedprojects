def euclid(a,b):
    while(a != b):
       if(a < b):
           b = b - a
       if(b < a):
           a = a - b
    if(a == 1 or b == 1):
        return False
    else:
        return True
        
            

