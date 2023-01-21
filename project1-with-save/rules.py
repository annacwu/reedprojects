from random import Random, random

# * * * * * * * * * * * conway * * * * * * * * * * *
#
# Example of a grid rule. This rule gets applied to
# each grid cell, inspecting its state and the states
# of its eight neighbor cells, and is used to determine
# its next state.
#
# This particular rule encodes the behavior of Conway's 
# game of life simulation.  It takes two parameters:
#
#   cntr: the state of the grid cell being inspected
#
#   nbrs: collection of states of the 8 grid neighbors 
#         that sit around the cell being inspected
#
# This rule interprets states of 0 as "dead" and
# states of 1 and above as being "alive". 
#
# Live cells die if they have too many or too many
# living neighbors.
#
# Dead cells come alive if they have just the 
# right number of live neighbors.
#
# See the if/else below for details.
#
def conway(cntr,nbrs):

  # live
  #
  # Helper function that returns 1/0 if live/dead.
  def life(cell_value):
    if cell_value > 0:
      return 1
    else:
      return 0

  #
  # count the number of living neighbors  
  #
  living = life(nbrs.NW) + life(nbrs.N) + life(nbrs.NE) \
           + life(nbrs.W) + life(nbrs.E) \
           + life(nbrs.SW) + life(nbrs.S) + life(nbrs.SE)

  #
  # determine next state
  #
  # if alive...
  if life(cntr) == 1:
    # and there are two or three live neighbors...
    if living == 2 or living == 3:
      # survive
      return cntr
    else:
      # otherwise, die.
      return 0
  #
  # if dead...
  else:
    # but there are three live neighbors...
    if living == 3:
      # come alive.
      return 100
    else:
      return 0
#
#
# * * * * * * * * * * * conway * * * * * * * * * * *

# * * * * * * * * * generational * * * * * * * * * * *
#
# This performs Conway's game of life except, when a
# cell is alive (1-100), its value is interpreted as
# its "generation".  This means that, when a live cell
# is born, it takes on the value that's one more than 
# the max value of its live neighbors.
#
def generational(cntr,nbrs):

  # live
  #
  # Helper function that returns 1/0 if live/dead.
  def life(cell_value):
    if cell_value > 0:
      return 1
    else:
      return 0

  #
  # count the number of living neighbors  
  #
  living = life(nbrs.NW) + life(nbrs.N) + life(nbrs.NE) \
           + life(nbrs.W) + life(nbrs.E) \
           + life(nbrs.SW) + life(nbrs.S) + life(nbrs.SE)

  largest = max(nbrs.NW,nbrs.N,nbrs.NE,nbrs.W,
                nbrs.SE,nbrs.S,nbrs.SW,nbrs.E)
  #
  # determine next state
  #
  # if alive...
  if life(cntr) == 1:
    # and there are two or three live neighbors...
    if living == 2 or living == 3:
      # survive
      return cntr
    else:
      # otherwise, die.
      return 0
  #
  # if dead...
  else:
    # but there are three live neighbors...
    if living == 3:
      # come alive, marking your new generation
      return 1+largest
    else:
      return 0
#
#
# * * * * * * * * * generational * * * * * * * * * * *

# * * * * * * * * * * * blur * * * * * * * * * * * * * 
#
# Example of an image processing rule.  This blurs an
# image.  A cell becomes the average of itself with
# the average value of its neighbors.  This "blends"
# greys and "smooths" out sharp transitions.  The 
# effect of a bright pixel is spread over an area
# of the image, centered at that pixel.

def blur(cntr,nbrs):

  # compute the average value of my neighbors
  avg = (nbrs.N + nbrs.E + nbrs.S + nbrs.W)//4

  # change state so that I'm closer to their average
  return (cntr + avg) // 2

#
#              
# * * * * * * * * * * * blur * * * * * * * * * * * * * 

# * * * * * * * * * * negative * * * * * * * * * * * * 
#
# This inverts brightness to darkness, and vice versa,
# in an image.  The effect makes the image look like 
# a photographic negative.
#
def negative(cntr,nbrs):
   return 255 - cntr
#
#
# * * * * * * * * * * negative * * * * * * * * * * * * 

# * * * * * * * * * * * aging * * * * * * * * * * * * 
#
# This performs Conway's game of life, though with an 
# adjustment in which the color of the cell represents 
# its age. Once a cell reaches 100, it dies.
#
def aging(cntr,nbrs):

  # previously established conway code
  def life(cell_value):
    if cell_value > 0:
      return 1
    else:
      return 0
  
  living = life(nbrs.NW) + life(nbrs.N) + life(nbrs.NE) \
           + life(nbrs.W) + life(nbrs.E) \
           + life(nbrs.SW) + life(nbrs.S) + life(nbrs.SE)

  # variable to track number of generations of a cell
  age = 1
  
  # if a cell is alive...
  if life(cntr) == 1:

    # start cell at age 1 (i.e. turn red) instead of 100 (aka white)
    if cntr == 100:
      cntr = 1

    # if there are two or three live neighbors (Conway rule)
    if living == 2 or living == 3:

      # and if the age is under 100
      if age <= 99:
        # add 1 to its age
        age += 1 
        # add its age to the state of the cell
        cntr += age

        # if the cell reaches 100
        if cntr > 99:
          # it dies
          return 0 
        # otherwise, change cell state to its updated value
        return cntr
      # ultimately, cells will die if they do not follow life rules
      return 0
    # dead cells remain dead
    else:
      return 0
  else:
    # same code as above, added to rebirth condition of Conway's rules
    if living == 3:
      if age <= 99:
        age += 1 
        cntr += age
        if cntr > 99:
          return 0 
        return cntr
      return cntr
    else:
      return 0
# * * * * * * * * * * * aging * * * * * * * * * * * * 

# * * * * * * * * * * * * decay * * * * * * * * * * * * 
# 
# This runs a version of Conway's game of life, though
# only white cells are live, and dead cells leave 
# a color trace that “decays” from violet down to red
# before dying altogether. The decay happens with each 
# successive step of the simulation. 
#
def decay(cntr,nbrs):

  # builds on conways code, counting only white cells as alive
  def life(cell_value):
    if cell_value == 100:
      return 1
    else:
      return 0

  # helper function "death" that manages the decay sequence
  def death(value):
   # decrease color away from white to mark it as dead
   value -= 1

   # if the cell is dying
   if value < 100 and value > 0:
     # change to a lower color (4 was tested to be a good decay speed value.)
     # (Higher than 4 decayed too fast through each color and lower stayed on the screen too long.)
     value -= 4
     return value

   # eventually, die
   else:
     return 0

 # below is the same code as in the original Conway
  living = life(nbrs.NW) + life(nbrs.N) + life(nbrs.NE) \
           + life(nbrs.W) + life(nbrs.E) \
           + life(nbrs.SW) + life(nbrs.S) + life(nbrs.SE)

  if life(cntr) == 1:
    # and there are two or three live neighbors...
    if living == 2 or living == 3:
      # survive
      return cntr
    else:
      # otherwise, decay by calling the death helper function
      return death(cntr)
  
  # if dead...
  else:
    # but there are three live neighbors...
    if living == 3:
      # come alive.
      return 100
    else:
      # otherwise, decay and subsequently die
      return death(cntr)
#
# * * * * * * * * * * * * decay * * * * * * * * * * * * 

# * * * * * * * * * * * contrast * * * * * * * * * * * * 
#
# This rule increases the contrast of an image. A cell 
# goes darker in a step when its brightness is below 
# middle gray. It  goes lighter in a step when its 
# brightness is above middle gray.
#
def contrast(cntr, nbrs):
  
  # if a cell is above middle gray
  if(cntr > 50):
    # noticeably increase darkness value by 5. 
    # (5 was determined to be significant enough to show visible change between steps.)
    cntr += 5
    # change state
    return cntr 
  # alternatively, if a cell is below middle gray
  elif(cntr < 50):
    # noticeably decrease darkness value
    cntr -= 5
    # change state
    return cntr
  # otherwise, stay the same
  else: 
    return cntr
#
# * * * * * * * * * * * contrast * * * * * * * * * * * * 

# * * * * * * * * * * * * sharpen * * * * * * * * * * * *  
#
# This rule sharpens the image with each step. A cell 
# takes on a value that differentiates it from the 
# average of its neighbors. This has the global effect 
# of bringing out details, since differences between 
# neighboring pixels get amplified.
#
def sharpen(cntr,nbrs):
  # take the average value of the surrounding cells
  avg = (nbrs.N + nbrs.E + nbrs.S + nbrs.W) // 4

  # compute the reciprocal equation of blur, making it so 
  # that the cell becomes less alike from its neighbor by 
  # amplifying its own value and getting further away from the average
  cntr = cntr * 2 - avg
  # return the updated value 
  return cntr
#
# * * * * * * * * * * * * sharpen * * * * * * * * * * * *  

# * * * * * * * * * * * * edges * * * * * * * * * * * *  
#
# This is my first programmer's choice rule, as I ended 
# up attempting all three image rules. I coded two 
# different versions of this rule. This version finds 
# places where there is a transition from dark to light 
# and, in one step, cells turn brighter grey if they are 
# at a major transition.
#
def edges(cntr,nbrs):

  # compute the average darkness of neighbors 
  avg = (nbrs.N + nbrs.E + nbrs.S + nbrs.W +
          nbrs.NE + nbrs.NW + nbrs.SE + nbrs.SW)//8

  # evaluate to find only those cells which are different enough from their surrounding average, 
  # and amplify them a little by doubling them then adding 10 to make those cells more visible
  cntr = (cntr - avg) * 2 + 10
  return cntr
#
# * * * * * * * * * * * * edges * * * * * * * * * * * *  

# * * * * * * * * * * * edgesAlt * * * * * * * * * * *  
#
# This is the alternate version of edges. It produces
# a more rigid output, as the condition for edges is
# more harsh and it polarizes each cell more significantly
# to be only white or black rather than grays.
#
def edgesAlt(cntr, nbrs):
  
 # compute the average darkness around a cell
 avg = (nbrs.N + nbrs.E + nbrs.S + nbrs.W +
         nbrs.NE + nbrs.NW + nbrs.SE + nbrs.SW)//8

  # if average darkness is significantly different from cell
  # (a difference of 20 was determined to be the value that proved it to be a major transition,
  # any less than that did not have enough lines and more showed insignificant transitions)
 if(avg - cntr > 20):
   # turn white
   cntr = 255
   return cntr
 # surrounding area turns black
 else:
   return 0
#
# * * * * * * * * * * * edgesAlt * * * * * * * * * * *  

# * * * * * * * * * * * * shadow * * * * * * * * * * * *
# 
# In one step, this rule adds a shadow image underneath 
# a white pattern to the pattern's southeast. If the 
# steps continue past the first, it adds increasingly 
# high value colors under the original pattern to create 
# a rainbow of shadow images. 
#
def shadow(cntr, nbrs):
  # if there is a pattern on the screen
  if(nbrs.NW != 0):
    # increase the cells to the southeast by 5 color values
    nbrs.SE = cntr + 5
    # show changed state of southeast area
    return nbrs.SE
  # ensure pattern stays on screen in subsequent steps
  elif (cntr != 0):
    return cntr
  # otherwise, stay black
  else: 
    return 0
#
# * * * * * * * * * * * * shadow * * * * * * * * * * * *

# * * * * * * * * * * * * * fill * * * * * * * * * * * * *
#
# This rule allows the user to start with a pattern 
# where the cells are mostly black, though with white 
# cells forming a boundary of a region and a single cell 
# of a color somewhere in the middle of that region. 
# With simulation steps, the color fills the whole interior 
# of that region without “leaking out”.
#
def fill(cntr,nbrs):
  # if a cell is white, remain white
  if cntr == 100:
    return 100
  
  # if a cell is a color, remain that color
  if cntr < 100 and cntr > 0:
    return cntr

  # if the cell above is a color, become that color
  if nbrs.N < 100 and nbrs.N > 0:
    cntr = nbrs.N 
    return cntr 

  # if the cell to the right is a color, become that color
  if nbrs.E < 100 and nbrs.E > 0:
    cntr = nbrs.E
    return cntr 

  # if the cell below is a color, become that color
  if nbrs.S < 100 and nbrs.S > 0:
    cntr = nbrs.S
    return cntr 

  # if the cell to the left is a color, become that color
  if nbrs.W < 100 and nbrs.W > 0:
    cntr = nbrs.W
    return cntr 

  # otherwise, stay the same
  else: 
    return cntr
#
# * * * * * * * * * * * * * fill * * * * * * * * * * * * *

# * * * * * * * * * * * rain prep * * * * * * * * * * * *
#
# This is a helper function to prepare for the final rule,
# which requires user input to initialize the game pattern. 
# This function itself does not manipulate the cells,
# it is only meant to give the user space to draw their 
# sprites which will be affected once the game starts. This
# makes use of the pattern-saving procedure built into the
# updated version of the project file. When the user quits
# this step of the demo, it saves the grid they edited 
# and loads it at the start of the next rule.
#
def prep(cntr,nbrs):
  return cntr
#
# * * * * * * * * * * * rain prep * * * * * * * * * * * *

# * * * * * * * * * * * acid rain * * * * * * * * * * * *
#
# The final rule is a quick game. The premise of the game 
# is that there is a row of purple "acid rain" quickly 
# descending from the top of the grid. At the bottom of 
# the grid, there lies the three sprites placed by the user 
# preceeding the start of the game. Once the user runs the
# program with the "." button, the user has to place a 
# white dot above each of their sprites before they get
# hit by the rain. If they fail to protect their sprites, 
# the screen turns red. If they are successful, the sprites 
# remain on screen with their protection above them.  
#
def acid_rain(cntr, nbrs):

  # conditional statement that encompasses all cells directly underneath a row of rain, not including the sprites
  if cntr < 95 and nbrs.N > 90 and nbrs.N != 0 and nbrs.N < 100:

    # age variable starts close to target value to establish how long it will take (making use of aging logic from rule 1);
    # this had to be done to slow down the rain between steps, or else there was not enough time for the game
    # to feasibly be played because the rain moved too fast.
    age = 75
    # while age is less than 95
    while age <= 95:
        # add 1 to age
        age += 1 
        # increase value of cell closer to rain color
        cntr += 5
        
        # if the cell below is a sprite
        if nbrs.S == 12:
          # turn red to indicate failure
          return 6
        
        # if there is red, all surrounding cells turn red (encompassing whole screen in a failure sequence)
        if nbrs.W == 6:
          return 6
        elif nbrs.E == 6:
          return 6
        elif nbrs.S == 6:
          return 6
        elif nbrs.N == 6:
          return 6

        # if the cell reaches rain color (95)
        if cntr > 94:
          # stay that color
          cntr = 95
          return cntr
        
        # if the cell below is white
        if nbrs.S == 100:
          # stop by turning purple (one value higher than the rain color to ensure it does not continue in the next iteration)
          return 96
          
        # if no preceeding conditions fit, change cell state to updated value (this is the rain)
        return cntr
    return cntr
  
  # alternatively, if you are not one of the cells in the row below the rain
  else: 

    # execute failure sequence if there is red on the screen
    if nbrs.W == 6:
      return 6
    elif nbrs.E == 6:
      return 6
    elif nbrs.S == 6:
      return 6
    elif nbrs.N == 6:
      return 6

    # if there is not red, stay the same
    return cntr 
 
# * * * * * * * * * * * acid rain * * * * * * * * * * * *