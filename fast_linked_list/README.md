# Fast Linked List
Linked list that can iterate through its contents in linear time using only its "get" function

Saves the last position in the list read or written by the user. Access time scales linearly with the number of elements between the list's start, the list's end, and the last element accessed.

Last updated June 7, 2025  
Updated with Rule of 3 methods January 24, 2026

### Run Instructions
Compile and run `run_fll.cpp` ("Run Fast Linked List") for the C++11 standard.

`run_fll.cpp` uses a Read-Eval-Print loop to take user input and manipulate a Fast Linked List through commands.  
Possible commands:
- q:  quit (exit the program)
- ab: add back
- af: add front
- al: add last accessed
- g:  get. Calls the `at` method
- l:  last accessed
- n:  get number of elements
- rb: remove back 
- rf: remove front
- rl: remove last accessed
- s:  set  

### Test Instructions
Compile and run `test_fll.cpp` ("Test Fast Linked List") for the C++11 standard. 

Any method that begins with a capital letter is a testing function.

Some of the tests are not complete. All testing has previously been done using `run_fll.cpp`, so there is no need to use additional tests.