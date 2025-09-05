Just a couple quick notes: 

The simulation is currently set to run only the first test of 50 doves 
and 50 hawks with the initial instructions in the project description. 
If you change each of the constants as outlined by the description 
(adding defensive, evolving, changing foodbenefit and injurycost, etc.), 
it outputs all the proper outcomes. You cannot run evolving and the other
types of birds at the same time, due to the fact that we were not meant
to add weights and aggressions to the other birds so they cannot complete
their encounter methods properly against each other---though it wasn't in
the scope of the project to make them compatible with each other so I
left it at this. 

There were a couple parts of the project instructions that were a bit
vague, such as what evolving birds are meant to do when neither fight.
In these cases, I assumed how they were supposed to behave. When neither
evolving bird is aggressive enough to fight, they act like doves. This 
was also the case with the defensive birds, as from what I could tell from
their description they are basically the exact same as doves other than 
the fact that they will defend their food. Additionally, the way the 
aggression levels work with evolving birds is that their liklihood
to fight is based on their aggression because it chooses a random float, 
and they will fight if the integer falls in their aggression range. Thus,
if they have a greater aggression, it is more likely that the random float
will be in the aggression range and they will fight. 

Overall from my testing, everything works well and you can experiment
to find interesting outcomes, such as how to clump all the evolving
birds in the top left corner (increase the food benefit significantly) 
as well as much more. I feel like I may have done some of the methods
a little unconventionally, but overall I attempted to make sure I was 
repeating as little code as possible and implementing inheritance and 
classes properly! 