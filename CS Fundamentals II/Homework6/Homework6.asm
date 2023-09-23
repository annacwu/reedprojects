# Question 5

main:
li $t0, 0 # counter

yloop:
beq $t0, $a3, endloops # if i = size
lw $t1, ($a2) # load $a2 into $t1 from ys in  RAM
addi $a2, $a2, 4 # move pointer to next spot in array

bge $t1, $a1, loopagain # if xs[y] is not in range, go to next value
mul $t1, $t1, 4 # y * 4
move $t3, $a0 # move pointer to xs[0] into $t3
add $t3, $t3, $t1 # increment address by number of spaces moved by y
lw $t2, ($t3) # get x value of xs[y]

addi $t2, $t2, 1 # x++
sw $t2, ($t3) # put x value from $t2 into address still in $t3
loopagain:
j yloop 

endloops:
syscall

# Question 6

g: 
addi $sp, $sp, -4 # creates space for pc
sw $ra, ($sp) # save pc
addi $sp, $sp, -4 # allocate space
li $t0, 3 # i = 5
sw $t0, ($sp) # put i on stack
lw $t1, 8($sp) # get to argument (int y)
add $t0, $t0, $t1 # i + y

move $v0, $t0 # return in v0
lw $ra, 4($sp)
addi $sp, $sp, 8 # deallocates space
jr $ra # return to f

f: 
addi $sp, $sp -4 # space to save pc
sw $ra, ($sp) # save pc
addi $sp, $sp, -4 # allocate space
li $t0, 5 # i = 5
sw $t0, ($sp) # put i on stack
addi $sp, $sp, -4 # allocate space for g parameters
lw $t1, 12($sp) # get x and put it in $t1
sw $t1, ($sp) # stores x current stack pc
jal g # assume after this that g(x) is now stored in v0

add $v0, $t0, $v0 # return i + g(x)
lw $ra, 8($sp)
addi $sp, $sp, 12 # deallocates space
jr $ra # return to main

main:
addi $sp, $sp, -4 # creates space for z on stack
li $t0, 2 # z = 2
sw $t0, ($sp) # puts z = 2 on the stack

addi $sp, $sp, -4 # allocate for f parameter
lw $t1, 4($sp) # put z = 2 on stack for f
sw $t1, ($sp)
jal f # result of f stored in v0

addi $v0, $v0, 1 # z++
sw $v0, ($sp) # store z++ on stack
addi $sp, $sp, 8 # deallocate
move $v0, $zero # return 0
syscall
