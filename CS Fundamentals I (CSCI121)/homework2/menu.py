name = input("Restaurant name? ")
e1 = input("First entree? ")
p1 = input("First entree price? ")
e2 = input("Second entree? ")
p2 = input("Second entree price? ")
e3 = input("Third entree? ")
p3 = input("Third entree price? ")
print()
print("1234567890" * 4)
print()
print(name + " Entrees" )
print("-"*(len(name + " Entrees")))
print(e1 + "."*(36 - len(str(e1))) + "$" + " "*(3 - len(p1)) +str(p1))
print(e2 + "."*(36 - len(str(e2))) + "$" + " "*(3 - len(p2)) +str(p2))
print(e3 + "."*(36 - len(str(e3))) + "$" + " "*(3 - len(p3)) +str(p3))