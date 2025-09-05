#include "Homework7.hpp"
#include <iostream> 
#include <cmath>
#include <vector>

using namespace std;

// returns a pointer to a vec struct with an array of 4 0s
// via pointer syntax and dynamic memory allocation
vec* vec_init(){
    vec* myvec = new vec;
    int* arr = new int[4];
    myvec -> arr = arr;
    myvec -> length = 0;
    myvec -> capacity = 4;
    return myvec;
}

// deallocates a vec from memory
// via delete command
void vec_destroy(vec* vec){
    delete [] vec -> arr;
    delete vec;
}

// prints the array of a vec pointer
// via for loops
void vec_print(vec* vec){
    cout << "[ ";
    for (int i =0; i<vec -> length; i++){
        cout << vec -> arr[i] << " ";
    }
    cout << "]" << endl;
}

// prints all members of a vec 
// via pointer syntax and for loops
void vec_dprint(vec* vec){
    cout << "length: " << vec -> length << endl;
    cout << "capacity: " << vec -> capacity << endl;

    cout << "[ ";
    for (int i =0; i<vec -> capacity; i++){
        cout << vec -> arr[i] << " ";
    }
    cout << "]" << endl;
}

// doubles the length of a vec's array
// via for loops and dynamic memory allocation
void vec_reallocate(vec* vec){
    int* newarr = new int[vec -> capacity * 2];
    for (int i = 0; i < vec -> capacity; i++){
        newarr[i] = vec -> arr[i];
    }
    delete [] vec -> arr;
    vec -> arr = newarr;
    vec -> capacity = vec -> capacity*2;
}

// appends a value to a vec's array
// via conditional statements and pointer syntax
void vec_append(vec* vec, int new_val){
    if (vec -> length != vec -> capacity){
        vec -> arr[vec -> length] = new_val;
        vec -> length += 1;
    } else {
        vec_reallocate(vec);
        vec -> arr[vec -> length] = new_val;
        vec -> length += 1;
    }
}

// returns the euclidean distance between two points
// via arithmetic
double dist(pt a, pt b){
    double x = b.x - a.x;
    x = x * x;
    double y = b.y - a.y;
    y = y * y;
    return sqrt(x + y);
}

// returns a vector of vectors representing a coordinate grid
// via nested for loops and vector commands
vector< vector <pt> > make_grid(int n){
    vector< vector<pt> > vec;
    for (int i = 0; i < n; i++){
        vector<pt> row;
        for (int j=0;j<n; j++){
            pt a;
            a.x = i;
            a.y = j;
            row.push_back(a);
            }
        vec.push_back(row);
    }
    return vec;
}

// returns an approximate value of pi
// via calculation through the coordinate grid with for loops and arithmetic
double calcpi (int n){
    vector<vector <pt> > grid = make_grid(n);
    
    pt center;
    center.x = n * 0.5;
    center.y = n * 0.5;

    double numcirc = 0;
    double total = n*n;

    for (int i = 0;i<n; i++){
        for (int j = 0;j<n; j++){
        if(dist(grid[j][i], center) <= n/2){
            numcirc++;
        }
    }
    }

    double step = numcirc/total;
    
    return step * 4;
}