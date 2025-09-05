import random
from faker import Faker

"""
Note I am using the Faker library to generate fake data,
I installed this dependency in a python environment with:
pip install faker
"""
fake = Faker()

"""
I've decided to represent players as a class with the
following attributes for the sake of finding the right
corresponding players in the big player array and the
preferences dictionary using the id attribute.
"""
class Player:
    # use global id counter to ensure call to Player always has new player id
    id_counter = 0

    def __init__(self, name, gender, sexuality, arrival_round):
        self.id = self.id_counter
        Player.id_counter += 1
        self.name = name
        self.gender = gender
        self.sexuality = sexuality
        self.arrival_round = arrival_round
        self.sexuality = self.assignSexuality()

    # defined to make hashing and equality checks work.
    # player id must be unique or else it all breaks!
    def __eq__(self, o):
        return False if o is None else self.id == o.id

    def __hash__(self):
        return hash(self.id)

    def __str__(self):
        return f"{self.name} ({self.sexuality} - {self.gender}) (arrives {self.arrival_round})"

    __repr__ = __str__

    def prettyPrint(self):
        print(self.name + ", " + self.gender + ", " + self.sexuality)

    def assignSexuality(self):
        rand = random.random()
        if self.gender == "F":
            if rand <= 0.15:
                return "lesbian"
            if rand <= 0.75:
                return "bi"
            else:
                return "straight"
        elif self.gender == "M":
            if rand <= 0.15:
                return "gay"
            if rand <= 0.75:
                return "bi"
            else:
                return "straight"
        elif self.gender == "X":
            if rand <= 0.15:
                return "lesbian"
            if rand <= 0.3:
                return "gay"
            else:
                return "bi"

    def genPrefs(self, options):
        prefs = options.copy()
        random.shuffle(prefs)
        return prefs


"""
generatePlayers takes the number of players you want in your dataset
and returns a python list of all the players as Player objects
"""
def generatePlayers(numPlayers, numPeriods):
    players = []
    periods = list(range(1, numPeriods + 1))

    # statistics based on Reed's first year demographics
    female = int(numPlayers * 0.47)
    male = int(numPlayers * 0.33)
    nonbinary = int(numPlayers * 0.2)

    genders = ["F"] * female + ["M"] * male + ["X"] * nonbinary

    for gender in genders:
        name = ""
        if gender == "F":
            name = fake.first_name_female() + " " + fake.last_name()
        elif gender == "M":
            name = fake.first_name_male() + " " + fake.last_name()
        elif gender == "X":
            name = fake.name()

        player = Player(name, gender, "", random.choice(periods))
        players.append(player)

    return players


"""
generateData takes the list of players generated above
and returns a dictionary of the following format: 

Player object: [Player object, Player object, ...]

where the list represents each player's preferences
"""
def generateData(players):
    prefs = {}

    fem = list(filter(lambda p: p.gender == "F", players))
    masc = list(filter(lambda p: p.gender == "M", players))
    nb = list(filter(lambda p: p.gender == "X", players))

    l = fem + nb
    b = fem + masc + nb
    g = masc + nb

    for p in players:
        if p.sexuality == "lesbian":
            options = l.copy()
            options.remove(p)
            prefs[p] = p.genPrefs(options)
        if p.sexuality == "bi":
            options = b.copy()
            options.remove(p)
            prefs[p] = p.genPrefs(options)
        if p.sexuality == "gay":
            options = g.copy()
            options.remove(p)
            prefs[p] = p.genPrefs(options)
        if p.sexuality == "straight":
            if p.gender == "M":
                prefs[p] = p.genPrefs(fem)
            if p.gender == "F":
                prefs[p] = p.genPrefs(masc)

    return prefs


if __name__ == "__main__":
    # to help you visualize the data
    all_players = generatePlayers(100, 3)
    for player in all_players:
        print(player)
    preferences = generateData(all_players)
    for player, pref_list in preferences.items():
        print(f"{player} prefers:")
        for pref in pref_list:
            print(f"  - {pref}")
        print()
