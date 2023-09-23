main: 
li $a0, 3 # for program execution, not inherently part of the program

numberOfBitsNotSet:
addi $sp, $sp, -4
sw $ra, ($sp) # storing PC

li $t0, 0 # number of 0s
li $t1, 0 # counter
li $t3, 32 # to terminate loop

loop:
beq $t3, $t1, endloop # end loop if checkd 32 bits
move $t2, $a0 # store value of a0 in register
srl $a0, $a0, 1 # shift once to the right
sll $a0, $a0, 1 # shift once to the left
bne $a0, $t2, skip # if they are not equal (i.e. there was a 1), dont add
addi $t0, $t0, 1 # add to the zero counter

skip:
addi $t1, $t1, 1 # increment counter
srl $a0, $a0, 1 # shift to next bit
j loop

endloop:
move $v0, $t0
lw $ra, ($sp)
addi $sp, $sp, 4
jr $ra