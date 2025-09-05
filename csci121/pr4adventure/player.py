import os
import random

def clear():
    os.system('cls' if os.name == 'nt' else 'clear')

class Player:

    # initializes all the player characteristics
    def __init__(self):
        self.location = None
        self.items = []
        self.inventory = 0
        self.hunger = 0
        self.happiness = 50
        self.know = 0
        self.time = 336
        self.timepoint = 336
        # sleep is based on saved timepoints 
        self.sleep = self.time - self.timepoint
        self.alive = True
        self.money = 10
        self.spawn = 0
    def goDirection(self, direction):
        self.location = self.location.getDestination(direction.lower())
    def pickup(self, item):
        self.items.append(item)
        item.loc = self
        self.inventory += item.weight
        self.location.removeItem(item)

    # drop method based of previously written pickup method, removes items from rooms instead of adding    
    def drop(self,item):
        self.items.remove(item)
        self.inventory -= item.weight
        item.loc = self.location
        item.putInRoom(self.location)

    # slightly altered show inventory method to include inventory weight and currency    
    def showInventory(self):
        clear()
        print("Inventory weight: " + str(self.inventory) + "/20")
        print("Money: $" + str(self.money))
        print("\nYou are currently carrying:")
        print()
        for i in self.items:
            print(i.name)
        print()
        input("Press enter to continue...")

    # attack monster method (altered from original)    
    def attackMonster(self, mon):
        clear()
        print("You are trying to do " + mon.name)
        print()
        # explaining how homework fights work
        print("Your happiness is " + str(self.happiness) + ".")
        print(mon.name + " has " + str(mon.health) + " problems, each is 1 happiness.")
        print()
        # if they succcessfully fought (had enough happiness)
        if self.happiness > mon.health:
            self.happiness -= mon.health
            # randomly assign an amount of knowledge from 5-15
            inc = random.randint(5,15)
            self.know += inc
            self.time -= 2
            # print outcome
            print("You did it. It took you 2 hours.\nYour happiness is now " + str(self.happiness) + ".")
            print("Your knowledge has increased by " + str(inc) + ".")
            # call monster die method to remove it from world
            mon.die()
        else:
            print("You ran out of happiness.")
            self.alive = False
        print()
        input("Press enter to continue...")

    # attack boss method, very different from attacking a regular monster    
    def attackBoss(self,boss):
        clear()
        print("You are trying to do " + boss.name)
        print()
        # all bosses follow the following structure: 
        # check boss name to determine what sequence to run
        if boss.name.lower() == "derivatives midterm":
            # ask first exam question
            print("What is the derivative of x^2?\na. x^3\nb. 2x\nc. 2x^2")
            ans = input("Your answer: ")
            # if the answer was right, move on to the next (and so forth)
            if ans == "b":
                print("What is the integral of sin x?\na. -cos x\nb. cos x\nc. tan x")
                ans = input("Your answer: ")
                if ans == "a":
                    print("What is the derivative and integral of e^x?\na. derivative is e^x, integral is e^x\nb. derivative is xe, integral is e^x\nc. derivative is e^x, integral is xe")
                    ans = input("Your answer: ")
                    # if made it to final question and answered right...
                    if ans == "a":
                        # alter stats based on win
                        self.happiness += 10
                        self.know -= boss.health
                        self.time -= 3
                        # print outcome 
                        print("You passed. It took you 3 hours.\nYour happiness increased by 10 and your knowledge decreased by " + str(boss.health) + " because you forget everything once you've passed the exam.")
                        print("You have now gained " + str(boss.drops) + ". Be sure to pick it up.")
                        # remove boss from world
                        boss.die()
                        # return that the attack was successful
                        return True
                    # otherwise, if any of the questions are answered wrong...
                    else: 
                        # print failure sequence
                        print("Incorrect.")
                        print("You failed. You will have to retake the exam.")
                        print("You have lost half your knowledge and happiness.")
                        # reduce stats in half
                        self.know = self.know // 2
                        self.happiness = self.happiness // 2
                        self.time -= 3
                        # return that the attack was unsuccessful
                        return False
                else: 
                    print("Incorrect.")
                    print("You failed. You will have to retake the exam.")
                    print("You have lost half your knowledge and happiness.")
                    self.know = self.know // 2
                    self.happiness = self.happiness // 2
                    self.time -= 3
                    return False
            else: 
                print("Incorrect.")
                print("You failed. You will have to retake the exam.")
                print("You have lost half your knowledge and happiness.")
                self.know = self.know // 2
                self.happiness = self.happiness // 2
                self.time -= 3
                return False
        # all other bosses are the same but with different questions
        elif boss.name.lower() == "calculus final":
            print("What is an integral calculation used for?\na. to find the slope of a tangent line\nb. to find the rate of change of an related rates problem\nc. to find the area under the curve of a function")
            ans = input("Your answer: ")
            if ans == "c":
                print("What is the limit definition of a function?\na. lim h->0 f(x) = (f(x-h) + f(x))/h\nb. lim h->0 f(x) = (f(x+h) + f(x))/h\nc. lim h->0 f(x) = (f(x+h) - f(x))/h")
                ans = input("Your answer: ")
                if ans == "c":
                    print("What do you always add to the end of the integrand of an indefinite integral?\na. +1\nb. +x\nc. +C")
                    ans = input("Your answer: ")
                    if ans == "c":
                        self.happiness += 20
                        self.know -= boss.health
                        self.time -= 3
                        print("You passed. It took you 3 hours.\nYour happiness increased by 20 and your knowledge decreased by " + str(boss.health))
                        print("You have now gained " + str(boss.drops) + ". Be sure to pick it up.")
                        boss.die()
                        return True
                    else: 
                        print("Incorrect.")
                        print("You failed. You will have to retake the exam.")
                        print("You have lost half your knowledge and happiness.")
                        self.know = self.know // 2
                        self.happiness = self.happiness // 2
                        self.time -= 3
                        return False
                else: 
                    print("Incorrect.")
                    print("You failed. You will have to retake the exam.")
                    print("You have lost half your knowledge and happiness.")
                    self.know = self.know // 2
                    self.happiness = self.happiness // 2
                    self.time -= 3
                    return False
            else: 
                print("Incorrect.")
                print("You failed. You will have to retake the exam.")
                print("You have lost half your knowledge and happiness.")
                self.know = self.know // 2
                self.happiness = self.happiness // 2
                self.time -= 3
                return False
        elif boss.name.lower() == "cs midterm":
            print("What is a recursive function?\na. a function that calls itself\nb. a function that uses loops\nc. a function that recurses")
            ans = input("Your answer: ")
            if ans == "a":
                print("What are the three key components of a while loop?\na. a global variable, a nested loop, a return statement\nb. the variable 'i', an algorithm within the loop, an increment\nc. a setup, an algorithm within the loop, an increment")
                ans = input("Your answer: ")
                if ans == "c":
                    print("What does % do?\na. integer division\nb. returns the specified percent of an amount\nc. returns the remainder of a division computation")
                    ans = input("Your answer: ")
                    if ans == "c":
                        self.happiness += 20
                        self.know -= boss.health
                        self.time -= 2
                        print("You passed. It took you 2 hours.\nYour happiness increased by 20 and your knowledge decreased by " + str(boss.health))
                        print("You have now gained " + str(boss.drops) + ". Be sure to pick it up.")
                        boss.die()
                        return True
                    else: 
                        print("Incorrect.")
                        print("You failed. You will have to retake the exam.")
                        print("You have lost half your knowledge and happiness.")
                        self.know = self.know // 2
                        self.happiness = self.happiness // 2
                        self.time -= 3
                        return False
                else: 
                    print("Incorrect.")
                    print("You failed. You will have to retake the exam.")
                    print("You have lost half your knowledge and happiness.")
                    self.know = self.know // 2
                    self.happiness = self.happiness // 2
                    self.time -= 3
                    return False
            else: 
                print("Incorrect.")
                print("You failed. You will have to retake the exam.")
                print("You have lost half your knowledge and happiness.")
                self.know = self.know // 2
                self.happiness = self.happiness // 2
                self.time -= 3
                return False
        elif boss.name.lower() == "cs final":
            print("What does an environment look like at its initialization?\na. it has a global frame\nb. it has no frames\nc. it has variables floating in space")
            ans = input("Your answer: ")
            if ans == "a":
                print("How do you add one thing to the end of a list?\na. .add\nb. .append\nc. .extend")
                ans = input("Your answer: ")
                if ans == "b":
                    print("True or False: You can search a dictionary by index.\na. True\nb. False")
                    ans = input("Your answer: ")
                    if ans == "b":
                        self.happiness += 20
                        self.know -= boss.health
                        self.time -= 3
                        print("You passed. It took you 3 hours.\nYour happiness increased by 20 and your knowledge decreased by " + str(boss.health))
                        print("You have now gained " + str(boss.drops) + ". Be sure to pick it up.")
                        boss.die()
                        return True
                    else: 
                        print("Incorrect.")
                        print("You failed. You will have to retake the exam.")
                        print("You have lost half your knowledge and happiness.")
                        self.know = self.know // 2
                        self.happiness = self.happiness // 2
                        self.time -= 3
                        return False
                else: 
                    print("Incorrect.")
                    print("You failed. You will have to retake the exam.")
                    print("You have lost half your knowledge and happiness.")
                    self.know = self.know // 2
                    self.happiness = self.happiness // 2
                    self.time -= 3
                    return False
            else: 
                print("Incorrect.")
                print("You failed. You will have to retake the exam.")
                print("You have lost half your knowledge and happiness.")
                self.know = self.know // 2
                self.happiness = self.happiness // 2
                self.time -= 3
                return False

    def getItemByName(self, name):
        for i in self.items:
            if i.name.lower() == name.lower():
                return i

    # check knowledge method that is essentially getitembyname but returns true or false instead of the item            
    def checkKnowledge(self, knowledge):
        for i in self.items:
            if i.name.lower() == knowledge.lower():
                return True

    def removeItem(self, item):
        self.items.remove(item)

    # inventory weight method that checks if a player has the capacity to carry a specified item    
    def inventoryWeight(self, weight):
        if self.inventory + weight > 20:
            return False
        return True

