from math import sqrt

class Car:

    def __init__(self,mpg,ftc):
        self.MPG = mpg
        self.FTC = ftc
        self.x = 0
        self.y = 0
        self.fuel = ftc

    def driveTo(self,x,y):
        dist = sqrt(((x - self.x) ** 2) + ((y - self.y) ** 2))
        gasNeeded = dist / self.MPG
        traveled = None
        if self.fuel >= gasNeeded:
            self.x = x
            self.y = y
            self.fuel = self.fuel - gasNeeded
            traveled = True
        else: 
            traveled = False
        return traveled
    
    def getLocationX(self):
        return self.x
    
    def getLocationY(self):
        return self.y

    def getGas(self):
        return self.fuel

    def getToFill(self):
        return self.FTC - self.fuel

