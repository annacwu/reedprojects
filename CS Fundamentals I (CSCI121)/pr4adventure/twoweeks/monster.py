import random
import updater

class Monster:
    type = "hw"
    # add monster to updater to have it move around, make each of its parameters a variable
    def __init__(self, name, health, room):
        self.name = name
        self.health = health
        self.room = room
        # uses room class methods 
        room.addMonster(self)
        updater.register(self)
    def update(self):
        if random.random() < .5:
            self.moveTo(self.room.randomNeighbor())
    def moveTo(self, room):
        self.room.removeMonster(self)
        self.room = room
        room.addMonster(self)
    def die(self):
        self.room.removeMonster(self)
        updater.deregister(self)

# sub class of monster
class Boss(Monster):
    type = "boss"

    # same init except does not add monster to register because should not move around
    def __init__(self, name, health, room, drops):
        self.name = name
        self.health = health
        self.room = room
        self.drops = drops
        room.addMonster(self)

    # overwrites die because does not need to deal with updater
    def die(self):
        self.room.removeMonster(self)

# this class is never called, was idea for a new type of monster that i would implement in the future
class Proj(Monster):
    type = "project"
    def __init__(self, name, health, room, drops):
        self.name = name
        self.health = health
        self.room = room
        self.drops = drops
        room.addMonster(self)
