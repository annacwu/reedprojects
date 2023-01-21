class GiftCard:

    def __init__(self, amount): # used by the constructor
        self.balance = amount

    def addFunds(self, amount): # a method definition
        self.balance = self.balance + amount
        return self.balance

    def spend(self, amount):    # another method definition
        if amount > self.balance:
            return "Insufficient funds"
        self.balance = self.balance - amount
        return self.balance

    def getBalance(self):      # a balance "getter"
        return self.balance
    
