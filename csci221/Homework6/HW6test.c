#include "Homework6.h"
#include <stdio.h>

int main (){
    
    // TEST for 2e
    printf("Question 2e: ");
    // Question 2e
    iCPU* mycpu = iCPU_init();
    write_instruction(1,1,0,4,0, mycpu);
    write_instruction(1,2,0,6,1, mycpu); 
    write_instruction(2,0,1,2,2, mycpu); 
    write_instruction(5,0,0,1,3, mycpu); 
    write_instruction(0,0,0,0,4, mycpu);
    write_instruction(9,0,0,0,5, mycpu); // terminate
    run_program(mycpu);
    iCPU_destroy(mycpu);
    printf("expected: 40\n");

    // TEST for 4
    printf("\nQuestion 4: ");
    // Question 4
    iCPU* cpuu = iCPU_init();
    write_instruction(1,2,0,1,0,cpuu); // put 1 in r2 (iterator)
    write_instruction(1,5,0,101,1,cpuu); // put 100 into r5
    write_instruction(1,1,0,1,2,cpuu); // put 1 in r1
    write_instruction(1,4,0,20,3,cpuu); // put 20 in r4
    write_instruction(4,0,4,0,4,cpuu); // r0 = value in 20th place in RAM

    write_instruction(7,2,5,9,5,cpuu); // if r2 < 101
    write_instruction(2,0,1,0,6,cpuu); // r0 = r1 + r0
    write_instruction(2,2,1,2,7,cpuu);
    write_instruction(6,0,0,5,8,cpuu); // jump back to line 5

    write_instruction(0,0,0,0,9,cpuu); // print
    write_instruction(9,0,0,0,10,cpuu); // terminate
    run_program(cpuu);
    iCPU_destroy(cpuu);
    printf("expected: 100\n");

    // Other tests: 
    printf("\nOther tests for write_instruction(), run_program(), clock_cycle(), and read_instruction():\n");
    
    // test one: 1 + 3 + 5 + 7 + 9
    // expected: 25
    printf("\nTest 1: ");
    iCPU* mycpuu = iCPU_init();
    write_instruction(1,1,0,1,0, mycpuu); // put 1 in r1
    write_instruction(1,2,0,3,1, mycpuu); // put 3 in r2
    write_instruction(2,0,1,2,2, mycpuu); // r0 = r1 + r2
    write_instruction(1,1,0,5,3, mycpuu); // put 5 in r1
    write_instruction(2,0,0,1,4, mycpuu); // r0 = r0 + r1
    write_instruction(1,1,0,7,5, mycpuu); // put 7 in r1
    write_instruction(2,0,0,1,6, mycpuu); // r0 = r0 + r1
    write_instruction(1,1,0,9,7, mycpuu); // put 9 in r1
    write_instruction(2,0,0,1,8, mycpuu); // r0 = r0 + r1
    write_instruction(0,0,0,0,9, mycpuu); // print r0
    write_instruction(9,0,0,0,10,mycpuu); // terminate
    run_program(mycpuu);
    iCPU_destroy(mycpuu);
    printf("expected: 25\n");

    // test 2: x++
    // expected: 1
    printf ("\nTest 2: ");
    iCPU* myycpuu = iCPU_init();
    write_instruction(1,1,0,1,0,myycpuu); // put 1 in r1
    write_instruction(1,2,0,20,1,myycpuu); // put 20 in r2
    write_instruction(4,2,2,0,2, myycpuu); // r2 = value in 20th place in RAM
    write_instruction(2,0,1,2,3,myycpuu); // r0 = r1 + r2
    write_instruction(0,0,0,0,4,myycpuu); // print r0
    write_instruction(9,0,0,0,5, myycpuu); // terminate
    run_program(myycpuu); 
    iCPU_destroy(myycpuu);
    printf("expected: 1\n");

    // test 3: array of first 5 powers of 2
    // expected: 2, 4, 8, 16, 32
    printf("\nTest 3: \n");
    iCPU* myycpuuu = iCPU_init();
    write_instruction(1,3,0,2,0, myycpuuu); //put 2 in r3
    write_instruction(1,1,0,2,1, myycpuuu); //put 2 in r1
    write_instruction(1,2,0,50,2, myycpuuu); //put 50 in r2
    write_instruction(3,1,2,0,3, myycpuuu); //put r1 in RAM[r2]
    write_instruction(4,0,2,0,4, myycpuuu); // put ram[r2] in r0
    write_instruction(0,0,0,0,5, myycpuuu); // print r0
    write_instruction(5,1,3,1,6, myycpuuu); // r1 = r3*r1
    write_instruction(1,2,0,51,7, myycpuuu); //put 51 in r2
    write_instruction(3,1,2,0,8, myycpuuu); //put r1 in RAM[r2]
    write_instruction(4,0,2,0,9, myycpuuu); // put ram[r2] in r0
    write_instruction(0,0,0,0,10, myycpuuu); // print r0
    write_instruction(5,1,3,1,11, myycpuuu); // r1 = r3*r1
    write_instruction(1,2,0,52,12, myycpuuu); //put 52 in r2
    write_instruction(3,1,2,0,13, myycpuuu); //put r1 in RAM[r2]
    write_instruction(4,0,2,0,14, myycpuuu); // put ram[r2] in r0
    write_instruction(0,0,0,0,15, myycpuuu); // print r0
    write_instruction(5,1,3,1,16, myycpuuu); // r1 = r3*r1
    write_instruction(1,2,0,53,17, myycpuuu); //put 53 in r2
    write_instruction(3,1,2,0,18, myycpuuu); //put r1 in RAM[r2]
    write_instruction(4,0,2,0,19, myycpuuu); // put ram[r2] in r0
    write_instruction(0,0,0,0,20, myycpuuu); // print r0
    write_instruction(5,1,3,1,21, myycpuuu); // r1 = r3*r1
    write_instruction(1,2,0,54,22,myycpuuu); //put 54 in r2
    write_instruction(3,1,2,0,23, myycpuuu); //put r1 in RAM[r2]
    write_instruction(4,0,2,0,24, myycpuuu); // put ram[r2] in r0
    write_instruction(0,0,0,0,25, myycpuuu); // print r0
    write_instruction(9,0,0,0,26, myycpuuu); // terminate
    run_program(myycpuuu);
    iCPU_destroy(myycpuuu);
    printf("expected: 2, 4, 8, 16, 32\n");


    return 0;
}