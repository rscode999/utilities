#include "big_integer.hpp"

using namespace std;
using namespace utils;


/**
 * Tests `big_integer(const std::string&)`.
 */
void test_string_constructor() {
    big_integer i;

    //Zero
    i = big_integer("0");
    cout << i << endl; //0
    cout << i.is_negative() << "\n" << endl; //0

    //Single digit
    i = big_integer("9");
    cout << i << endl; //9
    cout << i.is_negative() << "\n" << endl; //0

    //Single negative digit
    i = big_integer("-9");
    cout << i << endl; //-9
    cout << i.is_negative() << "\n" << endl; //1

    //Negative zero (SHOULD BE TREATED AS +0)
    i = big_integer("-0");
    cout << i << endl; //0 (NOT -0)
    cout << i.is_negative() << "\n" << endl; //0

    //Many digits
    i = big_integer("123");
    cout << i << endl; //123
    cout << i.is_negative() << "\n" << endl; //0

    //Many negative digits
    i = big_integer("-45678");
    cout << i << endl; //-45678
    cout << i.is_negative() << "\n" << endl; //1

    //Enough digits to require a new int32_t storage
    i = big_integer("1234567890");
    cout << i << endl; //1234567890
    cout << i.is_negative() << "\n" << endl; //0

    //Negative, enough digits to require a new int32_t storage
    i = big_integer("-1234567890");
    cout << i << endl; //-1234567890
    cout << i.is_negative() << "\n" << endl; //1
}



/**
 * Tests `big_integer(int64_t)`.
 */
void test_int_constructor() {
    big_integer i;

    //Zero
    i = big_integer(0);
    cout << i << endl; //0
    cout << i.is_negative() << "\n" << endl; //0

    //Single digit
    i = big_integer(9);
    cout << i << endl; //9
    cout << i.is_negative() << "\n" << endl; //0

    //Single negative digit
    i = big_integer(-9);
    cout << i << endl; //-9
    cout << i.is_negative() << "\n" << endl; //1

    //Many digits
    i = big_integer(123);
    cout << i << endl; //123
    cout << i.is_negative() << "\n" << endl; //0

    //Many negative digits
    i = big_integer(-123456);
    cout << i << endl; //-123456
    cout << i.is_negative() << "\n" << endl; //1

    //64-bit integer limit
    i = big_integer(9223372036854775807);
    cout << i << endl; //9223372036854775807
    cout << i.is_negative() << "\n" << endl; //0
}



/**
 * Tests the addition operator on positive addends only.
 */
void test_add() {
    big_integer i;
    big_integer j;

    //Single-digit add
    i = big_integer(2);
    j = big_integer(3);
    cout << (i + j) << endl; //5
    cout << (i + j).digits() << "\n" << endl; //1

    //Many-digit add
    i = big_integer(23);
    j = big_integer(45);
    cout << (i + j) << "\n" << endl; //69 (nice)

    //Many-digit add
    i = big_integer(23);
    j = big_integer(45);
    cout << (i + j) << "\n" << endl; //69 (nice)

    //i has more digits than j
    i = big_integer(23);
    j = big_integer(5);
    cout << (i + j) << endl; //28
    cout << (i + j).digits() << "\n" << endl; //2

    //j has more digits than i
    i = big_integer(5);
    j = big_integer(23);
    cout << (i + j) << "\n" << endl; //28
}



/**
 * Tests the addition operator, but where digits have to be carried. For positive addends only.
 */
void test_add_carry() {
    big_integer i;
    big_integer j;

    //Carrying
    i = big_integer(25);
    j = big_integer(39);
    cout << (i + j) << endl; //64
    cout << (i + j).digits() << "\n" << endl; //2

    //Carrying but i has more digits than j
    i = big_integer(25);
    j = big_integer(9);
    cout << (i + j) << "\n" << endl; //34

    //Carrying but j has more digits than i
    i = big_integer(9);
    j = big_integer(25);
    cout << (i + j) << endl; //34
    cout << (i + j).digits() << "\n" << endl; //2

    //Carrying into a new digit
    i = big_integer(75);
    j = big_integer(25);
    cout << (i + j) << endl; //100
    cout << (i + j).digits() << "\n" << endl; //3

    //Carrying into a new digit that requires extra space
    i = big_integer(55555555);
    j = big_integer(99999999);
    cout << (i + j) << endl; //155,555,554
    cout << (i + j).digits() << "\n" << endl; //9
}



/**
 * Tests subtraction without borrowing, where the difference is non-negative.
 * Both operands are positive.
 */
void test_subtract() {
    big_integer i;
    big_integer j;

    //Single digit
    i = big_integer(4);
    j = big_integer(3);
    cout << (i - j) << endl; //1
    cout << (i - j).digits() << "\n" << endl; //1

    //Many digits
    i = big_integer(6544);
    j = big_integer(3211);
    cout << (i - j) << endl; //3333
    cout << (i - j).digits() << "\n" << endl; //4

    //Minuend has more digits
    i = big_integer(12345);
    j = big_integer(1);
    cout << (i - j) << endl; //12344
    cout << (i - j).digits() << "\n" << endl; //5

    //Difference has less digits
    i = big_integer(12345);
    j = big_integer(12332);
    cout << (i - j) << endl; //13 (NOT 00013)
    cout << (i - j).digits() << "\n" << endl; //2

    //Difference has 1 digit
    i = big_integer(12345);
    j = big_integer(12342);
    cout << (i - j) << endl; //3 (NOT 00003)
    cout << (i - j).digits() << "\n" << endl; //1

    //Difference equals 0
    i = big_integer(12345);
    j = big_integer(12345);
    cout << (i - j) << endl; //0
    cout << (i - j).digits() << "\n" << endl; //1

    //Difference requires extra space
    i = big_integer(123456789123);
    j = big_integer(111111111111); 
    cout << (i - j) << endl; //12345678012
    cout << (i - j).digits() << "\n" << endl; //11
}



/**
 * Tests subtraction with borrowing, where the difference is non-negative.
 * Both operands are positive.
 */
void test_subtract_borrow() {
    big_integer i;
    big_integer j;
    big_integer result;

    //Single borrow
    i = big_integer(6789);
    j = big_integer(5690);
    result = i - j;
    cout << result << endl; //1099
    cout << result.digits() << "\n" << endl; //4

    //Borrowing across many digits
    i = big_integer(6308);
    j = big_integer(5209);
    result = i - j;
    cout << result << endl; //1099
    cout << result.digits() << "\n" << endl; //4

    //Borrowing with fewer digits in the subtrahend
    i = big_integer(6005);
    j = big_integer(27);
    result = i - j;
    cout << result << endl; //5978
    cout << result.digits() << "\n" << endl; //4

    //Borrowing where the difference has fewer digits
    i = big_integer(6107);
    j = big_integer(6009);
    result = i - j;
    cout << result << endl; //98
    cout << result.digits() << "\n" << endl; //2

    //Borrowing across multiple storage units
    i = big_integer(1234567890123);
    j = big_integer(1111111111111);
    result = i - j;
    cout << result << endl; //123,456,779,012
    cout << result.digits() << "\n" << endl; //12

    //Borrowing where the difference requires fewer storage units
    i = big_integer(456456456456);
    j = big_integer(456456279153);
    result = i - j;
    cout << result << endl; //177,303
    cout << result.digits() << "\n" << endl; //6
}



/**
 * Tests the increment operator
 */
void test_increment() {
    big_integer i;
    ++i;
    cout << i << "\n" << endl; //1

    i = big_integer(19);
    ++i;
    cout << i << "\n" << endl; //20

    i = big_integer(399);
    ++i;
    cout << i << "\n" << endl; //400

    i = big_integer(99);
    ++i;
    cout << i << "\n" << endl; //100

    i = big_integer(9);
    ++i;
    cout << i << "\n" << endl; //10

    i = big_integer(99999999);
    ++i;
    cout << i << "\n" << endl; //100000000

    i = big_integer(-2);
    ++i;
    cout << i << "\n" << endl; //-1

    i = big_integer(-30);
    ++i;
    cout << i << "\n" << endl; //-29

    i = big_integer(-100);
    ++i;
    cout << i << "\n" << endl; //-99

    i = big_integer(-10);
    ++i;
    cout << i << "\n" << endl; //-9

    i = big_integer(-1);
    ++i;
    cout << i << "\n" << endl; //0
}



/**
 * Tests the decrement operator
 */
void test_decrement() {
    big_integer i(1);
    --i;
    cout << i << "\n" << endl; //0

    i = big_integer(10);
    --i;
    cout << i << "\n" << endl; //9

    i = big_integer(22);
    --i;
    cout << i << "\n" << endl; //21

    i = big_integer(60);
    --i;
    cout << i << "\n" << endl; //59

    i = big_integer(-1);
    --i;
    cout << i << "\n" << endl; //-2

    i = big_integer(-99);
    --i;
    cout << i << "\n" << endl; //-100

    i = big_integer(0);
    --i;
    cout << i << "\n" << endl; //-1

    i = big_integer(100000000);
    --i;
    cout << i << "\n" << endl; //99,999,999 (8 nines)
}



/**
 * Tests the multi-comparison (spaceship) operator
 */
void test_compare() {
    big_integer i = big_integer(0);
    big_integer j = big_integer(0);

    //Single-digit comparison: i == j
    cout << (i < j) << endl; //0
    cout << (i > j) << endl; //0
    cout << (i <= j) << "\n" << endl; //1

    //Single-digit comparison: i > j
    i = big_integer(1);
    j = big_integer(0);
    cout << (i < j) << endl; //0
    cout << (i > j) << endl; //1
    cout << (i >= j) << "\n" << endl; //1

    //Many-digit comparison: i < j
    i = big_integer(1);
    j = big_integer(20);
    cout << (i < j) << endl; //1
    cout << (i > j) << endl; //0
    cout << (i >= j) << "\n" << endl; //0

    //Negative < positive even if absolute values are equal
    i = big_integer(-20);
    j = big_integer(20);
    cout << (i < j) << endl; //1
    cout << (i > j) << endl; //0
    cout << (i >= j) << "\n" << endl; //0

    //Positive > negative even if absolute values say otherwise
    i = big_integer(100);
    j = big_integer(-10000);
    cout << (i < j) << endl; //0
    cout << (i > j) << endl; //1
    cout << (i <= j) << "\n" << endl; //0

    //Divider
    cout << "-------------\n" << endl;

    //Both values are negative: i > j, but digit counts differ
    i = big_integer(-20);
    j = big_integer(-1234);
    cout << (i < j) << endl; //0
    cout << (i > j) << endl; //1
    cout << (i >= j) << "\n" << endl; //1

    //Both values are negative: i > j, same digit counts
    i = big_integer(-20);
    j = big_integer(-41);
    cout << (i < j) << endl; //0
    cout << (i > j) << endl; //1
    cout << (i >= j) << "\n" << endl; //1

    //Across several storage units, i < j
    i = big_integer("12345678901234567890");
    j = big_integer("12345678901234567891");
    cout << (i < j) << endl; //1
    cout << (i > j) << endl; //0
    cout << (i >= j) << "\n" << endl; //0
}



/**
 * Tests the equality comparison operator
 */
void test_equals() {
    big_integer i = big_integer(0);
    big_integer j = big_integer(0);
    cout << (i == j) << endl; //1
    cout << (i != j) << "\n" << endl; //0

    i = big_integer(0);
    j = big_integer(1);
    cout << (i == j) << endl; //0
    cout << (i != j) << "\n" << endl; //1

    i = big_integer(-1);
    j = big_integer(1);
    cout << (i == j) << endl; //0
    cout << (i != j) << "\n" << endl; //1

    i = big_integer("12345678901234567890");
    j = big_integer("12345678901234567890");
    cout << (i == j) << endl; //1
    cout << (i != j) << "\n" << endl; //0

    i = big_integer("123456789");
    j = big_integer("1234567890");
    cout << (i == j) << endl; //0
    cout << (i != j) << "\n" << endl; //1
}



/**
 * Tests addition with negative numbers
 */
void test_add_negative() {
    big_integer i = big_integer(10);
    big_integer j = big_integer(-1);
    big_integer result;

    //Positive + negative: same as subtraction
    result = i + j;
    cout << result << endl; //9
    cout << result.digits() << "\n" << endl; //1

    //Positive + negative (order switched): same as subtraction
    result = j + i;
    cout << result << endl; //9
    cout << result.digits() << "\n" << endl; //1

    //Negative + negative to create negative sum
    i = big_integer(-9);
    j = big_integer(-100);
    result = i + j;
    cout << result << endl; //-109
    cout << result.digits() << "\n" << endl; //3

    //Positive + negative (larger) to create negative sum
    i = big_integer(9);
    j = big_integer(-100);
    result = i + j;
    cout << result << endl; //-91
    cout << result.digits() << "\n" << endl; //2

    //Switch the order
    i = big_integer(-100);
    j = big_integer(9);
    result = i + j;
    cout << result << endl; //-91
    cout << result.digits() << "\n" << endl; //2
}



/**
 * Tests subtraction involving negative numbers
 */
void test_subtract_negative() {
    big_integer i = big_integer(10);
    big_integer j = big_integer(-1);
    big_integer result;

    result = i - j;
    cout << result << endl; //11
    cout << result.digits() << "\n" << endl; //2

    i = big_integer(-99);
    j = big_integer(9);
    result = i - j;
    cout << result << endl; //-108
    cout << result.digits() << "\n" << endl; //3

    i = big_integer(-99);
    j = big_integer(-10000);
    result = i - j;
    cout << result << endl; //9901
    cout << result.digits() << "\n" << endl; //4

    i = big_integer(100);
    j = big_integer(10000);
    result = i - j;
    cout << result << endl; //-9900
    cout << result.digits() << "\n" << endl; //4
}



/**
 * Tests the multiplication operator
 */
void test_multiply() {
    big_integer i;
    big_integer j;
    big_integer result;

    //Positive * positive
    i = big_integer(2);
    j = big_integer(9);
    result = i * j;
    cout << result << endl; //18
    cout << result.digits() << "\n" << endl; //2

    //Positive * negative
    i = big_integer(18);
    j = big_integer(-11);
    result = i * j;
    cout << result << endl; //-198
    cout << result.digits() << "\n" << endl; //3

    //Negative * positive
    i = big_integer(-156);
    j = big_integer(213);
    result = i * j;
    cout << result << endl; //-33228
    cout << result.digits() << "\n" << endl; //3

    //Multiply by 1
    i = big_integer(1);
    j = big_integer(12);
    result = i * j;
    cout << result << endl; //12
    cout << result.digits() << "\n" << endl; //2

    //Positive * 0 = 0
    i = big_integer(14);
    j = big_integer(0);
    result = i * j;
    cout << result << endl; //0
    cout << result.digits() << "\n" << endl; //1

    //Negative * 0 = 0 (NOT -0)
    i = big_integer(0);
    j = big_integer(-23);
    result = i * j;
    cout << result << endl; //0 (POSITIVE!)
    cout << result.digits() << "\n" << endl; //1
}



/**
 * Tests the divide operator
 */
void test_divide() {
    big_integer i;
    big_integer j;
    big_integer result;

    //Positive / positive
    i = big_integer(6);
    j = big_integer(3);
    result = i / j;
    cout << result << endl; //2
    cout << result.digits() << "\n" << endl; //1

    //Not evenly divisible
    i = big_integer(7);
    j = big_integer(3);
    result = i / j;
    cout << result << endl; //2
    cout << result.digits() << "\n" << endl; //1

    //Negative / positive
    i = big_integer(-228);
    j = big_integer(19);
    result = i / j;
    cout << result << endl; //-12
    cout << result.digits() << "\n" << endl; //2

    //Positive / negative (and not evenly divisible)
    i = big_integer(1550);
    j = big_integer(-4);
    result = i / j;
    cout << result << endl; //388
    cout << result.digits() << "\n" << endl; //3

    //0 / negative
    i = big_integer(0);
    j = big_integer(-4);
    result = i / j;
    cout << result << endl; //0 (NOT -0)
    cout << result.digits() << "\n" << endl; //1

    //Something / 0 should throw std::out_of_range
    i = big_integer(15);
    j = big_integer(0);
    try {
        result = i / j;
        throw std::runtime_error("No exception thrown upon division by zero");
    }
    catch(std::out_of_range& e) {
        cout << "Division by zero successfully handled" << endl;
    }
}


int main() {
    // test_string_constructor();
    // test_int_constructor();
    // test_add();
    // test_add_carry();
    // test_subtract();
    // test_subtract_borrow();
    // test_increment();
    // test_decrement();
    // test_compare();
    // test_equals();
    // test_add_negative();
    // test_subtract_negative();
    // test_multiply();
    // test_divide();
}