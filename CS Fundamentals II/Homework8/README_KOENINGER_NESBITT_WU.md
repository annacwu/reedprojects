## HW9 

To compile this code, run the following command:

```g++ manager.cpp manager_test.cpp -o test -std=c++11``` 

### 1. sManager() -- constructor
- **WHERE** manager.cpp file. 
- **WHAT** We assigned the arguments to the correct members of each sManager and checked if debug was on to determine the behavior of the memory.

### 2. sManager() -- destructor
- **WHERE** manager.cpp file. 
- **WHAT** Prints that the destructor was called.

### 3. init_headers()
- **WHERE** manager.cpp file. 
- **WHAT** We use for loops to initialize all the header spaces in memory as 'F' for free.

### 4. print()
- **WHERE** manager.cpp file.
- **WHAT** Uses a for loop to print out each index of the memory array, breaking to the next line every 80 characters.

### 5. ff_tiny and ff_big()
- **WHERE** manager.cpp file.
- **WHAT** Uses a for loop to go through the sManager memory and check if the memory is free. If it is, it returns the index of the first free memory chunk.  

### 6. salloc()
- **WHERE** manager.cpp file.
- **WHAT** calls ff_tiny() and ff_big() depending on the argument indicating the size of the memory to be allocated. Using pointer arithmetic, it returns a pointer to the place in memory that is usable (past the header).

### 7. sfree()
- **WHERE** manager.cpp file.
- **WHAT** Using pointer arithmetic and dereferencing, the function finds the header of the in-use memory chunk and changes it to 'F' for free. 

### 8. sUniquePtr() - constructor
- **WHERE** manager.cpp file. 
- **WHAT** We assigned the arguments to the correct members of each sUniquePtr.

### 8. sUniquePtr() - destructor
- **WHERE** manager.cpp file. 
- **WHAT** Calls sfree on the pointer to ensure the memory deallocates when it goes out of scope (as a unique pointer is meant to do). Also prints that it has been free'd.

### 10. get()
- **WHERE** manager.cpp file. 
- **WHAT** returns the pointer member of an sUniquePtr.

### 11. print_list()
- **WHERE** manager.cpp file. 
- **WHAT** calls STL list functions begin() and end() to use the iterator to go through the list with a for loop and print each value by dereferencing the iterator.

### 12. replace_specific()
- **WHERE** manager.cpp file. 
- **WHAT** uses the input start and end pointers of a list of ints to use a while loop and dereferencing of the list's iterator to change the argument old_value to the argument new_value.

### 13. replace_generic()
- **WHERE** manager.hpp file. 
- **WHAT** with a template intitializing a class myIter for the iterator and class T for the constant values of the argument, the function uses the input start and end pointers of a generic list to use a while loop and dereferencing of the list's iterator to change the argument old_value to the argument new_value.

### 14. is_unlucky()
- **WHERE** manager.cpp file. 
- **WHAT** uses the find() function from the STL to determine if 13 is in the set. It conditionally returns true or false depending on the outcome.

### 15. zero_evens()
- **WHERE** manager.cpp file and manager.hpp file. 
- **WHAT** With the predicate class in the header file, it uses the operator() function to determine if a number is even or not. Then, depending on the outcome, it uses STL functions begin() and end() withint replace_if() in the .cpp file to carry out the algorithm.

### 16. zero_evens_after_three()
- **WHERE** manager.cpp file and manager.hpp file. 
- **WHAT** With the predicate class in the header file, it uses the operator() function to determine if a number is even or not and if the algorithm has already gone through three even numbers. Then, depending on the outcome, it uses STL functions begin() and end() withint replace_if() in the .cpp file to carry out the algorithm.