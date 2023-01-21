from Grid import Grid, GREYS, COLORS
from rules import *
from splash import greet 

print()
greet()
print()
print("Select a color or gray tone and then")
print("paint cell patterns with the mouse.")
print()
print('Hit [SPACE] to step.')
print('Hit [DOT] to run.')
print('Hit [SLASH] to reset.')
print('Hit 0-9 then \'=\' to choose a color.') 
print('Hit \'s\' to save a pattern.')
print('Hit \'q\' to quit.')
print()
print("**NOTE**--*--*--*--*--*--*--*--*--*--*--*-")
print("Click the window to make keys work.")
print("Click the console for file name entry.")

Grid(conway)
# Grid(conway,pattern='patterns/rpentamino.pat')
# Grid(generational,pattern='patterns/rpentamino.pat')
# Grid(negative,pattern='images/reed-square.pgm')
# Grid(blur,pattern='images/reed.pgm')

# Below is an example of a demo of the given rules.
# Uncomment and modify the demo to show your rules.
# To uncomment, get rid of the """ lines at the top
# and at the bottom

"""
Grid(conway,
     '1. This runs a SE-traveling glider in Conway. The simulation runs long\
 enough to have the glider traverse the world.',
     pattern='patterns/glider.pat', generations=64)

Grid(generational,
     '2. This runs the R pentamino in generational Conway. Later cells will\
 be of advanced generations, and so will be colored further in the spectrum.',
     pattern='patterns/rpentamino.pat', generations=98)

Grid(blur,'3. This will blur an image.',
     pattern='images/reed.pgm')

Grid(negative,'4. This will form the negative of an image.',
     pattern='images/reed-square.pgm')
"""


