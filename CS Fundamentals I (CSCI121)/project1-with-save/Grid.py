#
# CSCI 121
# Project 1: grid
#
# See life.py for examples that use the code below.
#

"""Grid class
  
    Defines an object that performs a "grid simulation".
    This can be used in two ways:

       1. to perform two-dimensional cellular automata simulations
       2. to perform greyscale image processing

    In the first case (1), the Grid tracks an integer state for each
    of the cells in the simulation. We provide the Grid with a file
    that describes the initial states of all the cells. We then
    run the simulation, by modifying all the cells, changing their
    state according to a specified rule. That rule is given as
    a Python function, and its effect is to read the state of a
    grid cell, along with the states of its 8 neighboring cells,
    so as to determine that cell's next state.

    The rule is a Python function declared like so:

       def rule(cntr,nbrs):
         ... code for rule ...

    The value `cntr` stands for a cell's state. The object `nbrs` holds
    the states of all the neighboring cells. 

    Examples of cellular automata rules can be found in `life.py`.

    In the case of (2), the grid cells correspond to pixels in a
    greyscale image.  Low values code for darker pixels and higher
    values code for brighter pixels.  The rule, then, changes the
    pixels of the image and can be ,ade to perform image processing
    operations. The rule applies "locally" to each pixel, can be based
    on the pixel's neighbor's values, and so can be an image "filter."

"""
 
from tkinter import *
from colorsys import hsv_to_rgb

offsetRows = [-1,-1,-1,0,0,1,1,1]
offsetColumns = [-1,0,1,-1,1,-1,0,1]
COLORS = 0
GREYS = 1

class Neighbors:
    def __init__(self,grid,r,c):
        dr = offsetRows
        dc = offsetColumns
        self.list = [grid.get(r+dr[i],c+dc[i]) for i in range(8)]
        self.NW = self.list[0]
        self.N  = self.list[1]
        self.NE = self.list[2]
        self.W  = self.list[3]
        self.E  = self.list[4]
        self.SW = self.list[5]
        self.S  = self.list[6]
        self.SE = self.list[7]
 
class Grid(Frame):

    LEFT = 8
    TOP  = 8
  
    # * * * * CELL STATE ACCESSORS

    #
    # get - get the state of the given cell
    #
    def get(self,row,column):
        """ Accesses the state of the cell at the given
        row and column.  That state will be a value
        between max and min.  

        NOTE: row and column are interpreted modulo
        rows and modulo columns, respectively
        """
        
        return self.cell[row % self.rows][column % self.columns]

    #
    # around - get the state of the neighboring cells
    #
    def around(self,row,column):
        """ Gets the states of the cells surrounding the
        given cell.

        The object returned is a Neighbors instance, and
        each cell's state can be accessed with .NE, .S, etc.
        or with .list[0] thru .list[7]
        """
        
        return Neighbors(self,row,column)

    #
    # set - set the state for the given cell
    #
    def set(self,row,column,value):
        """ Sets the next state of the cell at the given
        row and column to the given state value. 

        NOTE:  That state has a ceiling of `max` and a
        floor of `min`, and is "trimmed" accordingly.

        NOTE: row and column are interpreted modulo
        rows and modulo columns, respectively.
        """
        
        if value > self.max:
            # trim the value to the ceiling
            value = self.max
        if value < 0:
            # trim the value to the floor
            value = 0

        # change that entry     
        self.next[row % self.rows][column % self.columns] = value


    # * * * * RENDERING

    #
    # squareAt - pixel for the given cell
    #
    def squareAt(self,row,column):
        """ Determines the coordinates of the corners of the given 
        cell's pixel.
        """

        # (upper left x, upper left y, 
        #  lower right x, lower right y)

        return (self.LEFT+column*self.size, self.TOP+row*self.size, \
                self.LEFT+(column+1)*self.size, self.TOP+(row+1)*self.size)

    #
    # shadeOf - gives back the color of the cell's state
    #
    def shadeOf(self,value):
        """ Determines the W2C hexadecimal RGB triplet string
        for rendering the given cell's state.
        """
        
        if value == 0:
            # min state is BLACK
            return 'black'

        elif value == self.max:
            # max state is WHITE
            return 'white'

        else:
            # otherwise...

            # scale the value between 0.0 and 1.0
            scaled = value / self.max

            # check the display mode
            if self.mode == GREYS:
                # and compute the greyscale triplet   
                triplet = (scaled,scaled,scaled)

            else:
                # or compute the color triplet.
                triplet = hsv_to_rgb(0.75*scaled,1.0,1.0)

            # scale up to byte values
            rgb = ((int)(triplet[0]*255), \
                   (int)(triplet[1]*255), \
                   (int)(triplet[2]*255))

            # and give back a hex code used by tkinter
            return '#%02x%02x%02x' % rgb
        
    #
    # render - draws the current states of the cells
    #
    def render(self,force=True):
        """ Renders the state of the simuation on the tkinter canvas. """

        # refresh the screen
        if force or len(self.changes) > 100:
            self.gridFullRefresh()
        else:
            self.gridDeltaRefresh()

        # draw the palette
        if not self.demo:
            self.PALETTE.delete('all')
            self.drawPalette()

    def drawPalette(self):
        # draw the palette
        self.PALETTE.create_rectangle((10,10,10+32,32), 
                                      fill="black", 
                                      outline="black")
        self.PALETTE.create_rectangle((10+self.width-32,10,
                                      10+self.width,32), 
                                      fill="white", 
                                      outline="white")
        block = (self.width - 64)//4
        for i in range(1,block):
            rect = (10+32+i*4-4, 10, 10+32+i*4+4, 32)
            shade = self.shadeOf(int(i/block*self.max))
            self.PALETTE.create_rectangle(rect, 
                                          fill=shade, 
                                          outline=shade)

        # highlight the selected shade    
        if 0 < self.selected and self.selected < self.max:
            i = self.selected * (self.width - 64) // self.max
            rect = (10+32+i, 10, 10+32+i+4, 32)
            shade = self.shadeOf(self.selected)
            self.PALETTE.create_rectangle(rect, 
                                          fill=shade, 
                                          outline="white")
        elif self.selected == self.max:
            self.PALETTE.create_rectangle((10+self.width-32,10,
                                          10+self.width,32), 
                                          fill="white", 
                                          outline="black")
        elif self.selected == 0:
            self.PALETTE.create_rectangle((10,10,10+32,32), 
                                          fill="black", 
                                          outline="white")

    def drawPixelAt(self,r,c,edge):
        rect = self.squareAt(r,c)
        value = self.get(r,c)
        shade = self.shadeOf(value)
        self.canvas.create_rectangle(rect, fill=shade, outline=edge)

    def gridFullRefresh(self):
        # remove past pixels
        self.canvas.delete('all')

        # determine the shader
        if self.mode == GREYS:
            edge = 'black'
        else:
            edge = 'white'

        # draw the pixels
        for r in range(self.rows):
            for c in range(self.columns):
                self.drawPixelAt(r,c,edge)
    
        self.changes = []
              
    def gridDeltaRefresh(self):
        # determine the shader
        if self.mode == GREYS:
            edge = 'black'
        else:
            edge = 'white'

        # draw the pixels
        for location in self.changes:
            r,c = location
            self.drawPixelAt(r,c,edge)

        self.changes = []
    
    #
    # dimensions - determines window & pixel size
    # 
    def set_dimensions(self):

        minsize = 4
        maxsize = 32
        maxwidth = 1024
        maxheight = 512

        xsize = maxwidth // self.columns
        ysize = maxheight // self.rows
        size = min(xsize,ysize)

        if size < minsize:
            self.scrolls = True
            self.size = minsize
        elif size > maxsize:
            self.size = maxsize
            self.scrolls = False
        else:
            self.scrolls = False
            self.size = size
      
        if self.scrolls and self.mode == GREYS:
            self.scrolls = False
            h = maxheight // self.size 
            w = maxwidth // self.size
            w_fromh = h * self.columns // self.rows
            h_fromw = w * self.rows // self.columns
            if w < w_fromh:
                self.width = w * self.size
                self.height = h_fromw * self.size
            else:
                self.width = w_fromh * self.size
                self.height = h * self.size
        else:
            self.width = self.columns * self.size
            self.height = self.rows * self.size
     

    # * * * * SIMULATION CONTROL

    #
    # advance - draws the current states of the cells
    #
    def advance(self):
        """ Advances the simulation one step, displays the result. """
    
        # if there's more simulation to compute
        if self.generations == None or self.clock < self.generations: 

            # advance the clock
            self.clock = self.clock + 1

            # advance the simulation by computing 'next'
            for r in range(self.rows):
                for c in range(self.columns):
                    before = self.get(r,c)
                    after  = self.rule(before,self.around(r,c))
                    self.set(r,c,after)
                    if before != after:
                        self.changes.append((r,c))

            # swap the roles of 'next' grid with prior grid 'cell'
            temp = self.cell
            self.cell = self.next
            self.next = temp

        # draw the result
        self.render()

    #
    # save - save the current grid state to a file
    #
    def save(self,filename=None):
        if filename is None:
            fn = input("SAVING GRID PATTERN. Enter a file name: ")
        else:
            fn = filename
          
        try:
            fl = open(fn,"w")
            fl.write(str(self.columns) + " " + str(self.rows) + "\n")
            fl.write(str(self.max) + "\n")
            for r in range(self.rows):
                first = True
                for c in range(self.columns):
                    cell_value = self.get(r,c)
                    if not first:
                        fl.write(" ")
                    fl.write(str(cell_value))
                    first = False
                fl.write("\n")
            fl.close()
        except:
            print("Saving a file didn't work for some reason.")
        
        
    #
    # paint - handle a mouse paint event
    #
    def paint(self,event):
        row = (event.y - 8)//self.size
        column = (event.x - 8)//self.size
        if row < 0: return
        if row >= self.rows: return
        if column < 0: return
        if column >= self.columns : return
        self.cell[row][column] = self.selected
        self.changes.append((row,column))

    #
    # donePaint - handles the last mouse paint event
    #
    def donePaint(self,event):
        self.paint(event)
        self.render()

    #
    # select - react to a mouse press inside the palette
    #
    def select(self,event):
        """ Handles mouse color select events. """
        if event.x < 40:
            self.selected = 0
        elif event.x > self.width-40:
            self.selected = self.max
        else:
            self.selected = int((event.x - 40)/(self.width-64)*self.max)
        self.render()
        
    #
    # keypress - react to a keypress
    #
    def keypress(self,event):
        """ Handles tkinter keyboard events. """

        if event.char == ' ':
            # SPACE makes single step
            self.running = False
            self.advance()

        
        elif event.char == '.':
            # '.' turns simulation on/off
            self.running = not self.running

        
        elif event.char == '/':
            # '/' resets the simulation
            self.reset()
            self.render()
          
        elif event.char == 'q':
            # 'q' key exits this simulation
            if self.save_as is not None:
                self.save(self.save_as)
            self.quit()

      
        elif event.char == 's':
            # 's' key saves the cell grid to a file
            self.save()

        elif event.char >= '0' and event.char <= '9':
            # digit keys enter a color value
            self.entry += event.char
                           
        # '=' key selects the entered color value
        elif event.char == '=':
            if self.entry == "":
                self.entry = "0"
            self.selected = min(max(int(self.entry),0),self.max)
            print("The paint cell value is now "+str(self.selected)+".")
            self.entry = ""                
            
    #
    # tick - one step of the background activity
    #
    def tick(self):
        """ This method gets called in the background,
        at regular intervals determined by 'delay',
        advancing the simulation one step.
        """

        # if we're still running, advance it
        if self.running:
            self.advance()

        # draw it
        self.render()

        # register another tick event
        self.root.after(self.delay,self.tick)

    #
    # reset - resets the simulation
    #
    def reset(self):      
        """ Resets the simulation, as if a fresh start. """

        # restart the clock
        self.running = False
        self.clock = 0

        # copy the loaded state
        for r in range(self.rows):
            for c in range(self.columns):
                self.cell[r][c] = self.init[r][c]
   
    # * * * * SIMULATION INITIATION

    #
    # load - loads the initial grid states from the given 
    #        file
    #
    def load(self,filename):      
        """ Reads the contents of the given file into self.input. """

        def open_and_read_header():
            if filename[-4:]=='.pat': 
                #
                file = open(filename,'r')
                print('File is ',file,'.',sep='')
                dims = file.readline().split(' ')
                #
                self.mode = COLORS

            elif filename[-4:]=='.pgm': 
                #
                file = open(filename,'r')
                print('File is ',file,'.',sep='')
                header = file.readline()
                comment = file.readline()[:-1]
                while comment[0] == '#':
                    comment = file.readline()[:-1]
                dims = comment.split(' ')
                #
                self.mode = GREYS

            else:
                return None

            self.columns = int(dims[0])
            self.rows = int(dims[1])
            self.max = int(file.readline())
            return file

        def read_entries(file):
            entries = ''
            line = ' '
            # read all the text (a series of integer values)
            while line != '':
                entries = entries + ' ' + line
                line = file.readline()[:-1]
            # parse the text, reading in row-major order
            r = 0
            c = 0
            for entry in entries.split(' '):
                if entry != '':
                    self.input[r][c] = int(entry)
                    c = c + 1
                    if c >= self.columns:
                        c = 0
                        r = r + 1 

        file = open_and_read_header()
        self.input = [[0 for c in range(self.columns)]\
                      for r in range(self.rows)]
        if file != None: read_entries(file)

    def subsample(self):
        box  = self.columns / (self.width // self.size)
        boxi = self.columns // (self.width // self.size)
        self.init = [[0 for c in range(self.columns)]
                     for r in range(self.rows)]
        print(box,boxi)
        for r in range(self.rows):
            for c in range(self.columns):
                rr = int(r * box)
                cc = int(c * box)
                v = 0
                for i in range(boxi):
                    for j in range(boxi):
                        v = v + self.input[rr+i][cc+j]
                self.init[r][c] = v // (boxi * boxi)
        self.columns = self.width // self.size
        self.rows = self.height // self.size

    #
    # build - creates the storage space, and loads the
    #         initial state
    #
    def build(self,filename):      
        """ Constructs init, cell, next. Each a matrix of cell states. 

        init: states of cells with simulation reset
        cell: current states of cells
        next: states of cells with simulation advance

        """
        if filename != None: 
            self.load(filename)
        
        self.set_dimensions()

        if self.width // self.size != self.columns: 

            print('width is ',self.width,', columns are ',self.columns,
                  '. subsampling...', sep='')
            self.subsample()

        else:
            if filename == None:
                self.init = [[0
                              for c in range(self.columns)]
                             for r in range(self.rows)]
            else:
                self.init = [[self.input[r][c] 
                              for c in range(self.columns)]
                             for r in range(self.rows)]

        self.cell = [[self.init[r][c]
                      for c in range(self.columns)]
                     for r in range(self.rows)]

        self.next = [[0 for c in range(self.columns)]\
                     for r in range(self.rows)]

    #
    # __init__ - constructs a new Grid instance 
    #
    def __init__(self, rule, info = None,
                       rows=16, columns=16, mode=COLORS, \
                       pattern=None, max=100, \
                       save_as=None, \
                       delay=10, generations=None):

        """ Performs a new Grid simulation with the given parameters 

        info: brief description of the simulation
        rule: function used to update each cell's state
        rows: number of grid rows
        columns: number of grid columns
        mode: COLORS or GREYS, makes display rainbow or greyscale
        pattern: filename for initial state of cells (.pat or .pgm)
        min, max: lowest and highest cell state values
        delay: animation update (in milliseconds)
        generations: number of updates, None if unlimited
        
        """    

        # initialize the world
        if info == None:
            self.demo = False
        else:
            self.demo = True

        self.changes = []
      
        self.max = max
        self.selected = max
        self.entry = ""
        self.rows = rows
        self.columns = columns
        self.mode = mode
        self.rule = rule
        self.build(pattern)
        self.save_as = save_as
        self.reset()
    
        # determine display parameters
        self.generations = generations
        self.delay = delay

        # initialize the display
        self.root = Tk()
        self.root.title('CSCI 121 Grid')
        Frame.__init__(self, self.root)
        if self.demo:
            self.TITLE = Text(self.root, height=4, width=60)
            self.TITLE.pack()
            self.TITLE.insert(END, info)
        else:
            self.PALETTE = Canvas(self.root, width=self.width+10, height=32)
            self.PALETTE.pack()
            self.PALETTE.bind('<Button-1>', self.select)
        self.canvas = Canvas(self.root, width=self.width+10, height=self.height+10)
        self.canvas.pack()
        self.root.bind('<Key>', self.keypress)
        self.canvas.bind('<Button-1>', self.paint)
        self.canvas.bind('<B1-Motion>', self.paint)
        self.canvas.bind('<ButtonRelease-1>', self.donePaint)
        self.pack()

        self.render(force=True)

        # run the simulation
        self.after(10,self.tick)
        self.mainloop()
        self.root.destroy()
