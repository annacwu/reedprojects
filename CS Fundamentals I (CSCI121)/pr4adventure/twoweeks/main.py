from room import *
from player import *
from item import *
from monster import *
from characters import *
import os
import updater

player = Player()

def createWorld():
    # creating all the rooms of the world.
    trill = Room("You are in Trillium")
    # some descriptions also detail room-specific commands
    gym = Room("You are in the gym\n\nYou can do the following things in this room:\nWorkout")
    pab = Room("You are in the PAB\n\nYou can do the following things in this room:\nPractice guitar")
    lib = Room("You are in the library")
    out = Room("You are on campus")
    el = Room("You are in Eliot Hall")
    etc = Room("You are in the ETC")
    dorm = Room("You are in your room\n\nYou can do the following things in this room:\nCall family\nPlay switch")
    tut = Room("You are in CSCI121 tutoring\n\nYou can do the following things in this room:\nAsk for help")
    kit = Room("You are in the kitchen\n\nYou can do the following things in this room:\nCook\nClean\nSit")
    com = Room("You are in the common room\n\nYou can do the following things in this room:\nHangout")
    calc = Room("You are in Calculus Lecture")
    vol = Room("You are in Vollum")
    cs = Room("You are in CSCI121 Lecture")
    jo = Room("You are in Jim's Office")
    sev = Store("You are in 7-11")
    cafe = Store("You are in Commons Cafe")

    # connecting all the rooms
    Room.connectRooms(out, "Trillium", trill, "campus")
    Room.connectRooms(out, "Gym", gym, "campus")
    Room.connectRooms(out, "PAB", pab, "campus")
    Room.connectRooms(out, "Library", lib, "campus")
    Room.connectRooms(out, "Eliot", el, "campus")
    Room.connectRooms(out, "ETC", etc, "campus")
    Room.connectRooms(out, "Vollum", vol, "campus")
    Room.connectRooms(out, "7-11", sev, "campus")
    Room.connectRooms(trill, "Dorm", dorm, "Trillium")
    Room.connectRooms(trill, "Kitchen", kit, "Trillium")
    Room.connectRooms(trill, "CommonRoom", com, "Trillium")
    Room.connectRooms(etc, "Tutoring", tut, "ETC")
    Room.connectRooms(el, "Calculus", calc, "Eliot")
    Room.connectRooms(vol, "CSLecture", cs, "Vollum")
    Room.connectRooms(lib, "JimOffice", jo, "Library")
    Room.connectRooms(out, "Commons", cafe, "campus")

    #creating items to put in various rooms, often relating to room-specific commands
    i = Item("Rock", "This is just a rock.", 10)
    i.putInRoom(out)
    switch = Item("Nintendo Switch", "Type 'play switch' to use.", 25)
    switch.putInRoom(dorm)
    guitar = Item("Guitar", "Type 'practice guitar' to use.", 30)
    guitar.putInRoom(pab)
    weights = Item("Weights", "You can use these to workout (type 'workout')", 100)
    weights.putInRoom(gym)
    cream = Food("Ice cream", "Your stolen ice cream!", 2, 15)
    cream.putInRoom(jo)
    clock = Item("Clock", "It would appear someone tried to freeze time.", 10)
    desk = Item("Desk", "You can study at this desk.", 50)
    desk.putInRoom(dorm)
    desk.putInRoom(lib)
    desk.putInRoom(tut)
    board = Item("Whiteboard", "Someone has drawn a diagram of a linked list.", 150)
    board.putInRoom(tut)
    proj = Item("CS Project", "You should get help from the tutors on this.\nType 'ask for help'", 0)
    proj.putInRoom(tut)
    # creating sleep objects
    bed = Sleep("Bed", "You can sleep in this bed.", 50)
    bed.putInRoom(dorm)
    couch = Sleep("Couch", "You can sleep on this couch.", 50)
    couch.putInRoom(com)
    couch.putInRoom(jo)
    couch.putInRoom(pab)
    couch.putInRoom(lib)
    # creating containers
    fridge = Container("Fridge", "Usually holds food, has been known to house random items.", 75, 16)
    fridge.putInRoom(kit)
    fridge.addItem(clock)
    pack = Container("Backpack", "Can use to carry items.", 5, 20)
    pack.putInRoom(dorm)
    # creating sale objects for currency actions
    rice = Sale("Rice Bowl", "Satiates hunger.", 7, 20, 10)
    rice.putInRoom(cafe)
    rice.putInRoom(cafe)
    cookie = Sale("Cookie", "Delicious", 2, 4, 1.50)
    cookie.putInRoom(cafe)
    cookie.putInRoom(sev)
    chips = Sale("Chips", "Addicting", 1, 3, 4)
    chips.putInRoom(sev)
    chips.putInRoom(sev)
    chips.putInRoom(sev)
    cream = Sale("Ice Cream", "Do you scream for ice cream?", 2, 15, 6)
    cream.putInRoom(cafe)
    cream.putInRoom(cafe)
    cream.putInRoom(sev)
    cream.putInRoom(sev)
    burger = Sale("Hamburger", "People wait in hour long lines for this.", 5, 17, 8)
    burger.putInRoom(cafe)
    burger.putInRoom(cafe)
    
    # set player location
    player.location = trill 

    # creating "monsters" -- homework type and boss (exam) type
    Boss("Derivatives Midterm", 50, calc, "derivative knowledge")
    Boss("CS Midterm", 30, cs, "recursion knowledge")
    Boss("Calculus Final", 100, calc, "calculus knowledge")
    Boss("CS Final", 100, cs, "cs fundamental knowledge")
    Monster("Homework", 25, trill)
    '''Proj("CS Project 4", 100, lib, "python coding ability") 
    -- was going to add this, did not have the scope to come up 
    with entirely new type of monster, idea was going to be to have stages of 
    completion, costs time to do, need to go to tutoring to complete a stage'''

    # creating characters
    Character("Rival", "I've hidden your ice cream somewhere on campus because I hate you.", trill)
    Character("Audrey", "At least you're not a biology major.", trill)
    Character("Habibi", "Habibi stares at you.", out)
    Character("Ryan", "I haven't started my project either!", vol)
    Character("James", "Not eating enough? Bad take.", out)
    Character("Daphne", "Did you see the ice cream in the PAB? Weird.",lib)
    Character("Larry", "You can find exam answers by listening to lecture.", tut)
    Character("Julia", "I called my family recently for some extra cash.", com)
    Character("Jameson", "Don't freak out, but I think we might be in a videogame...", com)
    Character("Luka", "This is me saying something cool.", dorm)
    Character("Cashier", "Take care of yourself.", sev)
    Character("Popeye", "Yeah I wear button ups to the gym, what about it?", gym)
    Character("Eleri", "Wanna do my makeup?", kit)
    Character("Gabriella", "Let's get lunch sometime!", cafe)
    Character("Emily", "Tutoring is so helpful!", el)
    Character("Will", "CompComp is killing me.", etc)
    Character("Leo", "I tried to freeze time recently but it didn't work.", pab)
    Character("Lyell", "Hi this is Lyell :)", lib)

    # list of phrases for professor to rotate through
    cslec = ["A recursive program that calls itself.", "Environments all start with the global frame.", "A while loop needs a setup, an algorithm within the loop, and an increment that will eventually end the loop.", "Use .append to add something to the end of a list.", "You cannot search a dictionary by index.", "% is the module symbol, which returns the remainder of a division computation."]
    # making professor using list and adding to room
    Prof("Jim", "I'm your awesome CSCI121 Professor!", cslec, cs)
    Prof("Jim", "I'm your awesome CSCI121 Professor!", cslec, jo)
    mathlec = ["The derivative of x^2 is 2x.", "An integral is used to find the area of a function.", "The integral of sin x is -cos x.", "The limit definition of a function is lim h->0 f(x) = (f(x+h) - f(x))/h.", "The derivative and integral of e^x is e^x.","If the integral is indefinite, always add +C to the end of the integrand."]
    Prof("Bob", "I teach calculus.", mathlec, calc)

def clear():
    os.system('cls' if os.name == 'nt' else 'clear')

def printSituation():
    clear()
    print(player.location.desc)
    print()
    if player.location.hasMonsters():
        print("This room contains the following assignments:")
        for m in player.location.monsters:
            print(m.name)
        print()
    # if the room they are in is a store
    if player.location.type == "store":
        # print the store items
        if player.location.hasSaleItems: 
            print("This store is selling the following items:")
            for k in player.location.saleitems:
                print (k.name)
            print()
    if player.location.hasItems():
        print("This room contains the following items:")
        for i in player.location.items:
            print(i.name)
        print()
    if player.location.hasCharacters():
        print("This room contains the following people:")
        for p in player.location.characters: 
            print(p.name)
        print()
    print("You can go to the following places:")
    for e in player.location.exitNames():
        print(e)
    print()

# method that prints if hunger is too high
def printHungerWarning():
    clear()
    print("Your hunger is dangerously high. You will lose happiness 5x faster now and die if your hunger is greater than 20.")
    print()
    input("Press enter to continue...")

# method that prints if lack of sleep is too much
def printSleepWarning():
    clear()
    print("You haven't slept in " + str(int(player.sleep)) + " hours, which is dangerously high. You will lose happiness 5x faster now and die if you don't sleep in the next " + str(int(36-player.sleep)) + "hours.")
    print()
    input("Press enter to continue...")

# method that prints if happiness runs out
def printHappyWarning():
    clear()
    print("Your happiness is getting low. If it drops to 0 you will drop out of Reed and fail.")
    print()
    input("Press enter to continue...")

# method that prints all game commands and instructions
def showHelp():
    clear()
    print("To win the game, you need to defeat all your assignments, and have a\nhappiness level greater than 50. As time goes on idly, you lose happiness and\nknowledge and your needs to sleep and eat increase. There are four assignments: the Derivatives Midterm, the Recursion Quiz, the CS Final, and the Calculus Final. \nGood luck.")
    print()
    print("enter <location> -- moves you into the given location")
    print("inventory -- opens your inventory")
    print("pickup <item> -- picks up the item")
    print("drop <item> -- removes item from inventory and places it back in the world")
    print("open <container> -- displays items in the specified container")
    print("put <item> in <container> -- places specified item into container from inventory")
    print("talk to <character> -- talks to specified character (+1 happiness)")
    print("eat <item in inventory> -- decrease hunger by specified amount upon inspection")
    print("buy <item> -- if item is purchasable, spend money to buy it")
    print("study -- if there is a desk in the room, you study for 1 hour (+2 knowledge)")
    print("sleep on <item> -- player sleeps for 8 hours (+10 happiness)")
    print("listen to <professor> -- listen to professor's lecture for 1 hour (+5 knowledge)")
    print("do <assignment> -- if possible use knowledge/happiness to complete an assignment")
    print("wait -- one hour of time passes")
    print("me -- displays your current status")
    print("exit -- exit the game")
    print("There are also some room specific commands that are detailed for those rooms.")
    print()
    input("Press enter to continue...")

# print player characteristics -- called by "me" command
def showStatus():
    clear()
    print("Happiness: " + str(player.happiness))
    print("Knowledge: " + str(int(player.know)))
    print("Hunger: " + str(player.hunger))
    print("Time Awake: " + str(int(player.sleep)) + " hours")
    print("Time Left: " + str(int(player.time)) + " hours")
    input("\nPress enter to continue...")

# function that uses a player method to check if player has all the required special knowledge to win
def checkWin():
    if player.checkKnowledge("derivative knowledge") and player.checkKnowledge("recursion knowledge") and player.checkKnowledge("cs fundamental knowledge") and player.checkKnowledge("calculus knowledge"):
        return True
    return False

createWorld()
playing = True
victory = False
clear()
print("You are in the last two weeks of your first semester at Reed, approximately 336 hours left before winter break.\n\nYour goal is to pass all your exams/complete all your assignments while maintaining your happiness.\n\nThe challenge: you are a computer science major.\n\n")
input("Press enter to continue...")
clear()
while playing and player.alive and player.time > 0 and not victory:
    printSituation()
    # if the player has all the items and high enough happiness, 
    if checkWin() and player.happiness >= 50:
        # they win
        victory = True
    commandSuccess = False
    timePasses = False
    while not commandSuccess:
        commandSuccess = True
        command = input("What now? ")
        commandWords = command.split()
        # if they just press enter, 
        if len(commandWords) == 0:
            # nothing happens, ask again
            commandSuccess = False

        # if they type enter 
        elif commandWords[0].lower() == "enter": 
            target = commandWords[1].lower()
            for i in player.location.exits: 
                if commandWords[1].lower() == i[0].lower(): 
                    target = commandWords[1].lower()
                    break
                else: 
                    target = False
            # if the room exists and is connected to current room
            if target != False: 
                # go that direction
                player.goDirection(commandWords[1])
                # it takes 10 minutes
                player.time -= 0.1
            else: 
                print("No such place.")
                commandSuccess = False

        # if command is pickup
        elif commandWords[0].lower() == "pickup":  
            targetName = command[7:].lower()
            target = player.location.getItemByName(targetName)
            if target != False:
                # if the item weighs more than inventory capacity
                if target.weight > 20:
                    # print failure
                    print("This item is too heavy to carry.")
                    commandSuccess = False
                # else, if the player has room for it
                elif player.inventoryWeight(target.weight):
                    # pick it up
                    player.pickup(target)
                # otherwise, print failure
                else: 
                    print("You are already carrying too many items.")
                    commandSuccess = False
            else:
                print("No such item.")
                commandSuccess = False

        # if command is inventory
        elif commandWords[0].lower() == "inventory":
            # call method that prints inventory items
            player.showInventory()    

        # if command is me        
        elif commandWords[0].lower() == "me":
            # show player characteristics
            showStatus()

        # if command is inspect    
        elif commandWords[0].lower() == "inspect":
            targetName = command[8:].lower()
            target = player.location.getItemByName(targetName)
            # if the target doesn't exist in the room
            if target == False: 
                # check if it is in the inventory
                target = player.getItemByName(targetName)
            # if it exists
            if target != False:
                # print the item's description
                clear()
                print(target.describe())
            # otherwise, print that it doesn't exist
            else:
                print("No such item.")
                commandSuccess = False

        # if command is drop        
        elif commandWords[0].lower() == "drop":
            targetName = command[5:].lower()
            target = player.getItemByName(targetName)
            # the item is in the players inventory
            if target != False:
                # if the item isnt a special knowledge type
                if target.type != "knowledge":
                    # drop it
                    player.drop(target)
                # otherwise, do not let them drop it
                else: 
                    print("Why would you try to drop this?")
                    commandSuccess = False
            # otherwise, print that item doesn't exists
            else:
                print("No such item.")
                commandSuccess = False

        # if command is wait        
        elif commandWords[0].lower() == "wait":
            # pass an hour (last .1 is in the timepasses)
            player.time -= .9
            timePasses = True

        # if command is talk to    
        elif commandWords[0].lower() == "talk":
            targetName = command[8:].lower()
            # check character list
            target = player.location.getCharacterByName(targetName)
            # if the target exists
            if target != False:
                # call talk method to print what the character says
                target.talk()
                # increase happiness
                player.happiness += 2
                timePasses = True
            else:
                print("No such person.")
                commandSuccess = False

        # if command is eat        
        elif commandWords[0].lower() == "eat":
            targetName = command[4:]
            # if item is in inventory
            target = player.getItemByName(targetName)
            if target != None:
                # if the target is food or purchased food
                if target.type == "food" or target.type == "sale":
                    # remove item from inventory and eat it (decrease hunger)
                    player.removeItem(target)
                    player.inventory -= target.weight
                    # if the hunger would be below 0
                    if player.hunger - target.hp < 0:
                        # set it to 0
                        player.hunger = -1
                        player.happiness += 3
                    else:
                        player.hunger -= target.hp - 1
                        player.happiness += 3
                    # print the change in player condition
                    clear()
                    print("Your hunger has decreased by " + str(target.hp) + ".")
                    input("\nPress enter to continue....")
                    timePasses = True
                # otherwise, print that the item is not consumable type
                else: 
                    print("You cannot eat this.")
                    commandSuccess = False
            # otherwise, print that the item is not in player inventory
            else:
                print("You are not holding this item.")
                commandSuccess = False

        # if commmand is study        
        elif commandWords[0].lower() == "study":
            # local helper variable
            able = False
            for i in player.location.items:
                # if there is a desk in the room (search room's items list)
                if i.name == "Desk":
                    # player is able to study
                    able = True
                    break
            # if they are able to study
            if able == True:
                # spend an hour, increase knowledge, print outcome
                player.time -= 1
                player.know += 3
                clear()
                print("You studied for one hour. Your knowledge has increased by 2.")
                print()
                input("Press enter to continue...")
                timePasses = True
            # otherwise, print failure
            else: 
                print("There is not a desk in this room.")
                commandSuccess = False

        # if command is listen to         
        elif commandWords[0].lower() == "listen":
            targetName = command[10:]
            target = player.location.getCharacterByName(targetName)
            # if the character is professor type
            if target != False and target.type == "prof":
                # call lecture method in prof class, print outcome
                clear()
                print(target.lecture())
                print()
                input("Press enter to continue...")
                clear()
                print("Your knowledge has increased by 5.\nYour happiness has decreased by 10.")
                print()
                input("Press enter to continue...")
                # adjust player characteristics accordingly
                player.happiness -= 10
                player.know += 5
                player.time -= 1
                timePasses = True
            # otherwise, print silence message
            else:
                clear()
                print("*crickets*")
                print()
                input("Press enter to continue...")

        # if command is sleep        
        elif commandWords[0].lower() == "sleep":
            targetName = command[9:]
            target = player.location.getItemByName(targetName)
            # if the specified item is in the room
            if target != False:
                # if the item is a sleep type
                if target.type == "sleep":
                    # adjust characteristics accordingly
                    player.time -= 8
                    player.timepoint = player.time
                    player.happiness += 10
                    player.hunger += 7
                    timePasses = True
                    # print outcome
                    clear()
                    print("You slept for 8 hours! +10 happiness")
                    print()
                    input("Press enter to continue...")
                else: 
                    print("You cannot sleep on that.")
                    commandSuccess = False
            # otherwise, print failure
            else:
                print("There is nothing to sleep on in this room.")
                commandSuccess = False

        # if command is open       
        elif commandWords[0].lower() == "open":
            targetName = command[5:]
            target = player.location.getItemByName(targetName)
            # if it wasnt in the room check inventory
            if target == False:  
                target = player.getItemByName(targetName)
            # if target exists
            if target != False:
                # if there are items in the container
                if target.hasItems():
                    # while items remain in the list
                    while len(target.carrying) > 0:
                        # for every item, remove it from container and place it in the room
                        for p in target.carrying:
                            player.location.items.append(p)
                            target.carrying.remove(p)
                            target.carryingweight -= p.weight
                # otherwise, print there is nothing in there
                else: 
                    print("There is nothing in here.")
                    commandSuccess = False
            # otherwise, print failure
            else:
                print("You cannot open this item.")
                commandSuccess = False

        # if command is put        
        elif commandWords[0].lower() == "put":
            itemName = commandWords[1]
            i = 2
            # set item name to be all the words before in
            while commandWords[i] != "in":
                itemName = itemName + " " + commandWords[i] 
                i += 1
            # check if item is in inventory
            target = player.getItemByName(itemName)
            # container will be the word after in 
            containerName = commandWords[i+1]
            targetContainer = player.location.getItemByName(containerName)
            # if there isnt a container in the room, check player inventory
            if targetContainer == False: 
                targetContainer = player.getItemByName(containerName)
            # if the target exists
            if target != None:
                # and the container exists
                if targetContainer != False: 
                    # if the container is too full already, print failure
                    if targetContainer.carryingweight + target.weight > targetContainer.capacity: 
                        print("Too full to hold item.")
                        commandSuccess = False
                    # if it can hold it, 
                    else: 
                        # take item out of room and put it in the container, adjust weights 
                        targetContainer.carrying.append(target)
                        player.items.remove(target)
                        player.inventory -= target.weight
                        targetContainer.carryingweight += target.weight
                        # print outcome
                        clear()
                        print("Placed in " + containerName)
                        print()
                        print(containerName + " now holding:")
                        for p in targetContainer.carrying: 
                            print(p.name)
                        input("\nPress enter to continue...")
                else: 
                    print("No such container.")
                    commandSuccess = False
            else: 
                print("You are not holding this item.")
                commandSuccess = False

        # if command is buy        
        elif commandWords[0].lower() == "buy":
            # if the room they are in is store type
            if player.location.type == "store":
                targetName = command[4:]
                # call sale item method to check specifically for sale itemes
                target = player.location.getSaleItemByName(targetName)
                if target != False:
                    # if player has enough money
                    if player.money - target.cost >= 0:
                        # remove item from sale list, create identical food item in room that can be picked up
                        player.location.removeItem(target)
                        new = Food(target.name, target.desc, target.weight, target.hp)
                        new.putInRoom(player.location)
                        # adjust stats accordingly
                        player.money -= target.cost
                        player.happiness += 2
                        timePasses = True
                    else: 
                        print("Insufficient funds.")
                        commandSuccess = False
                else:
                    print("Item not for sale.")
                    commandSuccess = False
            else: 
                print("You cannot buy things here.")
                commandSuccess = False

        # the following are all room specific commands following the same structure        
        elif commandWords[0].lower() == "workout":
            # if the description of current location matches the specific room for the command
            if player.location.desc == "You are in the gym\n\nYou can do the following things in this room:\nWorkout":
                # carry out associated stat changes
                player.happiness += 10
                player.hunger += 7
                player.time -= 2
                clear()
                print("You can almost do a pull up!\nYou have gained 10 happiness and 7 hunger")
                input("\nPress enter to continue...")
                timePasses = True
            # otherwise, print where they need to be to do the command
            else: 
                print("You need to be in the gym to workout.")
                commandSuccess = False
        elif commandWords[0].lower() == "cook":
            if player.location.desc == "You are in the kitchen\n\nYou can do the following things in this room:\nCook\nClean\nSit":
                if player.money - 1 >= 0:
                    player.happiness += 3
                    if player.hunger - 10 < 0:
                        player.hunger = 0
                    else: 
                        player.hunger -= 10
                    player.time -= 2
                    clear()
                    print("You made some delicious ramen. Your happiness has increased by 3 and your hunger decreased by 10.")
                    input("\nPress enter to continue...")
                    timePasses = True
                else: 
                    print("You do not have enough money to cook ramen. You need at least $1.")
                    commandSuccess = False
            else: 
                print("You need to be in a kitchen to cook.")
                commandSuccess = False
        elif commandWords[0].lower() == "clean":
            if player.location.desc == "You are in the kitchen\n\nYou can do the following things in this room:\nCook\nClean\nSit":
                player.happiness += 5
                player.time -= 1
                clear()
                print("You did the dishes and now everyone is pleased with you. Your happiness has increased by 5.")
                input("\nPress enter to continue...")
                timePasses = True
            else: 
                print("You can only clean the kitchen.")
                commandSuccess = False
        elif commandWords[0].lower() == "sit":
            if player.location.desc == "You are in the kitchen\n\nYou can do the following things in this room:\nCook\nClean\nSit":
                player.time -= .5
                clear()
                print("You sit for half an hour. You should really do some work.")
                input("\nPress enter to continue...")
                timePasses = True
            else: 
                print("You can only sit the kitchen.")
                commandSuccess = False
        elif commandWords[0].lower() == "call":
            if player.location.desc == "You are in your room\n\nYou can do the following things in this room:\nCall family\nPlay switch":
                if len(commandWords) == 1:
                    print("Type 'call family'.")
                    commandSuccess = False
                if commandWords[1].lower() == "family":
                    clear()
                    print("You called your family for one hour. They were happy to hear from you.")
                    print("+$10")
                    print("+5 happiness")
                    input("\nPress enter to continue...")
                    player.money += 10
                    player.time -= 1
                    player.happiness += 5
                    timePasses = True
                else: 
                    print("You should really call your family instead.")
                    commandSuccess = False
            else: 
                print("You can only call in the privacy of your room.")
                commandSuccess = False
        elif commandWords[0].lower() == "play":
            if player.location.desc == "You are in your room\n\nYou can do the following things in this room:\nCall family\nPlay switch":
                if len(commandWords) == 1:
                    print("Type 'play switch'.")
                    commandSuccess = False
                if commandWords[1].lower() == "switch":
                    clear()
                    print("You played Cuphead for 3 hours and screamed at the console the whole time. You had a blast.")
                    print("+10 happiness")
                    input("\nPress enter to continue...")
                    player.time -= 3
                    player.happiness += 11
                    player.hunger += 3
                    timePasses = True
                else: 
                    print("You can't play that.")
                    commandSuccess = False
            else: 
                print("You can only play in your dorm.")
                commandSuccess = False
        elif commandWords[0].lower() == "practice":
            if player.location.desc == "You are in the PAB\n\nYouc an do the following things in this room:\nPractice guitar":
                if len(commandWords) == 1:
                    print("Type 'play guitar'.")
                    commandSuccess = False
                if commandWords[1].lower() == "guitar":
                    clear()
                    print("You learned a new song. I bet people love when you pull that out at parties.")
                    print("+15 happiness")
                    print("-1.5 hours")
                    input("\nPress enter to continue...")
                    player.time -= 1.5
                    player.happiness += 16
                    timePasses = True
                else: 
                    print("You can't play that.")
                    commandSuccess = False
            else: 
                print("You can only practice in the PAB.")
                commandSuccess = False
        elif commandWords[0].lower() == "hangout":
            if player.location.desc == "You are in the common room\n\nYou can do the following things in this room:\nHangout":
                player.time -= 1
                player.happiness += 13
                clear()
                print("You and your friends talked about how much work you have to do.")
                print("\nYou hung out for 1 hour. +12 happiness.")
                print()
                input("Press enter to continue...")
                timePasses = True
            else: 
                print("The common room is really the best place to hangout.")
                commandSuccess = False
        elif commandWords[0].lower() == "ask":
            if player.location.desc == "You are in CSCI121 tutoring\n\nYou can do the following things in this room:\nAsk for help":
                player.time -= 2
                player.know += 11
                clear()
                print("You got help on your cs project. The cs tutors are amazing aren't they?")
                print("\n-2 hours\n+10 knowledge.")
                print()
                input("Press enter to continue...")
                timePasses = True
            else: 
                print("You can only get help in tutoring.")
                commandSuccess = False

        # if command is help
        elif commandWords[0].lower() == "help":
            # call help method to print all commands
            showHelp()

       # if command is exit     
        elif commandWords[0].lower() == "exit":
            # end loop to end game
            playing = False

        # if command is do (this is the 'attack' command)    
        elif commandWords[0].lower() == "do":
            # decipher which monster they have chosen to attack
            targetName = command[3:]
            target = player.location.getMonsterByName(targetName)
            if target != False:
                # if the target is homework
                if target.name.lower() == "homework":
                    # call attack monster command from player
                    player.attackMonster(target)
                    timePasses = True
                # if the target is an exam, do the following: 
                elif target.name.lower() == "derivatives midterm":
                    # if player does not have enough knowledge points
                    if player.know < target.health: 
                        # print command failure message
                        print("You need " + str(target.health - player.know) + " more knowledge points to attempt this assignment.")
                        commandSuccess = False
                    # if they do have enough, 
                    else: 
                        # call attack boss method to check if they attacked successfully
                        if player.attackBoss(target): 
                            # if they did, spawn the special type of knowledge in the room
                            mathknow1 = Special(str(target.drops),"Special knowledge to help complete your goals.", 0)
                            mathknow1.putInRoom(player.location)
                        timePasses = True
                        input("\nPress enter to continue...")
                        commandSuccess = True
                elif target.name.lower() == "cs midterm":
                    if player.know < target.health: 
                        print("You need " + str(target.health - player.know) + " more knowledge points to attempt this assignment.")
                        commandSuccess = False
                    else: 
                        if player.attackBoss(target):
                            csknow1 = Special(str(target.drops),"Special knowledge to help complete your goals.", 0)
                            csknow1.putInRoom(player.location)
                        timePasses = True
                        input("\nPress enter to continue...")
                        commandSuccess = True
                elif target.name.lower() == "calculus final":
                    if player.know < target.health: 
                        print("You need " + str(target.health - player.know) + " more knowledge points to attempt this assignment.")
                        commandSuccess = False
                    if not player.getItemByName("derivative knowledge"): 
                        print("You need derivatives knowledge before you can attempt this.\nTry again after you've completed the derivatives midterm.")
                        commandSuccess = False
                    else: 
                        if player.attackBoss(target):
                            mathknow2 = Special(str(target.drops),"Special knowledge to help complete your goals.", 0)
                            mathknow2.putInRoom(player.location)
                        timePasses = True
                        input("\nPress enter to continue...")
                        commandSuccess = True
                elif target.name.lower() == "cs final":
                    if player.know < target.health: 
                        print("You need " + str(target.health - player.know) + " more knowledge points to attempt this assignment.")
                        commandSuccess = False
                    if not player.getItemByName("recursion knowledge"): 
                        print("You need recursion knowledge before you can attempt this.\nTry again after you've completed the cs midterm.")
                        commandSuccess = False
                    else: 
                        if player.attackBoss(target):
                            csknow2 = Special(str(target.drops),"Special knowledge to help complete your goals.", 0)
                            csknow2.putInRoom(player.location)
                        timePasses = True
                        input("\nPress enter to continue...")
                        commandSuccess = True
                else:
                    print("No such assignment.")
                    commandSuccess = False
            else:
                print("No such assignment.")
                commandSuccess = False

        #cheat codes -- added to test code and surpass time it takes to gather enough knowledge and food
        elif commandWords[0].lower() == "time":
            player.time -= int(commandWords[1])
            timePasses = True
        elif commandWords[0].lower() == "buff":
            player.hunger = 0
            player.sleep = 0
        elif commandWords[0].lower() == "know":
            player.know += 100
        elif commandWords[0].lower() == "hap":
            player.happiness += 100
        # elif commandWords[0].lower() == "die":
        #     player.alive = False
        #     print(player.alive)
        # elif commandWords[0].lower() == "win":
        #     victory = True
        #     playing = False
        else:
            print("Not a valid command")
            commandSuccess = False

    # timepasses condition, called after every successful command
    if timePasses == True:
        # checks if player has fulfilled win conditions
        if checkWin() and player.happiness >= 50:
            victory = True
        # moves all characters and homeworks around
        updater.updateAll()
        # player gets hungrier, more tired, and loses happiness
        player.hunger += 1
        player.time -= .1
        player.happiness -= 1
        player.sleep = player.timepoint - player.time
        player.spawn += 1
        # if it 10 rounds of time have passed
        if player.spawn >= 10:
            # create a new homework monster at player current location and reset spawn count
            Monster("Homework", 25, player.location)
            player.spawn = 0
        # conditionals to check if any player characteristics are getting close to death
        if player.sleep >= 24 and player.sleep < 36: 
            printSleepWarning()
            player.happiness -= 4
        if player.hunger >= 15: 
            printHungerWarning()
            player.happiness -= 4
        if player.happiness <= 12: 
            printHappyWarning()
        # conditionals to check if player has died
        if player.hunger > 20: 
            player.alive = False
            clear()
            print("Your hunger is greater than 20. You can no longer function.")
            input("\nPress enter to continue...")
        if player.happiness <= 0: 
            player.alive = False
            clear()
            print("Your happiness has dropped to 0. You can no longer function.")
            input("\nPress enter to continue...")
        if player.sleep >= 36: 
            player.alive = False
            clear()
            print("You have been awake for 36 hours or more. You can no longer function.")
            input("\nPress enter to continue...")
        if player.time <= 0: 
            playing = False
            clear()
            print("The semester has ended. Time is up.")
            input("\nPress enter to continue...")
        # adjusting knowledge to make sure it doesn't drop below 0
        if player.know - 1 < 0:
            player.know = 0
        else: 
            player.know -= .25
        
        

# if player has died,
if player.alive == False: 
    # print lose message
    clear()
    print("You lose! Better luck next time.")

# alternatively, if player has ended play and fulfilled victory conditions,    
elif victory == True: 
    # print their win message and final stats they ended the game with
    clear()
    print("You somehow managed to pass all your exams and maintain a sane level of happiness. Wow. That means you won! Congrats on making it through your first semester.\n\nSee you in a month for the spring semester!")
    print()
    print("Here are your final stats: ")
    print("Happiness: " + str(player.happiness))
    print("Knowledge: " + str(int(player.know)))
    print("Time Left: " + str(int(player.time)) + " hours")
    print("\nAt the end your inventory held: ")
    for i in player.items:
            print(i.name)
    input("\nThanks for playing, press enter to end session...")
# if they stopped playing and didnt die, but didn't finish the victory conditions before play ended,
elif victory == False: 
    # print loss message
    clear()
    print("You managed to sustain your basic needs (congrats!) but you didn't complete your tasks. You lose. Better luck next time.")