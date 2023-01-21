import os
import random
import updater

def clear():
    os.system('cls' if os.name == 'nt' else 'clear')

# new character class
class Character:
    type = "regular"
    # similar to monster but instead of health it has a 
    # description of what the charater says
    def __init__(self, name, desc, loc):
        self.name = name
        self.desc = desc
        self.loc = loc
        loc.addCharacter(self)
        updater.register(self)
    
    # talk method that prints character description (similar to item describe method)
    def talk(self):
        clear()
        print(self.desc)
        input("\nPress enter to continue...")
    
    # updates and moves around similarly to monsters
    def update(self):
        if random.random() < .3:
            self.moveTo(self.loc.randomNeighbor())
    def moveTo(self, room):
        self.loc.removeCharacter(self)
        self.loc = room
        room.addCharacter(self)

# professor type subclass of character
class Prof(Character):
    type = "prof"

    # init is the same, except new paramater quest is list of lecture saying to rotate through 
    # and is not added to the updater because should stay in specified rooms
    def __init__(self, name, desc, quest, loc):
        self.name = name
        self.desc = desc
        self.quest = quest
        self.loc = loc
        loc.addCharacter(self)

    # lecture method than randomly chooses one of the items from the lecture list to print    
    def lecture(self):
        i = random.randint(0,len(self.quest) - 1)
        return (self.quest[i])
    