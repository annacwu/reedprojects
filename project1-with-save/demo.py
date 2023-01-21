# imports to help python run each of the statements below
from Grid import Grid
from rules import *
from splash import greet 

# introduction and instructions printed in terminal (mostly copied from life.py)
print()
# splash file was slightly altered to have a different greeting line specific to my project
greet()
print()
print("Type in a color/tone from 0(black) to 100(white) and paint")
print("cell patterns with the mouse.")
print()
print('Hit [SPACE] to step.')
print('Hit [DOT] to run.')
print('Hit [SLASH] to reset.')
print('Hit \'q\' to quit.')

# the following statements run each of my additional rules from the rules file with a very short description posted at the top. There are 10 such statements. For more detailed descriptions, see the blurb above each rule in the rules.py file.

# this is life rule 1, the aging rule
Grid(aging,
     '1. This runs an "aging" version of conway in which the color of the cell represents its age, dying at 100.')

# this is life rule 2, the decay rule
Grid(decay,
     '2. This runs a version of conway in which the dead cells leave a color trace. For best effect, make a glider pattern to start.')

# this is image rule 1, the contrast rule
Grid(contrast,
     '3. This will increase the contrast of an image.',
      pattern='images/reed.pgm')

# this is image rule 2, the sharpen rule
Grid(sharpen,
     '4. This will make the cells less alike from each other, sharpening the image.',
     pattern='images/reed.pgm')

# this is image rule 3 and also my first programmer's choice rule, the highlight edges rule
Grid(edges, 
     '5. This will highlight the edges of an image.', 
     pattern='images/reed.pgm')

# this is an alternative version of image rule 3
Grid(edgesAlt,
     '5.5. This is an alternative, more rigid version of edges where only the most significant transitions turn bright white.',
     pattern='images/reed.pgm')

# this is shape rule 1, the fill rule
Grid(shadow,
     '6. This will add a shadow image of a pattern to the southeast. For maximum effect, draw pattern in the upper lefthand corner.')

# this is shape rule 2, the shadow rule
Grid(fill,
     '7. This will allow you to fill a closed white shape with a color by placing a single colored dot in the middle of the shape.')

# this is the pre-rule to my second, original programmer's choice rule
Grid(prep,
     '8ish. Follow instructions carefully! (scroll for all)\nPlace three 12 value dots toward the bottom of the screen.\nThese are your beloved sprites! I sure hope nothing happens to them. (scroll for more)\nDo not change anything else about the pattern.\nQuit when finished.',
     pattern='patterns/preprep.pat',
     save_as='patterns/rainprep.pat')

# this is the second programmer's choice rule, which runs a small game using logic from aging and fill
Grid(acid_rain,
     '8. When you press ".", the acid rain will begin.\nYour color is already set to white.\nAFTER YOU PRESS "." PLACE A WHITE DOT ABOVE YOUR SPRITES TO SAVE THEM! (scroll for more)\nIf you fail, the screen will turn red.',
     pattern='patterns/rainprep.pat')
