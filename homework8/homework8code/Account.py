class Account:
    interest_rate = .02

    def __init__(self, amount):
        self.balance = amount

    def deposit(self, amount):
        self.balance += amount

    def withdraw(self, amount):
        self.balance -= amount

    def payInterest(self):
        self.balance *= 1.0 + self.interest_rate

    def getBalance(self):
        return self.balance


class Savings(Account):
    interest_rate = 0.04
    withdraw_fee = 1.0
    
    def withdraw(self, amount):
        Account.withdraw(self, amount + self.withdraw_fee)


class Checking(Account):

    min_balance = 1000.0

    def payInterest(self):         
        if self.balance >= self.min_balance:        
            Account.payInterest(self)


class PromotionalChecking(Checking):
             
    reward = 50

    def __init__(self,amount):
        Checking.__init__(self,amount+self.reward)
        
        #
        # Could also write this as super().__init__(amount+self.reward)
        #
