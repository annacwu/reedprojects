def stars(width, height):
    line = "* " * (width // 2)
    line2 = " *" * (width // 2)
    if(width % 2 != 0):
        line = line + "*"
        line2 = line2 + " "
    if(height % 2 == 0):
       s = ((str(line) + "\n" + str(line2) + "\n") * (height // 2))
    elif(height % 2 != 0):
        s = ((str(line) + "\n" + str(line2) + "\n") * (height // 2) + str(line) + "\n")
    return s
