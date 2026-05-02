# Big Integer
Space-efficient signed integer class that supports an arbitrary number of digits.

Each base-10 digit is stored with 4 bits instead of using the 8-bit `char` type, halving the number of digits required to store a number.

 A `std::vector<uint32_t>` stores the digits. The contiguously-stored data minimizes overhead from linked lists, such as `std::list`. The `uint32_t` storage units minimize costly extensions while keeping overall space at a minimum.

<details>
<summary>Why not use a base greater than 10?</summary>

Using a base larger than 10 allows for storing higher values more compactly. I decided against using higher bases due to base-10 conversion issues and the related inability to print the number.

A key requirement of a big integer class is to convert to and from standard integer types, such as `int64_t`.

 The problem with base>10 is that large bases prevent conversion to a base-10 integer. When converting base-N to base-10, the least significant digit has a base-10 value of N, the second least significant digit has a value of N^2, the third has value N^3, and so on.
 
 Given arbitrarily many digits, the base-10 value of one of the converted digits will exceed the limits of any integer type, making the number impossible to convert to base-10. Printing the number to the standard output requires that the number be in a base-10 representation, so any base larger than 10 will make the integer type useless.

</details>

<br>
Supports addition, subtraction, multiplication, division, prefix increments, and prefix decrements.

<br>
Completed May 1, 2026

### Run Instructions
Compile and run `tests.cpp`, containing test functions for the Big Integer class.

**IMPORTANT:** Compilation must be for the C++20 standard or later. The Big Integer class uses the "spaceship" multi-comparison operator, which requires C++20. *Compiling without C++20 will give very nasty error messages.*