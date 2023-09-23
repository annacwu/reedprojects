#include "Homework7.hpp"
#include <iostream> 
#include <vector>

using namespace std;

int main(){

    // TESTS for vec struct
    cout << "Tests for vec:\n" << endl;

    // test 1 -- 1, 3, 5, 7
    vec* vecc = vec_init();
    vec_append(vecc,1);
    vec_append(vecc,3);
    vec_append(vecc,5);
    vec_append(vecc,7);
    cout << "Test 1 -- 1, 3, 5, 7:" << endl;
    vec_print(vecc);
    cout << "expected: [ 1 3 5 7 ]\n" << endl;
    vec_destroy(vecc);

    // test 2 -- 7, 9, 11, 13, 15
    vec* myvec = vec_init();
    vec_append(myvec,7);
    vec_append(myvec,9);
    vec_append(myvec,11);
    vec_append(myvec,13);
    vec_append(myvec,15);
    cout << "Test 2 -- 7, 9, 11, 13, 15:" << endl;
    vec_print(myvec);
    cout << "expected: [ 7 9 11 13 15 ]\n" << endl;
    vec_destroy(myvec);

    // test 3 -- 1, 2, 3, 4, 5, 6, 7, 8, 9
    vec* veccc = vec_init();
    vec_append(veccc,1);
    vec_append(veccc,2);
    vec_append(veccc,3);
    vec_append(veccc,4);
    vec_append(veccc,5);
    vec_append(veccc,6);
    vec_append(veccc,7);
    vec_append(veccc,8);
    vec_append(veccc,9);
    cout << "Test 3 -- 1, 2, 3, 4, 5, 6, 7, 8, 9:" << endl;
    vec_print(veccc);
    cout << "expected: [ 1 2 3 4 5 6 7 8 9 ]\n" << endl;

    // test 3 with dprint
    cout << "Test 3 with vec_dprint():" << endl;
    vec_dprint(veccc);
    cout << "expected: [ 1 2 3 4 5 6 7 8 9 0 0 0 0 0 0 0 ], length: 9, capacity: 16 \n" << endl;
    vec_destroy(veccc);

    // TESTS for dist()
    cout << "Tests for dist():" << endl;

    // test 1 -- (1,2), (4,5)
    pt a;
    a.x = 1;
    a.y = 2;
    pt b;
    b.x = 4;
    b.y = 5;
    double distance = dist(a, b);
    cout << "Test 1 -- (1,2), (4,5): " << distance << "\t expected: 4.24264" << endl;

    // test 2 -- (0,0), (1,0)
    pt c;
    c.x = 0;
    c.y = 0;
    pt d;
    d.x = 1;
    d.y = 0;
    double distancey = dist(c, d);
    cout << "Test 2 -- (0,0), (1,0): " << distancey << "\t expected: 1" << endl;

    // TESTS for make_grid() and calcpi():
    cout << "\nTests for make_grid() and calcpi():" << endl;

    // test 1: 10
    cout << "Test 1 -- 10: " << calcpi(10) << endl;

    // test 2: 100
    cout << "Test 2 -- 100: " << calcpi(100) << endl;

    // test 3: 1000
    cout << "Test 3 -- 1000: " << calcpi(1000) << endl;

    // test 4: 10000
    cout << "Test 4 -- 10000: " << calcpi(10000) << endl;

    return 0;
}