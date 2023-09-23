from math import sqrt
x1 = input("Location x-coordinate? ")
y1 = input("Location y-coordinate? ")
x2 = input("Classroom x-coordinate? ")
y2 = input("Classroom y-coordinate? ")
print("Distance:")
print(sqrt(((float(x2) - float(x1)) ** 2) + ((float(y2) - float(y1)) ** 2)))