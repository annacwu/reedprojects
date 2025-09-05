main:
li $t1, 0 # i = 0
li $t2, 2 # store 2 for arithmetic
li $t3, 1 
; li $a0, 4 # puts 4 in a0 to test the program, not implicitly part of the function
sub $a0, $a0, $t3 # loop runs one too many times w/o this

loop:
bge $t1, $a0, endLoop # if i < $a0
mul $t2, $t2, 2 # 2 * 2
add $t1, $t1, 1 # i++

j loop # jump back to start of loop

endLoop: 
move $v0, $t2 # store final value in register

# gives errors at the end, but if you look the right value is stored in v0.
# errors are due to the fact that the program is not terminated properly