import os

def clear():
    os.system('cls' if os.name == 'nt' else 'clear')

class Item:
    # class specific variable 
    type = "item"
    def __init__(self, name, desc, weight):
        self.name = name
        self.desc = desc
        self.weight = weight
        self.loc = None
    def describe(self):
        clear()
        print(self.desc)
        print("Weight: " + str(self.weight))
        print()
        input("Press enter to continue...")
    def putInRoom(self, room):
        self.loc = room
        room.addItem(self)

# food inherits from item class
class Food(Item):
    type = "food"
    # same initialization, but add hp variable
    def __init__(self, name, desc, weight, hp):
        Item.__init__(self, name, desc, weight)
        self.hp = hp
    
    # overwrite describe method from super to also print new variable stat
    def describe(self):
        clear()
        print(self.desc)
        print("This item restores " + str(self.hp) + " hunger.")
        print("Weight: " + str(self.weight))
        print()
        input("Press enter to continue...")

# sleep item is same as super, with different class specific type variable
class Sleep(Item):
    type = "sleep"

# sub class container inherits from item
class Container(Item):
    # different class specific variable type
    type = "container"
    # init from super, add three new class specific variables
    def __init__(self,name,desc,weight,capacity):
        Item.__init__(self,name,desc,weight)
        self.capacity = capacity
        self.carryingweight = 0
        self.carrying = []
    
    # overwrite describe method to include specific characteristics
    def describe(self):
        clear()
        print(self.desc)
        print("You can store " + str(self.capacity) + " item weight in this.")
        print("Currently holding " + str(self.carryingweight) + " item weight.")
        print("Weight: " + str(self.weight))
        print()
        input("Press enter to continue...")

    # new methods (copied from room) to access items in the container
    def addItem(self, item):
        self.carrying.append(item)
    def getItemByName(self, name):
        for i in self.carrying:
            if i.name.lower() == name.lower():
                return i
    def removeItem(self, item):
        self.carrying.remove(item)
    def hasItems(self):
        return self.carrying != []

class Special(Item):
    type = "knowledge"

# sub class of food which is sub class of item
class Sale(Food):
    # class specific type is sale 
    type = "sale"

    # initializes same as food, but add one more parameter cost
    def __init__(self, name, desc, weight, hp, cost):
        super().__init__(name, desc, weight, hp)
        self.cost = cost
