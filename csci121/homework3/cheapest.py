def cheapest(boxes):
    atrips = int(boxes) // 11 
    btrips = int(boxes) // 14 
    if(int(boxes) % 11 != 0):
        atrips = atrips + 1
    if(int(boxes) % 14 != 0):
        btrips = btrips + 1
    alice = atrips * 200
    bob = btrips * 250
    if (alice < bob):
        return "Alice"
    elif(bob < alice):
        return "Bob"
    elif (alice == bob):
        return "Alice"
