class Dial:

    def __init__(self,limit,r,l):
        self.maxLevel = limit
        self.level = 0
        self.rchar = r
        self.lchar = l
        self.dial = "[" + (self.lchar * self.maxLevel) + "]"

    def display(self):
        print (self.dial)

    def increaseBy(self,inc):
        if self.level + inc <= self.maxLevel:
            self.level += inc
            self.dial = "[" + (self.rchar * self.level) + (self.lchar * (self.maxLevel - self.level)) + "]"
        else: 
            self.dial = "[" + (self.rchar * self.maxLevel) + "]"
            self.level = self.maxLevel

    def decreaseBy(self,dec):
        if self.level - dec > 0:
            self.level -= dec
            self.dial = "[" + (self.rchar * self.level) + (self.lchar * (self.maxLevel - self.level)) + "]"
        else: 
            self.dial = "[" + (self.lchar * self.maxLevel) + "]"
            self.level = 0

class VerboseDial(Dial): 

    def increaseBy(self,inc):
        Dial.increaseBy(self,inc)
        print(self.dial)

    def decreaseBy(self,dec):
        Dial.decreaseBy(self,dec)
        print(self.dial)

