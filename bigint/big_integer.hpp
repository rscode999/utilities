#pragma once

#include <cassert>
#include <compare>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>


namespace utils {




/**
 * Number of 4-bit base-10 digits that can be stored in one storage unit
 */
#define DIGITS_PER_STORAGE 8


/**
 * Integer capable of storing arbitrarily many digits.
 * 
 * The postfix increment and decrement operators are not supported.
 * Use `++i` and `--i` to increment or decrement the integer.
 * 
 * ORDER OF OPERATIONS IS NOT SUPPORTED!
 * 
 * Requires compilation with C++20 or later.
 */
class big_integer {
private:

    /**
     * Contains the numerical value of the Big Integer
     */
    std::vector<uint32_t> contents;

    /**
     * Number of digits that this Big Integer contains
     */
    int32_t n_digits;

    /**
     * True if the number is negative. False if the number is positive or zero.
     */
    bool negative;


    /**
     * Returns the value of the 4-bit integer in `digit_storage` at position `digit_storage_index`.
     * 
     * Use only as a helper to `digit_at`.
     * 
     * Digits in `digit_storage` are represented as eight 4-bit values. The least significant 4-bit value
     * in `digit_storage` is at position 0.
     * 
     * @param digit_storage_index position in `digit_storage` to get the value. Must be on the interval [0, `DIGITS_PER_STORAGE` - 1]
     * @param digit_storage number to extract a value from
     * @return 4-bit integer value at the specified position
     */
    int storage_value_at(int32_t digit_storage_index, uint32_t digit_storage) const {
        assert(0 <= digit_storage_index && digit_storage_index < DIGITS_PER_STORAGE);

        int output = 0;
        int digit_index = 0;

        for(int i = 4 * digit_storage_index; i < 4 * digit_storage_index + 4; i++) {
            //Get each bit from the base digit, multiplied by its 4-bit place value
            output += ((digit_storage >> i) & 1) * pow(2, digit_index);
            digit_index++;
        }
        return output;
    }


    /**
     * Sets the 4-bit integer at position `digit_storage_index` in `digit_storage` to `new_value`.
     * 
     * Use only as a helper to `set_digit_at`.
     * 
     * Mutates `digit_storage`.
     * @param digit_storage_index index to set in `digit_storage`. Must be on the interval [0, `DIGITS_PER_STORAGE` - 1]
     * @param digit_storage 32-bit integer to set value in
     * @param new_value value to set `digit_storage[digit_storage_index]` to. Must be on the interval [0, 9]
     */
    void set_storage_value_at(int32_t digit_storage_index, uint32_t& digit_storage, int32_t new_value) {
        assert(0 <= new_value && new_value <= 9);

        // Zero the existing bits
        digit_storage &= ~(0xF << (4*digit_storage_index));

        // set the new value
        digit_storage |= (new_value & 0xF) << (4*digit_storage_index);
    }


    /**
     * Returns the 4-bit numerical value of the digit at position `index` in the entire contents vector.
     * 
     * The lowest place value is at index 0.
     * 
     * @param index position to get in the contents vector. Must satisfy 0 <= `index` < `n_digits`
     * @return numerical value at `index`
     */
    int32_t digit_at(int32_t index) const {
        assert(0 <= index && index < n_digits && "Digit-at access index out of range");
        return storage_value_at(index % DIGITS_PER_STORAGE, contents[index / DIGITS_PER_STORAGE]);
    }


    /**
     * Sets the 4-bit numerical value of the digit at position `index` (in the entire contents vector) to `new_value`.
     * 
     * The lowest place value is at index 0.
     * 
     * @param index position to set in the contents vector. Must satisfy 0 <= `index` < `n_digits`
     * @param new_value value to set at position `index`. Must satisfy 0 <= `new_value` <= 9
     */
    void set_digit_at(int32_t index, int32_t new_value) {
         assert(0 <= new_value && new_value <= 9 && "New value must be on the interval [0, 9]");

         set_storage_value_at(index % DIGITS_PER_STORAGE, contents[index / DIGITS_PER_STORAGE], new_value);
    }


    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    /**
     * Returns the sum of `lhs_addend` and `rhs_addend`, ignoring negative signs (both addends are treated as positive).
     * @param lhs_addend left-hand side addend
     * @param rhs_addend right-hand side addend
     * @return sum of the given addends
     */
    big_integer _add_positive(const big_integer& lhs_addend, const big_integer& rhs_addend) const;

    /**
     * Decreases the absolute value of `i` by 1, ignoring positive or negative signs.
     * 
     * Mutates `i`.
     * 
     * @param i Big Integer to decrement
     */
    void _decrement_positive(big_integer& i);

    /**
     * Increases the absolute value of `i` by 1, ignoring positive or negative signs.
     * 
     * Mutates `i`.
     * 
     * @param i Big Integer to increment
     */
    void _increment_positive(big_integer& i);

    /**
     * Returns the difference of `minuend` and `subtrahend`, ignoring negative signs.
     * @param minuend number to subtract
     * @param subtrahend number to subtract from `minuend`. Must be no greater than `minuend`.
     * @return `abs(minuend)` minus `abs(subtrahend)`
     */
    big_integer _subtract_positive(big_integer minuend, const big_integer& subtrahend) const;


    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////

public:

    friend class big_integer; //Declare self as friend

    /**
     * Creates a Big Integer with value 0.
     */
    big_integer() {
        contents = {0};
        n_digits = 1;
        negative = false;
    }


    /**
     * Creates a new Big Integer containing `initial_value` interpreted as a string.
     * @param initial_value value to set the new Big Integer to. Must be a valid integer, and all characters must be digits 0-9 or negative signs
     */
    big_integer(const std::string& initial_value) {
        #ifndef NDEBUG
            assert(initial_value.length() > 0 && "Initial value must be non-empty");
            if(initial_value[0] == '-') {
                assert(initial_value.length() > 1 && "Initial value cannot be a negative sign only");
            }
        #endif

        //Special case for -0
        if(initial_value == "-0") {
            contents = {0};
            n_digits = 1;
            negative = false;
            return;
        }
        
        //Check the - sign for negativity
        negative = (initial_value[0] == '-');

        contents = {0};
        n_digits = 0;
        
        //Load the least significant digits first.
        for(int32_t i = (int32_t)initial_value.length() - 1; i >= (initial_value[0]=='-' ? 1 : 0); i--) {
            assert(48 <= initial_value[i] && initial_value[i] <= 57 && "All characters in the initial value (except for the negative sign) must be digits");
            
            //Put least significant digit in the lowest storage index
            set_storage_value_at(n_digits % DIGITS_PER_STORAGE, contents[n_digits / DIGITS_PER_STORAGE], (int)(initial_value[i] - 48));
            
            //Increase number of digits, and add another storage integer if necessary
            n_digits++;
            if(n_digits % 8 == 0 && n_digits > 0) {
                contents.push_back(0);
            }
        }
    }


    /**
     * Creates a new Big Integer containing `initial_value`.
     * @param initial_value value to set the new Big Integer to
     */
    big_integer(int64_t initial_value) : big_integer(std::to_string(initial_value)) {
        //Initial value is converted to a string, then constructed with the string constructor
    }


    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    /**
     * @return number of digits in this Big Integer
     */
    int32_t digits() const;

    /**
     * @return true if the number is positive (or zero), false otherwise
     */
    bool is_negative() const;

    /**
     * Returns the Big Integer converted to a 64-bit integer.
     * @return the Big Integer as a int64_t
     * @throws `std::out_of_range` if the Big Integer's value exceeds the limits of a 64-bit signed integer
     */
    int64_t to_integer() const;

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    /**
     * Returns whether this Big Integer is equal to `other`.
     * @return if `this` == `other`
     */
    constexpr bool operator==(const big_integer& other) const;

        /**
     * Returns whether this Big Integer is not equal to `other`.
     * @return if `this` != `other`
     */
    constexpr bool operator!=(const big_integer& other) const;

    /**
     * Performs a comparison between this Big Integer and `other`, returning the result as a `std::strong_ordering`.
     * @param other Big Integer to compare to
     * @return comparison result (greater than, less than, greater than or equal to, etc.)
     */
    constexpr std::strong_ordering operator<=>(const big_integer& other) const; //the operator is defined

    /**
     * Returns the sum of this Big Integer and `other`.
     * @param other Big Integer to add to this object
     * @return this object + `other`
     */
    big_integer operator+(const big_integer& other) const;

    /**
     * Adds `other` to this Big Integer.
     * @param other Big Integer to add
     */
    void operator+=(const big_integer& other);

    /**
     * `++i` operator: Prefix-increments this Big Integer by 1.
     */
    big_integer& operator++();

    /**
     * Returns the difference of this Big Integer and `other`.
     * @param other Big Integer to subtract from this object
     * @return this object minus `other`
     */
    big_integer operator-(const big_integer& other) const;

    /**
     * Subtracts `other` from this Big Integer.
     * @param other Big Integer to subtract
     */
    void operator-=(const big_integer& other);

    /**
     * `--i` operator: Prefix-decrements this Big Integer by 1.
     */
    big_integer& operator--();

    /**
     * Returns the product of this Big Integer and `other`.
     * @param other Big Integer to multiply this object by
     * @return this object * `other`
     */
    big_integer operator*(const big_integer& other) const;

    /**
     * Multiplies `other` by this Big Integer.
     * @param other Big Integer to multiply
     */
    void operator*=(const big_integer& other);

    /**
     * Returns the quotient of this Big Integer and `other`. Throws `std::out_of_range` if `other` is 0.
     * 
     * Integer division rounds down.
     * 
     * @param other Big Integer to divide this object by
     * @return this object / `other`
     * @throws `std::out_of_range` in case of division by zero
     */
    big_integer operator/(const big_integer& other) const;

    /**
     * Divides `other` by this Big Integer. Throws `std::out_of_range` if `other` is 0.
     * 
     * Integer division rounds down.
     * 
     * @param other Big Integer to divide
     * @throws `std::out_of_range` in case of division by zero
     */
    void operator/=(const big_integer& other);

    /**
     * Exports `i` to the output stream `output_stream`, returning a reference to `output_stream` with `i` inserted in it.
     * @param output_stream stream to insert the integer into
     * @param i Big Integer to export
     */
    template<typename CharT, typename Traits>
    friend std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& output_stream, const big_integer& i);

    friend big_integer abs(big_integer i);
};


/**
 * Returns the absolute value of the Big Integer `i`.
 * @param i Big Integer to take absolute value of
 * @return |`i`|
 */
big_integer abs(big_integer i) {
    i.negative = false;
    return i;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////


big_integer big_integer::_add_positive(const big_integer& lhs_addend, const big_integer& rhs_addend) const {
    big_integer output;

    int32_t digit_index = 0;

    int32_t digit_sum = 0;
    int32_t carry = 0;

    //While both have not yet run out of digits
    while(digit_index < lhs_addend.n_digits && digit_index < rhs_addend.n_digits) {
        //Add digits
        digit_sum = lhs_addend.digit_at(digit_index) + rhs_addend.digit_at(digit_index) + carry;

        //Check for overflow. If so, extract carry
        if(digit_sum > 9) {
            digit_sum -= 10;
            carry = 1;
        }
        else {
            carry = 0;
        }

        //Assign result to output
        output.set_digit_at(digit_index, digit_sum);

        digit_index++;
        //Add more space if necessary
        if(digit_index % DIGITS_PER_STORAGE == 0 && digit_index > 0) {
            output.contents.push_back(0);
        }
    }

    //While left-hand side has not run out of digits
    while(digit_index < lhs_addend.n_digits) {
        //Add digits
        digit_sum = lhs_addend.digit_at(digit_index) + carry;

        //Check for overflow. If so, extract carry
        if(digit_sum > 9) {
            digit_sum -= 10;
            carry = 1;
        }
        else {
            carry = 0;
        }

        //Assign result to output
        output.set_digit_at(digit_index, digit_sum);

        digit_index++;
        //Add more space if necessary
        if(digit_index % DIGITS_PER_STORAGE == 0 && digit_index > 0) {
            output.contents.push_back(0);
        }
    }

    //While right-hand side has not run out of digits
    while(digit_index < rhs_addend.n_digits) {
        //Add digits
        digit_sum = rhs_addend.digit_at(digit_index) + carry;

        //Check for overflow. If so, extract carry
        if(digit_sum > 9) {
            digit_sum -= 10;
            carry = 1;
        }
        else {
            carry = 0;
        }

        //Assign result to output
        output.set_digit_at(digit_index, digit_sum);

        digit_index++;
        //Add more space if necessary
        if(digit_index % DIGITS_PER_STORAGE == 0 && digit_index > 0) {
            output.contents.push_back(0);
        }
    }

    //Check if the carry digit has something. If so, add it
    if(carry > 0) {
        if(digit_index % DIGITS_PER_STORAGE == 0 && digit_index > 0) {
            output.contents.push_back(0);
        }
        output.set_digit_at(digit_index, carry);
        digit_index++;
    }

    output.n_digits = digit_index;
    return output;
}



void big_integer::_decrement_positive(big_integer& i) {
    //Decrement the first digit
    int32_t new_digit = i.digit_at(0) - 1;

    //Bail out early if there is no need to decrement other digits
    if(new_digit >= 0) {
        i.set_digit_at(0, new_digit);
        return;
    }

    //Decrement subsequent digits in case of underflow
    int32_t digit_index = 1;
    while(digit_index < n_digits) {
        i.set_digit_at(digit_index - 1, 9); //set previous digit to 9
        new_digit = i.digit_at(digit_index) - 1; //decrement current digit

        //Don't need to decrease next digit: bail out
        if(new_digit >= 0) {
            //Ran out of digits (i.e. 10000...): Decrease number of digits
            if(digit_index == n_digits-1 && new_digit == 0) {
                if(i.n_digits % DIGITS_PER_STORAGE == 0) {
                    i.contents.pop_back();
                }
                i.n_digits--;
            }

            i.set_digit_at(digit_index, new_digit);
            return;
        }

        digit_index++;
    }
}



void big_integer::_increment_positive(big_integer& i) {
    //Increment the first digit
    int32_t new_digit = i.digit_at(0) + 1;

    //Bail out early if there is no need to increment other digits
    if(new_digit <= 9) {
        i.set_digit_at(0, new_digit);
        return;
    }

    //Increment all subsequent digits (that are in the number) if the digits overflow
    int32_t digit_index = 1;
    while(digit_index < n_digits) {
        i.set_digit_at(digit_index - 1, 0); //zero the previous digit
        new_digit = i.digit_at(digit_index) + 1; //increment next digit

        //Don't need to increase next digit: bail out
        if(new_digit <= 9) {
            i.set_digit_at(digit_index, new_digit);
            return;
        }

        digit_index++;
    }

    
    //If all digits must be incremented (incrementing from 99999...): Add extra digit at the end
    i.n_digits++;
    if(digit_index % DIGITS_PER_STORAGE == 0) { //make more space if necessary
        i.contents.push_back(0);
    }
    i.set_digit_at(digit_index - 1, 0);
    i.set_digit_at(digit_index, 1);

    return;
}



big_integer big_integer::_subtract_positive(big_integer minuend, const big_integer& subtrahend) const {
    assert(minuend.n_digits >= subtrahend.n_digits && "Minuend must be at least the subtrahend");

    big_integer output;

    int32_t minuend_index = 0;

    int32_t minuend_digit = 0;
    int32_t subtrahend_digit = 0;

    while(minuend_index < subtrahend.n_digits) {
        minuend_digit = minuend.digit_at(minuend_index);
        subtrahend_digit = subtrahend.digit_at(minuend_index);

        //Digit-wise subtraction is positive: Subtract the digits immediately
        if(minuend_digit >= subtrahend_digit) {
            output.set_digit_at(minuend_index, minuend_digit - subtrahend_digit);
        }
        //If not: Borrow
        else {
            //Search for the first nonzero digit in the minuend, that is at least past the current place value
            int32_t borrow_digit_index = minuend_index + 1;
            while(minuend.digit_at(borrow_digit_index) <= 0) {
                //Implicitly add 10, then borrow a digit
                minuend.set_digit_at(borrow_digit_index, 9);

                borrow_digit_index++;
            }
            //Subtract 1 from current final index to complete the borrowing
            minuend.set_digit_at(borrow_digit_index, minuend.digit_at(borrow_digit_index) - 1);
            
            //Add 10 to current minuend digit and do subtraction
            output.set_digit_at(minuend_index, minuend_digit + 10 - subtrahend_digit);
        }


        //Allocate extra space if necessary
        minuend_index++;
        if(minuend_index % DIGITS_PER_STORAGE == 0 && minuend_index > 0) {
            output.contents.push_back(0);
        }
    }

    //Subtrahend is overrun: Insert digits of minuend as-is
    while(minuend_index < minuend.n_digits) {
        output.set_digit_at(minuend_index, minuend.digit_at(minuend_index));
        minuend_index++;

        //Allocate extra space if necessary
        if(minuend_index % DIGITS_PER_STORAGE == 0 && minuend_index > 0) {
            output.contents.push_back(0);
        }
    }

    output.n_digits = minuend_index;

    //Remove leading zeros
    if(output.digit_at(minuend_index-1) == 0) {
        minuend_index--;
        while(minuend_index > 0 && output.digit_at(minuend_index) == 0) {
            minuend_index--;
        }
        output.n_digits = minuend_index + 1;
    }

    return output;
}



inline int32_t big_integer::digits() const {
    return n_digits;
}



inline bool big_integer::is_negative() const {
    return negative;
}



int64_t big_integer::to_integer() const {
    int64_t output = 0;

    for(int64_t i = 0; i < n_digits; i++) {
        //Add digits multiplied by their place value
        output += (int64_t)pow((int64_t)10, i) * (int64_t)digit_at(i);

        //Check for out of bounds. Guaranteed to trigger if out of range
        if(output > INT64_MIN && output < 0) { //Bounds check on INT64_MIN is to handle edge case where the Big Integer equals INT64_MAX
            throw std::out_of_range("Integer overflow during Big Integer -> int64_t conversion");
        }
    }

    //Output exactly equals INT64_MIN: The only way to get this without triggering the exception is to be exactly INT64_MIN or INT64_MAX+1
    if(output == INT64_MIN) {
        //If positive, you overflowed
        if(!negative) {
            throw std::out_of_range("Integer overflow during Big Integer -> int64_t conversion");
        }
        //If negative, you are exactly INT64_MIN
        return INT64_MIN;
    }

    //Flip sign of negative inputs
    if(negative) {
        output *= -1;
    }

    return output;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

constexpr bool big_integer::operator==(const big_integer& other) const {
    //Opposite negative polarities: Cannot be equal
    if(this->negative != other.negative) {
        return false;
    }

    //Differing number of digits: Cannot be equal
    if(this->n_digits != other.n_digits) {
        return false;
    }

    //Element-wise compare digits, or rather, the digit storages
    for (int32_t i = (int32_t)contents.size() - 1; i >= 0; i--) {
        // std::cout << this->contents[i] << " " << other.contents[i] << std::endl;
        if(this->contents[i] != other.contents[i]) {
            return false;
        }
    }
    return true;
}



constexpr bool big_integer::operator!=(const big_integer& other) const {
    return !(*this == other);
}



constexpr std::strong_ordering big_integer::operator<=>(const big_integer& other) const {

    //Positive > negative
    if(!this->negative && other.negative) {
        return std::strong_ordering::greater;
    }
    //Negative < positive
    if(this->negative && !other.negative) {
        return std::strong_ordering::less;
    }

    //From this point on, the two numbers have the same positive/negative polarity

    //Compare number of digits
    if(this->n_digits != other.n_digits) {
        //negative: the one with *less* digits is greater
        if(negative) {
            return other.n_digits <=> this->n_digits;
        }
        else {
            return this->n_digits <=> other.n_digits;
        }
    }

    //Compare numbers element-wise (all but the last digit)
    for (size_t i = contents.size() - 1; i > 0; i--) {
        if(this->contents[i] != other.contents[i]) {
            //Negative: Smaller absolute value is greater
            if(negative) {
                return other.contents[i] <=> this->contents[i];
            }
            else {
                return this->contents[i] <=> other.contents[i];
            }
        }
    }
    //Only the least significant digits are not compared

    //Negative: Smaller absolute value is greater
    if(negative) {
        return other.contents[0] <=> this->contents[0];
    }
    return this->contents[0] <=> other.contents[0];
}



big_integer big_integer::operator+(const big_integer &other) const {
    //Negative signs have the same polarity: Add absolute values, then give the result the same sign
    if(this->negative == other.negative) {
        big_integer output = _add_positive(*this, other);
        output.negative = this->negative;
        return output;
    }
    
    //Positive + negative
    if(!this->negative && other.negative) {
        //abs(this) >= abs(other): result is positive
        if(abs(*this) >= abs(other)) {
            big_integer output = _subtract_positive(*this, other);
            return output;
        }
        //abs(this) < abs(other): result is negative
        else {
            big_integer output = _subtract_positive(other, *this);
            output.negative = true;
            return output;
        }
    }

    //Negative + positive

    //abs(this) >= abs(other): result is negative
    if(abs(*this) >= abs(other)) {
        big_integer output = _subtract_positive(*this, other);
        output.negative = true;
        return output;
    }
    //abs(this) < abs(other): result is positive
    big_integer output = _subtract_positive(other, *this);
    return output;
}



inline void big_integer::operator+=(const big_integer& other) {
    *this = *this + other;
}



big_integer& big_integer::operator++() {
    if(negative) {
        _decrement_positive(*this);
    }
    else {
       _increment_positive(*this);
    }

    //Avoid -0
    if(this->contents.size() == 1 && contents[0] == 0) {
        this->negative = false;
    }
    return *this;
}



big_integer big_integer::operator-(const big_integer& other) const {
    big_integer other_sign_flipped = other;
    other_sign_flipped.negative = !other_sign_flipped.negative;

    return *this + other_sign_flipped;
}



void big_integer::operator-=(const big_integer& other) {
    *this = *this - other;
}



big_integer& big_integer::operator--() {
    //Handle 0 as a special case
    if(n_digits == 1 && contents[0] == 0) {
        contents[0] = 1;
        negative = true;
        return *this;
    }

    if(negative) {
        _increment_positive(*this);
    }
    else {
       _decrement_positive(*this);
    }
    return *this;
}



big_integer big_integer::operator*(const big_integer& other) const {
    big_integer output;
    for(big_integer i = 0; i < abs(other); ++i) {
        output += *this;
    }   

    //Negate if factors' negative polarities are opposite, and result is nonzero
    if(this->negative != other.negative && output != 0) {
        output.negative = true;
    }
    return output;
}



void big_integer::operator*=(const big_integer& other) {
    *this = *this * other;
}



big_integer big_integer::operator/(const big_integer& other) const {
    if(other == big_integer(0)) {
        throw std::out_of_range("Cannot divide by zero");
    }

    big_integer output;
    big_integer count = abs(*this);
    const big_integer abs_other = abs(other);

    const big_integer one = big_integer(1);
    while(count > one) {
        ++output;
        count -= abs_other;
    }

    if(count > one) {
        --output;
    }

    //Negate if operands' negative polarities are opposite, and result is nonzero
    if(this->negative != other.negative && output != 0) {
        output.negative = true;
    }
    return output;
}



void big_integer::operator/=(const big_integer& other) {
    *this = *this / other;
}



template<typename CharT, typename Traits>
std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& output_stream, const big_integer& i) {

    //Export the negative sign
    if(i.negative) {
        output_stream << '-';
    }

    //Get and export each digit individually
    for(int32_t index = i.n_digits - 1; index >= 0; index--) {
        output_stream << i.digit_at(index);
    }
    return output_stream;
}




}
