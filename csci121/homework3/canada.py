def total(dol, cen):
    if(cen == 0):
        Tdol = int(dol) * amount
        Tcen = int(cen) * amount
        print("Your total is $" + str(Tdol) + ".00 Canadian.") 
    else:
        Tdol = int(dol) * amount
        Tcen = int(cen) * amount
        if(Tcen > 99):
            Tdol = float(Tdol + Tcen / 100)
            print("Your total is $" + str(Tdol) + "0 Canadian.") 
        else: 
            print("Your total is $" + str(Tdol) + "." + str(Tcen) + " Canadian.") 


def convert(dol, cen):
    Tdol = (int(cen) / 100) * amount + dol * amount
    if((cen * amount) % 100 != 0):
        Tdol = int(Tdol) + 1
    toon = int(Tdol) // 2 
    if(toon == 0):
        input("Please give me 1 loonie. [hit RETURN]: ")
    elif(Tdol == 3):
        input("Please give me 1 toonie and 1 loonie. [hit RETURN]: ")
    elif(toon == 1):
        input("Please give me 1 toonie. [hit RETURN]: ")
    elif(Tdol % 2 == 1):
        input("Please give me " + str(toon) + " toonies and 1 loonie. [hit RETURN]: ")
    elif(Tdol % 2 == 0):
        input("Please give me " + str(toon) + " toonies. [hit RETURN]: ")
    elif(cen % 2 == 0):
        input("Please give me " + str(toon) + " toonies and 1 loonie. [hit RETURN]: ")


print("||||          ||||")
print("||||  _/TT\_  ||||")
print("||||  \\\||//  ||||")
print("||||  '-||-'  ||||")
print("||||          ||||")
print("Welcome to Canada!")
print("------------------")
print()

temp = int(input("Enter the current temperature in whole degrees Celsius: "))
f = int((temp)*(9/5) + 32)
print("That means that it is " + str(f) + " degrees Fahrenheit outside.")
print()

print("Let's get the cost of potato chips here in Canadian dollars and cents...")
dol = int(input("Enter the number of dollars per bag of chips: "))
cen = int(input("Enter the number of cents per bag of chips: "))
amount = int(input("Okay. Enter the number of bags would you like to purchase: "))
if(amount == 0):
    print()
    print("Okay! Thanks for chatting about our beautiful weather.")
else: 
    flavor = input("Enter the chip flavor you prefer [plain, pickle, or ketchup]: ")
    if(flavor != "plain" and flavor != "pickle" and flavor != "ketchup"):
        print("Sounds tasty, but we can only offer you plain chips.")
        print()              
        if(dol == 0 and cen == 0):
            print("Your total is $0.00 Canadian.\nEnjoy your free plain chips!")
        else:
            total(dol, cen)
            convert(dol, cen)
            print()
            print("Thank you! Here are your plain chips.")
    else:
        print()
        if(dol == 0 and cen == 0):
            print("Your total is $0.00 Canadian.\nEnjoy your free " + flavor + " chips!")
        else:
            total(dol, cen)
            convert(dol, cen)
            print()
            print("Thank you! Here are your " + str(flavor) + " chips.")
