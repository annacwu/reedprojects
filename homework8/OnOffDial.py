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

class OnOffDial(VerboseDial):

    def __init__(self,limit,r,l):
        self.maxLevel = limit
        self.level = 0
        self.rchar = r
        self.lchar = l
        self.dial = "[" + (self.lchar * self.maxLevel) + "]"
        self.state = None
    
    def turnOff(self):
        self.state = False
    
    def increaseBy(self,inc):
        if self.state == False:
            Dial.increaseBy(self,inc)
        else: 
            Dial.increaseBy(self,inc)
            print(self.dial)

    def decreaseBy(self,dec):
        if self.state == False:
            Dial.decreaseBy(self,dec)
        else:
            Dial.decreaseBy(self,dec)
            print(self.dial)

    def display(self):
        if self.state == False:
            return None
        else: 
            print (self.dial)

    def turnOn(self):
        self.state = True
