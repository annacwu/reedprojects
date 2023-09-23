import random
import tkinter
random.seed()

def plot(xvals, yvals):
    # This is a function for creating a simple scatter plot.  You will use it,
    # but you can ignore the internal workings.
    root = tkinter.Tk()
    c = tkinter.Canvas(root, width=700, height=400, bg='white') # Was 350 x 280
    c.grid()
    # Create the x-axis.
    c.create_line(50,350,650,350, width=3)
    for i in range(5):
        x = 50 + (i * 150)
        c.create_text(x,355,anchor='n', text='%s'% (.5*(i+2) ) )
    # Create the y-axis.
    c.create_line(50,350,50,50, width=3)
    for i in range(5):
        y = 350 - (i * 75)
        c.create_text(45,y, anchor='e', text='%s'% (.25*i))
    # Plot the points.
    for i in range(len(xvals)):
        x, y = xvals[i], yvals[i]
        xpixel = int(50 + 300*(x-1))
        ypixel = int(350 - 300*y)
        c.create_oval(xpixel-3,ypixel-3,xpixel+3,ypixel+3, width=1, fill='red')
    root.mainloop()

# Constants: setting these values controls the parameters of your experiment.
injurycost     = 11 # Cost of losing a fight  
displaycost    = 1   # Cost of displaying between two passive birds  
foodbenefit    = 8  # Value of the food being fought over   
init_hawk      = 50
init_dove      = 50
init_defensive = 0
init_evolving  = 0

########
# MY CODE BELOW :) 
########
class World:

    # initialize method that creates a list of all the birds in the world
    def __init__(self):
        self.existingbirds = []

    # update method
    def update(self):
        # for every bird in the list...
        for bird in self.existingbirds:
            # call the update method from the Bird class
            bird.update()

    # free food method that represents the uncontested food that birds can find
    def free_food(self, occurances):
        # while we have not run the simulation as many times as the paramater specifies,
        i = 0
        while i < occurances: 
            # if there are birds in the world,
            if len(self.existingbirds) > 0:
                # choose a random bird from the world
                awardedbird = random.choice(self.existingbirds)
                # chosen bird eats
                awardedbird.eat()
            i += 1

    # conflict method that creates encounters between birds in the world
    def conflict(self, occurances):
        # while we have not run the simulation as many times as the paramater specifies,
        i = 0
        while i < occurances:
            # if there are birds in the world,
            if len(self.existingbirds) > 0:
                # choose two random birds
                b1 = random.choice(self.existingbirds)
                b2 = random.choice(self.existingbirds)
                # if the two chosen birds are actually the same bird
                while b1 == b2: 
                    # choose a different second bird
                    b2 = random.choice(self.existingbirds)
                # have the first bird encounter the second bird 
                b1.encounter(b2)
            i += 1

    # status method to count and print the number of birds of each species in the world
    def status(self):
        # create variables for each type of bird
        doves = 0
        hawks = 0
        defensive = 0
        # for every bird in the list of birds in the world,
        for bird in self.existingbirds:
            # read its class-specific variable, and add to the variables in this method accordingly
            if bird.species == "Dove":
                doves += 1
            elif bird.species == "Hawk":
                hawks += 1
            elif bird.species == "Defensive":
                defensive += 1
        # print the amount of each bird recorded
        print("Doves: " + str(doves) + "\nHawks: " + str(hawks) + "\nDefensive: " + str(defensive)) 

    # plot method for running the evolving birds simulation
    def evolvingPlot(self):
        # create two lists for the birds weights and aggression levels
        birdweights = []
        birdaggressions = []
        # for every bird in the world's list of birds
        for bird in self.existingbirds:
            # add its weight and aggression to the same spot in each respective list
            birdweights.append(bird.weight)
            birdaggressions.append(bird.aggression)
        # create the scatter plot based on the lists
        plot(birdweights, birdaggressions)

class Bird:
    # initialize method that adds a new bird to the same world and sets its health
    def __init__(self, World):
        self.world = World
        self.world.existingbirds.append(self)
        self.currenthealth = 100
        
    # simple methods for eating, injury, and displaying using the constants above
    def eat(self):
        self.currenthealth += foodbenefit
    
    def injured(self):
        self.currenthealth -= injurycost

    def display(self):
        self.currenthealth -= displaycost

    # die method that removes bird from world list
    def die(self):
        # get index of the dying bird with .index
        birdindex = self.world.existingbirds.index(self)
        # delete that index from the list of birds
        del self.world.existingbirds[birdindex]

    # updtea method to check how much health a bird has 
    def update(self):
        self.currenthealth -= 1
        # if the bird has no health left
        if self.currenthealth <= 0:
            # call the die method for that bird
            self.die()

# dove class inherits from the bird class
class Dove(Bird):
    species = "Dove"

    # update method specific to doves
    def update(self):
        # calls its super's update method
        Bird.update(self)
        # adds that if the health is 200 or more
        if self.currenthealth >= 200:
            # lose 100 health
            self.currenthealth -= 100
            # and reproduce by adding another dove to the world
            self.world.existingbirds.append(Dove(self.world))

    # defend choice method that indicates a dove will not defend itself
    def defend_choice(self):
        return False

    # encounter method that outlines dove behavior upon encounter
    def encounter(self, otherbird):
        # if the other bird will defend its food,
        if otherbird.defend_choice() == True:
            # the other bird eats
            otherbird.eat()
        # if it will not defend its food,
        elif otherbird.defend_choice() == False: 
            # both birds display
            otherbird.display()
            self.display()
            # choose a random winner between the two
            winner = random.choice([otherbird, self])
            # winner eats
            winner.eat()

# dove class inherits from the bird class
class Hawk(Bird):
    species = "Hawk"

    # update method specific to hawks, similar to dove but reproduces more hawks
    def update(self):
        Bird.update(self)
        if self.currenthealth >= 200:
            self.currenthealth -= 100
            self.world.existingbirds.append(Hawk(self.world))

    # defend choice method that indicates a hawk will defend itself
    def defend_choice(self):
        return True

    # encounter method that outlines hawk behavior upon encounte
    def encounter(self, otherbird):
        # if the other bird will defend itself,
        if otherbird.defend_choice() == True:
            # add both birds to a list to fight
            birds = [otherbird, self]
            # randomly choose an integer 0 or 1 for the index
            winner = random.choice([0,1])
            # the winning index eats
            birds[winner].eat()
            # remove the winner from the fight list
            del birds[winner]
            # the remaining bird, the only one in the list, is injured
            birds[0].injured()
        # otherwise, if the other bird will not defend itself
        elif otherbird.defend_choice() == False:
            # the current hawk gets to eat 
            self.eat()

# defensive type of bird, which is essentially a dove that defends itself 
# so it inherits from dove class
class Defensive(Dove):
    species = "Defensive"

    # overwrite defend chocie method so it will defend
    def defend_choice(self):
        return True

    # overwrite update method so it reproduces more defensive birds
    def update(self):
        Bird.update(self)
        if self.currenthealth >= 200:
            self.currenthealth -= 100
            self.world.existingbirds.append(Defensive(self.world))

# evolving type of bird that inherits from bird class
class Evolving(Bird):
    species = "Evolving"

    # initialization  method that uses bird init but adds on heritage conditions
    def __init__(self, World, parent):
        super().__init__(World)
        # if they are the first birds in the world,
        if parent == None:
            # give them random aggressions and weights
            self.weight = random.uniform(1.0,3.0)
            self.aggression = random.random()
        # if they have parents,
        else: 
            # their weight is a little more or less than their parents weights
            self.weight = parent.weight + random.uniform(-0.1,0.1)
            # if this adjustment exceeds or is less than the max weight, 
            # set it to the max/min
            if self.weight > 3.0:
                self.weight = 3.0
            elif self.weight < 1.0:
                self.weight = 1.0
            # same process is run to create aggression levels
            self.aggression = parent.aggression + random.uniform(-0.05,0.05)
            if self.aggression > 1.0: 
                self.aggression = 1.0
            elif self.aggression < 0.0:
                self.aggression = 0.0

    # update method that overwrites the super's update
    def update(self):
        # reduce health based on its weight
        self.currenthealth -= 0.4 + 0.6 * self.weight
        # if it has no health, die
        if self.currenthealth <= 0:
            self.die()
        # if it has enough health, reproduce
        if self.currenthealth >= 200:
            self.currenthealth -= 100
            # add self as the parent for the new bird
            self.world.existingbirds.append(Evolving(self.world, self))
        
    # more complicated defend choice method 
    def defend_choice(self):
        # the bird has a random liklihood to fight 
        self.fightprobability = random.random()
        # if its aggression is high, it is more likely to defend itself, 
        if self.aggression >= self.fightprobability: 
            return True
        # otherwise, it will not
        else: 
            return False

    # more complicated encounter method, seperate from defend choice
    def encounter(self, otherbird): 
        # random probability of fighting
        self.fightprobability = random.random()
        # if the other bird will defend itself and this bird is aggressive (and more likely to fight), 
        if otherbird.defend_choice() == True and self.aggression >= self.fightprobability:
            # the winner is based on its weight
            win = random.random()
            w1 = self.weight
            w2 = otherbird.weight
            # if the probability of the self winning is lower than the win outcome,
            if win > w1/(w1+w2):
                # other bird gets to eat
                otherbird.eat()
                # and self is injured
                self.injured()
            # otherwise,
            else: 
                # self gets to eat and other is injuered
                self.eat()
                otherbird.injured()
        # alternatively, if the other bird will defend itself and self is not aggressive enough to fight,
        elif otherbird.defend_choice() == True and self.aggression < self.fightprobability:
            # other bird eats
            otherbird.eat()
        # alternatively, if the other bird will not defend itself and self is aggressive enough to fight,
        elif otherbird.defend_choice() == False and self.aggression >= self.fightprobability:
            # self eats
            self.eat()
        # otherwise, if neither bird will defend or fight,
        elif otherbird.defend_choice() == False and self.aggression < self.fightprobability:
            # birds act like doves, displaying and then randomly choosing a winner
            otherbird.display()
            self.display()
            winner = random.choice([otherbird, self])
            winner.eat()
            


########
# The code below actually runs the simulation.  You shouldn't have to do anything to it.
########
w = World()
for i in range(init_dove):
    Dove(w)
for i in range(init_hawk):
    Hawk(w)
for i in range(init_defensive):
    Defensive(w)
for i in range(init_evolving):
    Evolving(w,None)

for t in range(10000):
    w.free_food(10)
    w.conflict(50)
    w.update()
w.status()
#w.evolvingPlot()    # This line adds a plot of evolving birds. Uncomment it when needed.


