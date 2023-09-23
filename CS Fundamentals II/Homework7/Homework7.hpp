#ifndef HOMEWORK7_HPP
#define HOMEWORK7_HPP

#include <vector>

using namespace std;

// struct vec represents a vec
// arr is an int* array of integers
// length is an integer that represents the currently length of the array
// capacity is an integer that represents the maximum size of the array
struct vec{
    int* arr;
    int length;
    int capacity;
};

// vec_init() takes no arguments
// and returns a pointer to a vec
// no arguments
// result: 
// pointer to an empty vec
vec* vec_init();

// vec_destroy() takes a pointer to a vec
// and deallocates the memory
// arguments: 
// vec* vec -- vec to be deallocated
// result: 
// vec deleted from the heap
void vec_destroy(vec*);

// vec_print() takes a pointer to a vec
// and prints a formatted version of the array
// arguments: 
// vec* vec -- vec to be printed
// result: 
// [0 0 0] format printed to console
void vec_print(vec*);

// vec_dprint() takes a pointer to a vec
// and prints all of the members of the struct
// arguments: 
// vec* vec -- vec to be printed
// result: 
// vec arr, length, and capacity printed to console
void vec_dprint(vec*);

// vec_reallocate() takes a pointer to a vec
// and doubles its capacity, copying over all its values 
// arguments:
// vec* vec -- vec to be reallocated
// result: 
// new array pointed to in vec with same values and double capacity
void vec_reallocate(vec*);

// vec_append() takes a pointer to a vec and an integer
// and adds a value to the vec array
// arguments: 
// vec* vec -- vec to be appended to
// int new_val -- value to be appended
// result: 
// integer added to the array 
void vec_append(vec*, int);

// struct pt represents a coordinate point
// double x is a point's x value
// double y is a point's y value
struct pt{
   double x;
   double y; 
};

// dist() takes two pts
// and returns the euclidean distance between them
// arguments: 
// pt a -- first coordinate point
// pt b -- second coordinate point
// result: 
// double representing the distance between a and b
double dist(pt, pt);

// make_grid() takes an integer
// and returns a vector of vectors representing a grid of pts
// arguments:
// int n -- size of the grid to be returned
// result: 
// n sized grid of pts 
vector< vector <pt> > make_grid(int);

// calcpi() takes an integer
// and returns an approximate value of pi calculated from a grid
// arguments: 
// int n -- size of grid to be used to calculate
// result: 
// approximate value of pi
double calcpi (int);

#endif