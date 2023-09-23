## Homework 7
To compile this code, run the following command: 
```g++ Homework7.cpp HW7test.cpp -o test```

### 1a. vec struct
- **WHERE**: code for the vec struct is in Homework7.cpp and Homework7.hpp.
- **HOW**: it holds three members, int* arr to represent the array of the vector, int length to hold the length, and int capacity to hold the capacity. 
- **Notes**: appears to be functioning properly.

### 1b. vec_init() 
- **WHERE**: code for vec_init() is in Homework7.cpp and Homework7.hpp.
- **HOW**: run vec_init() by initializing a new vec pointer variable. For example, vec* myevc = vec_init() will create a new dynamically allocated empty vec. 
- **Notes**: appears to be functioning properly.

### 1c. vec_destroy()
- **WHERE**: code for vec_destroy() is in Homework7.cpp and Homework7.hpp.
- **HOW**: run vec_destroy() by inputting a pointer to a vec variable. For example, vec_destroy(myvec) will delete all of the dynamically allocated memory associated with myvec.
- **Notes**: appears to be functioning properly.

### 1d. vec_print() and vec_dprint()
- **WHERE**: code for vec_print() and vec_dprint() is in Homework7.cpp and Homework7.hpp.
- **HOW**: run vec_print() and vec_dprint() by inputting a pointer to a vec variable. For example, vec_print(myvec) will print out a formatted version of all of the variables in the array associated with myvec. vec_dprint() will print the length and capacity as well.
- **Notes**: appears to be functioning properly.

### 1e. vec_reallocate()
- **WHERE**: code for vec_reallocate() is in Homework7.cpp and Homework7.hpp.
- **HOW**: run vec_reallocate() by inputting a pointer to a vec variable. There should be no reason to run it, as vec_append() calls the function when necessary to increse the vector capacity. 
- **Notes**: appears to be functioning properly.

### 1f. vec_append()
- **WHERE**: code for vec_append() is in Homework7.cpp and Homework7.hpp.
- **HOW**: run vec_append() by inputting a pointer to a vec variable and the value to append to it. For instance, vec_append(myvec, 7) adds 7 to the myvec array. 
- **Notes**: appears to be functioning properly.

### 2a. pt struct
- **WHERE**: code for the pt struct is in Homework7.cpp and Homework7.hpp.
- **HOW**: it holds two members, double x and double y to represent coordinates. 
- **Notes**: appears to be functioning properly.

### 2b. dist()
- **WHERE**: code for dist() is in Homework7.cpp and Homework7.hpp.
- **HOW**: run dist() by inputting two pts. For instance, inputting pt a, pt b, which have their individual coordinates will return the distance between those two coordinates using the distance function. 
- **Notes**: appears to be functioning properly.

### 2c. make_grid()
- **WHERE**: code for make_grid() is in Homework7.cpp and Homework7.hpp.
- **HOW**: run make_grid() by inputting an int n that dictates the size of the grid. For instance, make_grid(4) will make a 4 x 4 grid of pts evely spaced along the grid.
- **Notes**: appears to be functioning properly. It only increments the distance between points by +1, so there is possibly a way to make it more precise by doing smaller increments, but this is sufficiently accurate. 

### 2. calcpi()
- **WHERE**: code for calcpi() is in Homework7.cpp and Homework7.hpp.
- **HOW**: run calcpi() by inputting the size of the grid you wish to use to calculate pi. For instance, calcpi(10000) will give you 3.14159.
- **Notes**: appears to be functioning properly. Testing with 10000 takes awhile to run, but eventually gives the right number of pi. It is the only completely accurate test. 

Pi estimates with calcpi(): 
Test 1 -- 10: 3.16
Test 2 -- 100: 3.1372
Test 3 -- 1000: 3.14139
Test 4 -- 10000: 3.14159