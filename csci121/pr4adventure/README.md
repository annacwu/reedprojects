“Two Weeks Left”
Created by Anna Wu

Game Description:

“Two Weeks Left” is an end of the semester simulation adventure game. The player is a first year computer science student at Reed, in the last two weeks before the first semester ends. The player has a variety of characteristics, such as the need for sleep, hunger, currency, knowledge, happiness, and time remaining. The player has 336 hours (two weeks worth) of in-game time to acquire each of the four types of special knowledge and maintain 50+ levels of happiness in order to achieve a victory. The game is set on Reed’s campus, and the player must explore the campus to find activities they can do in different rooms and different characters/items they can interact with to increase their knowledge and happiness. However, as time continually passes, they lose more hunger and their need for sleep increases, so they must also manage those characteristics. If their happiness, hunger, or sleep is improperly taken care of, the player will cease to function and the game will end in a loss. 

Instructions: 

  Each of the four exams (derivatives midterm, calculus final, cs midterm, cs exam) drops the four required special knowledge items. They are located in the calculus lecture (in eliot) and the cs lecture (in vollum). 
	You must acquire enough knowledge through studying, doing homework, going to lecture, and other small ways to have the prerequisites required to do the first exams. 
	You must also maintain sleep (by sleeping on things every once in a while) and hunger (by cooking or buying food) periodically. You can increase your currency by calling your family in your dorm room. 
	 Finally, you must maintain happiness by doing a variety of activities throughout the campus (e.g. talking to characters, eating, room specific activities). 
	It is recommended that you begin by exploring campus and then carry out actions once you are familiar with the actions in each room and where everything is located. 
	If you need further clarification/command help, type “help” while playing. I had my friend test the game, they were able to figure out how to play with incredibly minimal instruction so it should be intuitive.


Notes for playing: 

- Going to lectures has all the answers for each of the exams, use those to beat them
- Homework drops random amounts of knowledge each time, spawns in every 10 rounds of time passing
- I added cheat codes so you can reach the exams and other later-game features quicker without having to play the whole thing (ease for grading)
	- time <integer> — passes the specified amount of time in the game
	- buff – resets hunger and need to sleep
	- know – increases knowledge by +100
	- hap – increases happiness by +100

Features List: 
- “me” command (2 points): Create a command that lets the player see their current status, including their health and/or whatever other attributes you’ve added.
	- Added showStatus() method to main
	- Added commandWords conditional for “me” command in main
- bigger world (2 points): The game starts with four rooms. That is not enough for much to happen in. Make the world bigger and more interesting. 	- Give descriptions to rooms that say more than just a room number.
	- Created 17 total rooms – all connected to Reed’s campus
	- Some “rooms” are buildings that have rooms in them (ex. Trillium, vollum, etc.)
	- Some rooms have a different description that details the room specific commands available in that location
	- Changed “go” command to “enter” to make more sense with the lore of the world names
- “inspect” command (2 points): Create a command that lets the player look at items. This should display the item description. The player should be able to look at both items they see in their current location and items in their inventory.
	- Added if statement to main for that command word
	- Multi-word input follows same structure as pickup did
	- Checks location and inventory for the item
	- Calls describe method in item 
- “drop” command (2 points): Make it so that the player can drop items, so that their inventory doesn’t become unreasonably large.
	- Added drop method in player
	- Added if statement to main for drop command
	- Added get item name in player class to look through player inventory
- healing items (2 points): Create items that players can use to restore health. These items should only be usable once each and should disappear when used.
	- Created food items subclass and sale items subclass in item.py 
	- Eat command added to main
	- Similar to pickup, checks player inventory instead of room
	- Food class has another parameter, where it has a specific amount of healing property (gets rid of a certain amount of hunger)
	- Added self.hp to init for this item
	- Changed describe method to also give the hp it restores in its description
	- Added type variable to make sure you can only eat food and not other items
	- Created sleep items to heal need for sleep 
	- Added bed subclass in item with different type to ensure you can only sleep on beds
- limited inventory (2 points): Make objects you’re carrying have a size or weight, and put a limit on how much you can carry.
	- Add weight as parameter for item class, all items have weight now
	- Changed pickup conditional in main to add fail messages if they do not have the space/item is too heavy
	- Changed inventory display to show how much weight player is carrying in player
	- Changed inspect to show the weight of each item in main
- weapons (2 points): Implement a new type of item for weapons, ones that a player can use to make themselves more formidable in battle. This will requires modifying how fights against monsters work.
	- Added lectures with “Listen to command” for professors
	- When player listens to lectures, they get all the answers to the exam questions
	- Added “study” command to main to increase player knowledge for exams
	- Added desk item and subclass in item
	- “Do” command in main
	- Use to attack monsters
	- Adjusted attack method in player to use happiness not health
- “wait” command (2 points): In the game as it is, time passes only when you move to a new room. Make it so that you can type a command that just lets time pass. Or implement a trickier version: let the user type a number so they can wait for a number of turns with only one command entry.
	- Added new command condition in main
- locked doors (3 points): Create doors that cannot be used unless first unlocked with a key, and also the relevant keys.
	- Not a locked door in my game, but actually a locked exam/boss
	- Can only do certain exams if you have the right type of special knowledge and enough general knowledge
	- Added special subclass to item w “special” type to create special knowledge
  	- Player needs to be holding the item in their inventory to do the locked exams (cannot drop this type of knowledge) 
	- Incorporated through conditional statements in main
	- Loop through inventory when try to do do command to see if have item
- containers (3 points): Make containers (bags, boxes, etc.) that can hold other objects.
	- Added container sub class to item.py
	- Changed init function to also have capacity and carrying list
	- Added similar functions to that of room items list
	- Added open command in main
	- Uses display function in container class to check if there are items
	- If there are items, it puts the items from the container into the room and removes them from the container
	- Added put in command to put items in container
	- String stuff at beginning of command to get names of containers and items
	- Many if statements to account for weights and names
	- Many if statements checking if the container is already in ones inventory or not
- victory condition (3 points): Create a way for the player to win the game.
	- Made victory variable that will print the winning message if true
	- Becomes true if all exams beaten and happiness greater than 50 
	- Checks each time timepasses
	- Added check function to main to see if player has all items needed to win
	- Added check function to player that is called by main
	- Game only keeps playing if victory variable is false
	- Runs victory message if player wins
- loot (3 points): Make it so that defeating some monsters causes useful items to appear. There should be some amount of randomness about what those items are.
	- Completing exams gives special knowledge needed to take higher level exams
	- Altered subclass to take an additional parameter “drops” to detail what it drops
	- When exam is beaten, spawns special item type identical to exam’s drop variable
	- Completing homework gives a random number of knowledge points
	- Altered attackmonster to use random to drop a random amount of knowledge points
- more monsters (3 points): Create several different types of monsters. They should not just have different names, but really be different in important ways.
	- Quizzes/exams
		- Fight with knowledge, lose the amount of knowledge equivalent to the boss health
		- Added boss subclass to monster
		- Changed init so it does not move around
		- Added function to player (attackboss)
		- Runs each exam with questions and accounts for each of their fail situations
		- Uses many conditionals (detailed in code comments) to run the exam questions and answers sequence
		- Added a million conditionals to main to test what boss they were fighting and if they had the proper knowledge and type of knowledge to beat it
		- Added new item called special knowledge dropped by these, needed to keep completing new ones
		- Sub class in item.py
	- Homework
		- Moves around, similar to starter code monsters
		- Uses happiness to beat
		- Drops random knowledge
		- Added functions to main, monster 
	- Projects — didn’t have time or will to add, really wish i could’ve, may add in the future
		- Work on them for a long time, gain knowledge
		- Use tutoring to level up stages of the project
		- Drops coding ability, which gives permanent cs knowledge
- player attributes (3 points): By default the only characteristic of the player is their health. Add more attributes that affect what the player can do.
	- Happiness
		- Added check in time passes to see if player is maintaining enough happiness
		- printHappyWarning method to main
	- Sleep 
		- Add time since sleep variable (keeps track of how much time is left since last sleep and every hour since)
		- Reset time variable every time sleep happens 
		- If sleep on bed vs couch different happiness levels
		- Added printSleepWarning to main
	- Hunger (if it gets above 15 your happiness begins dropping rapidly)
		- Added printHungerWarning method to main
	- Knowledge 
	- Time 
	- All of these are adjusted by calling player.____ in the necessary situations based on players actions
- currency (4 points): Create a currency for the game. There should be a way for the player to gain currency, and also useful ways for them to spend the currency. (A merchant? a store?)
	- Filled out seven eleven, Made commons (store) where they sell food	
	- Addition would be to have these replenish over time
	- Made new variable in player
	- Displays in inventory 
	- Made new sub-class item of food for sale
	- Changed type
	- Takes new parameter cost
	- Edited eat to allow sale items to be eaten
	- Edited pickup to prevent them from just picking up those items
	- Made new command in main “buy” 
	- When player buys, remove that item from sale list and create identical food item
	- New sub class of room called store that has list of purchasable items
	- Type is store, can only run buy command in store (bc regular places dont have “getsaleitem” method —- crashes
	- Gain currency by calling family (done in dorm room)
- characters (4 points): Characters are neutral beings in the game that you can talk to and interact with. (The “talking to” can be a matter of picking one of several options of things to say from a list.) They should interact with you or the world in some way when you talk to them. This should be different than the “helper” above (that is, if you make a helper character, then you need another character to get credit for this option).
	- Created new character class
	- Added addCharacter method to room class
	- Made new list in init
	- Added talk to method to character class
	- Added import to main
	- Added characters to main create world
	- Added hasCharacters method to room class
	- Altered print situation
	- Added talk to conditional in main
	- Added get character by name method in room class
	- Using monster code to add methods in character class to move them around at random
	- Added prof subclass where they can lecture and they don’t leave the lecture rooms/office hours
- introducing monsters during play (2 points): Make it so that over time new monsters appear in the world.
	- Added player variable spawn to player
	- If 10 rounds of timepasses in timepasses conditional in main
	- adds homework in players current location
CUSTOM ALTERATIONS (with suggested point values): 
- Change code to accept lowercase letters in commands (+1 point)
- Added .lower() into room and player destination methods, followed flow of the code to make sure this didn’t bug
- Changed go to accept multi word directions (+1 point)
- Altered if statement in main
- Changed it so that if you mistype a room it won't fail you
- Added for loop through exits connected to player location
- Added if statements to go command in main
- Added many print statements and commandsuccess false statements to ensure game continues running
- Changed go to ‘enter’ to make more sense
- Made room specific commands to do fun things for happiness (happy commands) (+2 points)
	- Gym - workout
	- Kitchen - cook, clean, sit
	- Dorm - call, play switch
	- Common room - hangout
	- For all above ^^ checked if player was in properly associated room and then carried out specific player characteristic alterations 
- Lectures “listen to” 
	- Added professor character to classroom
	- “Listen to” command to main
	- Causes professors to lecture, which makes them choose a phrase from a list associated with their character at random
	- Adds knowledge points, takes 1 hour of time
