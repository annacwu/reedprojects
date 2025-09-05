#include "Homework5.h"
#include <stdio.h>
#include <stdint.h>

int main(){

    // TESTS for maximum()
    printf("\nTests for maximum():\n");

    // test 1: {100, 2, 58, 738, 4}
    // expected output: 738
    uint32_t array[5] = {100, 2, 58, 738, 4}; 
    printf("test 1 -- {100, 2, 58, 738, 4}: %d\texpected: 738\n", *(uint32_t*) maximum(&array, sizeof(uint32_t), 5));

    // test 2: {'d', 'g', 'a', 'z'}
    // expectd output: z
    char arrayy[4] = {'d', 'g', 'a', 'z'};
    printf("test 2 -- {'d', 'g', 'a', 'z'}: %c\texpected: z\n", *(char*) maximum((void*) arrayy, sizeof(char), 4));

    // test 3: {1, 2, 3, 4, 5}
    // expected output: 5
    uint8_t arrayyy[5] = {1, 2, 3, 4, 5}; 
    printf("test 3 -- {1, 2, 3, 4, 5}: %d\texpected: 5\n", *(uint8_t*) maximum(&arrayyy, sizeof(uint8_t), 5));

    // TESTS for CmpInt()
    printf("\nTests for CmpInt():\n");

    // test 1: 5, 7
    // expected output: -1
    int intx = 5;
    int inty = 7;
    printf("test 1 -- 5, 7: %d\texpected: -1\n", CmpInt(&intx, &inty));

    // test 2: 7, 5
    // expected output: 1
    int intxx = 7;
    int intyy = 5;
    printf("test 2 -- 7, 5: %d\texpected: 1\n", CmpInt(&intxx, &intyy));

    // test 3: 7, 7
    // expected output: 0
    int intxxx = 7;
    int intyyy = 7;
    printf("test 3 -- 7, 7: %d\texpected: 0\n", CmpInt(&intxxx, &intyyy));

    // TESTS for CmpFloat()
    printf("\nTests for CmpFloat():\n");

    // test 1: 3.4, 7.8
    // expected output: -1
    float fx = 3.4;
    float fy = 7.8;
    printf("test 1 -- 3.4, 7.8: %d\texpected: -1\n", CmpFloat(&fx, &fy));

    // test 1: 7.8, 3.4
    // expected output: 1
    float fxx = 7.8;
    float fyy = 3.4;
    printf("test 2 -- 7.8, 3.4: %d\texpected: 1\n", CmpFloat(&fxx, &fyy));

    // test 1: 7.8, 7.8
    // expected output: 0
    float fxxx = 7.8;
    float fyyy = 7.8;
    printf("test 3 -- 7.8, 7.8: %d\texpected: 0\n", CmpFloat(&fxxx, &fyyy));

    // TESTS for maximum_v2()
    printf("\nTests for maximum_v2():\n");

    // test 1: {100, 2, 58, 738, 4}
    // expected output: 738
    int aryay[5] = {100, 2, 58, 738, 4};
    printf("test 1 -- {100, 2, 58, 738, 4}: %d\texpected: 738\n", *(int*) maximum_v2(&aryay, sizeof(int), 5, CmpInt));

    // test 2: {10.0, 0.2, 5.8, 7.3, 0.4}
    // expected output: 10.0
    float arnay[5] = {10.0, 0.2, 5.8, 7.3, 0.4};
    printf("test 2 -- {10.0, 0.2, 5.8, 7.3, 0.4}: %f\texpected: 10.0\n", *(float*) maximum_v2(&arnay, sizeof(float), 5, CmpFloat));

    // test 3: {16, 8, 4, 2}
    // expected output: 16
    int arhay[4] = {16, 8, 4, 2};
    printf("test 3 -- {16, 8, 4, 2}: %d\texpected: 16\n", *(int*) maximum_v2(&arhay, sizeof(int), 4, CmpInt));

    // TESTS for CPU_init(), CPU_destroy, and CPU: Utility
    printf("\nTests for CPU_init(), CPU_destroy(), and CPU Utility:\n");

    // test 1
    printf("Checkpoint 2 output:\n");
    sCPU* cpu_ptr = CPU_init();
    CPU_step(0,0,0,0, cpu_ptr); // print r0
    CPU_step(1,0,1,1, cpu_ptr); // put 0b 00000001 00000001 = 257 into r0
    CPU_step(0,0,0,0, cpu_ptr); // print r0
    printf("expected: 0, 257\n");

    // test 2
    printf("\nCheckpoint 2 test 2:\n");
    CPU_step(1,0,0,0, cpu_ptr); // put 0b0 into r0
    CPU_step(0,0,0,0, cpu_ptr); // print r0
    CPU_step(1,0,5,6, cpu_ptr); // put 1286 into r0
    CPU_step(0,0,0,0, cpu_ptr); // print r0
    printf("expected: 0, 1286\n");
    CPU_destroy(cpu_ptr);

    // TESTS for CPU ALU
    printf("\nTests for CPU ALU:\n");

    // test 1
    printf("Checkpoint 3 output:\n");
    sCPU* cpualu_ptr = CPU_init();
    CPU_step(1,1,0,4, cpualu_ptr); // put a 4 into r1
    CPU_step(1,2,0,6, cpualu_ptr); // put a 6 into r2
    CPU_step(2,0,1,2, cpualu_ptr); // r0 = r1 + r2
    CPU_step(5,0,0,1, cpualu_ptr); // r0 = r0 * r1
    CPU_step(0,0,0,0, cpualu_ptr); // print r0
    printf("expected: 40\n");

    // test 2
    printf("\nCheckpoint 3 test 2:\n");
    CPU_step(1,1,0,7, cpualu_ptr); // put a 7 into r1
    CPU_step(2,2,1,1, cpualu_ptr); // r2 = r1 + r1
    CPU_step(5,0,1,2, cpualu_ptr); // r0 = r1 * r2
    CPU_step(0,0,0,0, cpualu_ptr); // print r0
    printf("expected: 98\n");
    CPU_destroy(cpualu_ptr);

    // TESTS for CPU RAM
    printf("\nTests for CPU RAM:\n");

    // test 1
    printf("Checkpoint 4 output:\n");
    sCPU* cpuram_ptr = CPU_init();
    CPU_step(1,1,0,4, cpuram_ptr); // put a 4 into r1
    CPU_step(1,2,0,200, cpuram_ptr); // put a 200 into r2
    CPU_step(3,1,2,0, cpuram_ptr); // store 4 into ram[200]
    CPU_step(4,0,2,0, cpuram_ptr); // load the 4 from ram[200] into r0
    CPU_step(0,0,0,0, cpuram_ptr); // print r0
    printf("expected: 4\n");

    // test 2
    printf("\nCheckpoint 4 test 2:\n");
    CPU_step(1,1,0,7, cpuram_ptr); // put a 7 into r1
    CPU_step(1,2,0,50, cpuram_ptr); // put a 50 into r2
    CPU_step(3,1,2,0, cpuram_ptr); // store 7 into ram[50]
    CPU_step(4,0,2,0, cpuram_ptr); // load the 7 from ram[50] into r0
    CPU_step(0,0,0,0, cpuram_ptr); // print r0
    printf("expected: 7\n");

    CPU_destroy(cpuram_ptr);


    // TESTS for CPU ALU
    printf("\n7. Executing Machine Instructions:\n");

    // 7a
    printf("\n7a: 1 + 3 + 5 + 7 + 9\n");
    sCPU* cpu_ptr7a = CPU_init();
    CPU_step(1,1,0,1, cpu_ptr7a); // put 1 in r1
    CPU_step(1,2,0,3, cpu_ptr7a); // put 3 in r2
    CPU_step(2,0,1,2, cpu_ptr7a); // r0 = r1 + r2
    CPU_step(1,1,0,5, cpu_ptr7a); // put 5 in r1
    CPU_step(2,0,0,1, cpu_ptr7a); // r0 = r0 + r1
    CPU_step(1,1,0,7, cpu_ptr7a); // put 7 in r1
    CPU_step(2,0,0,1, cpu_ptr7a); // r0 = r0 + r1
    CPU_step(1,1,0,9, cpu_ptr7a); // put 9 in r1
    CPU_step(2,0,0,1, cpu_ptr7a); // r0 = r0 + r1
    CPU_step(0,0,0,0, cpu_ptr7a); // print r0
    CPU_destroy(cpu_ptr7a);
    printf("expected: 25\n");

    // 7b
    printf("\n7b: x++\n");
    sCPU* cpu_ptr7b = CPU_init();
    CPU_step(1,1,0,1,cpu_ptr7b); // put 1 in r1
    CPU_step(1,2,0,20,cpu_ptr7b); // put 20 in r2
    CPU_step(4,2,2,0, cpu_ptr7b); // r2 = value in 20th place in RAM
    CPU_step(2,0,1,2,cpu_ptr7b); // r0 = r1 + r2
    CPU_step(0,0,0,0,cpu_ptr7b);
    CPU_destroy(cpu_ptr7b);
    printf("expected: 1\n");

    // 7c
    printf("\n7c: array of first 5 powers of 2\n");
    sCPU* cpu_ptr7c = CPU_init();
    CPU_step(1,3,0,2, cpu_ptr7c); //put 2 in r3
    CPU_step(1,1,0,2, cpu_ptr7c); //put 2 in r1
    CPU_step(1,2,0,50,cpu_ptr7c); //put 50 in r2
    CPU_step(3,1,2,0, cpu_ptr7c); //put r1 in RAM[r2]
    CPU_step(4,0,2,0, cpu_ptr7c); // put ram[r2] in r0
    CPU_step(0,0,0,0, cpu_ptr7c); // print r0
    CPU_step(5,1,3,1, cpu_ptr7c); // r1 = r3*r1
    CPU_step(1,2,0,51,cpu_ptr7c); //put 51 in r2
    CPU_step(3,1,2,0, cpu_ptr7c); //put r1 in RAM[r2]
    CPU_step(4,0,2,0, cpu_ptr7c); // put ram[r2] in r0
    CPU_step(0,0,0,0, cpu_ptr7c); // print r0
    CPU_step(5,1,3,1, cpu_ptr7c); // r1 = r3*r1
    CPU_step(1,2,0,52,cpu_ptr7c); //put 52 in r2
    CPU_step(3,1,2,0, cpu_ptr7c); //put r1 in RAM[r2]
    CPU_step(4,0,2,0, cpu_ptr7c); // put ram[r2] in r0
    CPU_step(0,0,0,0, cpu_ptr7c); // print r0
    CPU_step(5,1,3,1, cpu_ptr7c); // r1 = r3*r1
    CPU_step(1,2,0,53,cpu_ptr7c); //put 53 in r2
    CPU_step(3,1,2,0, cpu_ptr7c); //put r1 in RAM[r2]
    CPU_step(4,0,2,0, cpu_ptr7c); // put ram[r2] in r0
    CPU_step(0,0,0,0, cpu_ptr7c); // print r0
    CPU_step(5,1,3,1, cpu_ptr7c); // r1 = r3*r1
    CPU_step(1,2,0,54,cpu_ptr7c); //put 54 in r2
    CPU_step(3,1,2,0, cpu_ptr7c); //put r1 in RAM[r2]
    CPU_step(4,0,2,0, cpu_ptr7c); // put ram[r2] in r0
    CPU_step(0,0,0,0, cpu_ptr7c); // print r0
    CPU_destroy(cpu_ptr7c);
    printf("expected: 2, 4, 8, 16, 32\n");

    return 0;
}