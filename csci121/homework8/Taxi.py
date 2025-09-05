from math import sqrt

class Car:

    def __init__(self,mpg,ftc):
        self.MPG = mpg
        self.FTC = ftc
        self.x = 0
        self.y = 0
        self.fuel = ftc

    def driveTo(self,x,y):
        self.dist = sqrt(((x - self.x) ** 2) + ((y - self.y) ** 2))
        gasNeeded = self.dist / self.MPG
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

class Taxi(Car): 
    def __init__(self,mpg,ftc):
        self.MPG = mpg
        self.FTC = ftc
        self.x = 0
        self.y = 0
        self.fuel = ftc
        self.money = 0
        self.passenger = None
        self.passMiles = 0

    def pickup(self):
        passenger = True
        if self.passenger != True:
            passenger = True
            self.passenger = True
        else: 
            passenger = False
        return passenger

    def dropoff(self):
        passenger = True
        self.money += 2 + 3 * self.passMiles
        self.passMiles = 0
        if self.passenger != True:
            passenger = False
        self.passenger = False
        return passenger

    def driveTo(self,x,y):
        self.dist = sqrt(((x - self.x) ** 2) + ((y - self.y) ** 2))
        gasNeeded = self.dist / self.MPG
        traveled = None
        if self.passenger == True: 
            self.passMiles = self.dist
        if self.fuel >= gasNeeded:
            self.x = x
            self.y = y
            self.fuel = self.fuel - gasNeeded
            traveled = True
        else: 
            traveled = False
        return traveled

    def getMoney(self):
        return self.money

