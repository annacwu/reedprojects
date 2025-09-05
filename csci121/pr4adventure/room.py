import random

class Room:
    # type of room variable
    type = "room"
    def __init__(self, description):
        self.desc = description
        self.monsters = []
        self.exits = []
        self.items = []
        # added separate list for container items and characters
        self.citems = []
        self.characters = []
    def addExit(self, exitName, destination):
        self.exits.append([exitName, destination])
    def getDestination(self, direction):
        for e in self.exits:
            if e[0].lower() == direction.lower():
                return e[1]
    def connectRooms(room1, dir1, room2, dir2):
        #creates "dir1" exit from room1 to room2 and vice versa
        room1.addExit(dir1, room2)
        room2.addExit(dir2, room1)
    def exitNames(self):
        return [x[0] for x in self.exits]
    def addItem(self, item):
        self.items.append(item)
    def removeItem(self, item):
        self.items.remove(item)
    def addMonster(self, monster):
        self.monsters.append(monster)
    def removeMonster(self, monster):
        self.monsters.remove(monster)

    # added methods for characters that are essentially the same as the monster and item methods    
    def addCharacter(self, character):
        self.characters.append(character)
    def removeCharacter(self, character):
        self.characters.remove(character)
    def hasItems(self):
        return self.items != []
    def getItemByName(self, name):
        for i in self.items:
            if i.name.lower() == name.lower():
                return i
        return False
    def hasMonsters(self):
        return self.monsters != []
    def getMonsterByName(self, name):
        for i in self.monsters:
            if i.name.lower() == name.lower():
                return i
        return False
    def hasCharacters(self):
        return self.characters != []
    def getCharacterByName(self, name):
        for i in self.characters:
            if i.name.lower() == name.lower():
                return i
        return False
    def randomNeighbor(self):
        return random.choice(self.exits)[1]

# sub class store that inherits from room
class Store(Room):
    type = "store"
    # has same init but one extra list of sale items
    def __init__(self, description):
        super().__init__(description)
        self.saleitems = []

    # overwrite add item and remove item to check what type of item it is first and adjust accordingly
    def addItem(self, item):
        if item.type == "sale":
            self.saleitems.append(item)
        else: 
            self.items.append(item)
    def removeItem(self, item):
        if item.type == "sale":
            self.saleitems.remove(item)
        else: 
            self.items.remove(item)

    # hasitem and getitembyname methods but for the sale items list
    def hasSaleItems(self):
        return self.saleitems != []
    def getSaleItemByName(self, name):
        for i in self.saleitems:
            if i.name.lower() == name.lower():
                return i
        return False