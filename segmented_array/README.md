# Segmented Array
Space-efficient array that divides its data into segments

Valid only for the types `char` and `wchar_t`. Any other type will cause an assertion error upon creation.

A segmented array with limited functionality was used in some cryptographic processes. 

Last updated April 19, 2025  
Updated with Rule of 3 methods January 24, 2026

### Run Instructions

#### Normal Run
Compile `test_sarray.cpp` ("Test Segmented Array") or `test_wchar_sarray` ("Test Wide-Character Segmented Array") for the C++11 standard.

`test_sarray.cpp` uses a Read-Eval-Print loop to allow users to interact with a segmented array.  
Users enter commands into the standard input. Each command involves one piece of functionality of a segmented array.  

Commands:
- q:  quit (exit the program)
- a:  append (calls `push_back`)
- e:  extend capacity
- r:  remove (calls `pop_back`)
- ra: remove all
- rt: remove and trim (calls `pop_back`)
- s:  set character value
- si: set integer value
- t:  trim (calls `shrink_to_fit`)  

The original function names resembled Java conventions. The names were later changed to reflect C++ naming patterns.

#### Operator Overloads
To test operator overloads, compile `test_overloads.cpp` for the C++11 standard.  

Commands:  
- q:  quit
- a:  append
- e:  extend capacity
- r:  remove (calls `pop_back`)
- s:  shrink to fit contents


### Deprecated Files
The `old` directory contains non-parameterized implementations of the segmented array.