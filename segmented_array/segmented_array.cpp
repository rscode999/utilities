//Uncomment to disable assertions.
// #define NDEBUG

#include <cassert>
#include <cstdint>
#include <stdexcept>
#include <string>

namespace utils {




/**
 * A 2D resizeable array that stores data of type `char` or `wchar_t` only.
 * 
 * Data is divided into segments of a set length. Segment and element indexing is 0-based.
 * 
 * The final segment may hold less elements than the other segments. If so, the final segment is the "remainder".
 * The remainder doesn't count as a segment, but elements of the remainder are accessed as if the remainder were the final segment.
 * 
 * Example data arrangement: 
 * [ (segment 0) ['a', 'b', 'c']  |  (segment 1) ['d', 'e', 'f']  |  (remainder / segment 2) ['g', 'h']].
 * This segmented array has 2 full-length segments and 3 elements per segment. The length of its remainder is 2.
 * 
 * Some operations may create unused space to increase speed of future append operations.
 * 
 * Uses a pointer to an array to store its contents. This is not a linked list
 * 
 * @param T datatype (either `char` or `wchar_t`) stored in the array
 */
template <typename T> 
class segmented_array {
/*
The original version did not support resizing.
*/

private:

    /**
     * Number of full-length segments in the array. The remainder does not count as a segment.
     * 
     * Non-negative.
     */
    int32_t n_segments;

    /**
     * Number of elements in each full-length segment in the array
     * 
     * Positive. Should not be changed after the array's construction.
     */
    int32_t n_elems_per_segment;

    /**
     * Number of elements in the remainder segment.
     * 
     * Non-negative.
     */
    int32_t n_remainder_elems;


    /**
     * A pointer to the data stored in the array.
     * 
     * In memory, every `n_elems_per_segment` elements are one segment.
     * How the segment indices look in the array:
     * (segment 0) 0, 1, 2... `n_elems_per_segment`-1, (segment 1) 0, 1, 2... `n_elems_per_segment`-1, (segment 2) 0, 1, 2... `n_elems_per_segment`-1, ...
     */
    T* contents;

    /**
     * Total number of elements pointed to by `contents`. May be larger than the total number of elements.
     * 
     * Non-negative, and at least the size of the array.
     */
    int64_t n_contents_elems;


    //////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////
    //PRIVATE HELPERS

    /**
     * Checks if `segment_number` and `elem_number` are valid indices in the array.
     * If not, raises an assertion.
     * 
     * Does nothing if assertions are disabled.
     * 
     * @param segment_number segment index to check
     * @param elem_number element index to check
     */
    void _assert_valid_index(int32_t segment_number, int32_t elem_number) const {
        //Automatic exit if assertions are disabled
        #ifndef NDEBUG
            //Check for integer overflow
            assert(n_segments >= 0 && "(POSSIBLE INTEGER OVERFLOW) Number of segments must be non-negative");
            assert(n_elems_per_segment > 0 && "(POSSIBLE INTEGER OVERFLOW) Number of elements per segment must be positive");
            assert(n_remainder_elems >= 0 && "(POSSIBLE INTEGER OVERFLOW) Number of remainder elements must be non-negative");
            assert(n_contents_elems >= 0 && "(POSSIBLE INTEGER OVERFLOW) Number of segments must be non-negative");

            //Check segment and element numbers are non-negative
            assert(segment_number >= 0 && "Segment number must be non-negative");
            assert(elem_number >= 0 && "Element number must be non-negative");

            //In remainder: check if remainder is exceeded
            if(segment_number == filled_segment_count()) {
                assert(elem_number < remainder_length() && "When accessing the remainder, element number must be less than the number of elements in the remainder");
            }
            //Not in remainder: check like normal 2D array
            else {
                assert(segment_number < filled_segment_count() && "Segment number must be less than the number of segments in the array");
                assert(elem_number < filled_segment_length() && "Element number must be less than the number of elements per segment");
            }

        #endif
    }



    /**
     * Appends `new_value` to the remainder. If the remainder is filled, a new segment is created and `new_value` becomes the first value in the segment.
     * 
     * If the array is too small to hold the new elements, the array increases its size to hold the new elements.
     * It also adds twice its new length in unused elements.
     * 
     * The method is private to prevent the public method `push_back` from becoming too cluttered.
     * 
     * @param new_value value to append to the array
     */
    void _push_back_char(T new_value) {

        //Idiot check
        assert(size() <= capacity() && "INTERNAL ERROR- Total element count cannot exceed total size");

        //If there is not enough room, extend the array
        if(size() == capacity()) {
            
            //Track the actual new size, since the doubling of the array size will not work if the array is empty
            int64_t new_size = size()==0 ? 1 : (size()+1)*2; //1 if array is empty, 2x the new size otherwise

            //Create new array pointer with more elements than before
            T* new_contents = new T[new_size];
            //Update size of contents
            n_contents_elems = new_size;

            //Copy current contents to new array, then put the new value in
            std::copy(contents, contents + size(), new_contents);
            
            //Make the current contents point to the newly allocated memory
            delete[] contents;
            contents = new_contents;
        }

        //add the new element
        contents[size()] = new_value;

        //Increase number of remainder elements. The new element was likely added to the remainder
        n_remainder_elems++;

        //If the remainder got filled, take the remainder as a new segment
        if(n_remainder_elems == n_elems_per_segment) {
            n_remainder_elems = 0;
            n_segments++;
        }
    }



    /**
     * Adds `new_value` to the end of the final segment, then removes any unused space. 
     * Faster than `push_back_string` and `shrink_to_fit` called separately.
     * 
     * The method is private to prevent the public method `push_back` from becoming too cluttered.
     * 
     * @param new_value new elements to add to the last segment
     */
    void _push_back_char_and_resize(T new_value) {
        //Make new pointer for the new element
        T* new_contents = new T[size() + 1];
        
        //Copy current contents to new array, then put the new value in
        std::copy(contents, contents + size(), new_contents);
        new_contents[size()] = new_value;
        
        
        //Switch pointers
        delete[] contents;
        contents = new_contents;

        //Increase number of remainder elements. The new element was likely added to the remainder
        n_remainder_elems++;

        //If the remainder got filled, take the remainder as a new segment
        if(n_remainder_elems == n_elems_per_segment) {
            n_remainder_elems = 0;
            n_segments++;
        }

        //Update contents size
        n_contents_elems = size();
    }



    /**
     * Adds the elements of `new_values` to the end of the array.
     * Creates enough new segments and elements to hold the new values.
     * 
     * If the array is too small to hold the new elements, the array increases its size to hold the new elements.
     * It also adds twice its new length in unused elements.
     * 
     * The method is private to prevent the public method `push_back` from becoming too cluttered.
     * 
     * @param new_values new elements to add to the last segment(s)
     */
    void _push_back_string(const std::basic_string<T>& new_values) {

        //If there is no more room, extend the array
        if(this->size() + new_values.length() - 1 >= n_contents_elems) {
            //Track the actual new size, since the doubling of the array size will not work if the array is empty
            int64_t new_size = size()==0 ? new_values.length() : ((int64_t)new_values.length() + this->size())*2; //string's length if array is empty, 2x the new size otherwise

            //Create new array pointer with enough elements to hold the new contents
            T* new_contents = new T[new_size];
            //Update size of contents
            n_contents_elems = new_size;

            //Copy current contents to new array
            std::copy(contents, contents + this->size(), new_contents);

            //Make the current contents point to the newly allocated memory
            delete[] contents;
            contents = new_contents;
        }

        //Put the new values in
        for(int64_t i = this->size(); i < this->size() + (int64_t)new_values.length(); i++) {
            contents[i] = new_values[i - size()];
        }

        //Update remainder and elem. counts
        n_segments = n_segments + ((n_remainder_elems + new_values.length()) / n_elems_per_segment);
        n_remainder_elems = (n_remainder_elems + new_values.length()) % n_elems_per_segment;
    }



    /**
     * Adds the elements of `new_values` to the end of the array.
     * Creates enough new segments and elements to hold the new values.
     * 
     * This function also removes unused space.
     * Faster than `push_back_string` and `shrink_to_fit` called separately.
     * 
     * The method is private to prevent the public method `push_back` from becoming too cluttered.
     * 
     * @param new_values value to append to the array
     */
    void _push_back_string_and_resize(const std::basic_string<T>& new_values) {

        //Make new pointer for the new elements
        T* new_contents = new T[size() + (int64_t)new_values.length()];

        //Copy current contents to new array
        std::copy(contents, contents + size(), new_contents);
        //Put the new values in
        for(int64_t i = size(); i < size() + (int64_t)new_values.length(); i++) {
            new_contents[i] = new_values[i - size()];
        }

        //Switch pointers
        delete[] contents;
        contents = new_contents;

        //Update remainder and elem. counts
        n_segments = n_segments + ((n_remainder_elems + new_values.length()) / n_elems_per_segment);
        n_remainder_elems = (n_remainder_elems + new_values.length()) % n_elems_per_segment;
        //Update contents size
        n_contents_elems = size();
    }



    /**
     * Removes `n_elems_to_remove` elements from the final segments.
     * 
     * The removal process will convert the removed elements into unused space.
     * 
     * The method is private to prevent the public method `pop_back` from being too cluttered.
     * 
     * @param n_elems_to_remove number of elements to remove. Cannot be negative, or greater than the total number of elements in the array.
     */
    void _pop_back_n(int32_t n_elems_to_remove) {
        //No need to enforce preconditions- the calling method does enforcing

        //Update remainder elements
        n_remainder_elems = (n_remainder_elems - n_elems_to_remove) % n_elems_per_segment;
        if(n_remainder_elems < 0) {
            n_remainder_elems += n_elems_per_segment;
        } 

        //Update segments
        n_segments = n_segments - ((n_remainder_elems + n_elems_to_remove) / n_elems_per_segment);
    }
    
    

    /**
     * Removes `n_elems_to_remove` elements from the final segment(s), along with any unused space.
     * 
     * The method is private to prevent the public method `pop_back` from being too cluttered.
     * 
     * @param n_elems_to_remove number of elements to remove. Cannot be negative or greater than the total number of elements in the array.
     */
    void _pop_back_n_and_resize(int32_t n_elems_to_remove) {
        //No need to enforce preconditions- the calling method does enforcing
        
        //Make new array
        T* new_contents = new T[size() - n_elems_to_remove];
        //Populate array with current contents
        std::copy(contents, contents + size() - n_elems_to_remove, new_contents);

        //Switch pointers
        delete[] contents;
        contents = new_contents;

        //Update contents size
        n_contents_elems = size() - n_elems_to_remove;

        //Update remainder elements
        n_remainder_elems = (n_remainder_elems - n_elems_to_remove) % n_elems_per_segment;
        if(n_remainder_elems < 0) {
            n_remainder_elems += n_elems_per_segment;
        } 

        //Update segments
        n_segments = n_segments - ((n_remainder_elems + n_elems_to_remove) / n_elems_per_segment);
    }


//////////////////////////////////////////////////////////////////////////////////////////////////////   
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////   
//////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////   
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////   
//////////////////////////////////////////////////////////////////////////////////////////////////////

public:

    /**
     * Creates a new Segmented Array with `segments` segments and `elements_per_segment` elements per segment, with an initial remainder size of `remainder_size`.
     * All elements are uninitialized.
     * 
     * @param segments number of segments in the array. Must be non-negative
     * @param elements_per_segment number of elements per segment in the array. Must be positive
     * @param remainder_size number of elements in the remainder. Must be non-negative and less than `elements_per_segment`. Default: 0
     */
    segmented_array(int32_t segments, int32_t elements_per_segment, int32_t remainder_size = 0) {
        using namespace std;
        static_assert(is_same<T, char>::value || is_same<T, wchar_t>::value, "The array is for types `char` and `wchar_t` only");
        assert((segments >= 0 && "Segments must be non-negative"));
        assert((elements_per_segment > 0 && "Elems. per segment must be positive"));
        assert((remainder_size >= 0 && "Remainder size cannot be negative"));
        assert((remainder_size < elements_per_segment && "Remainder size must be less than the number of elements per segment"));
        
        //Allocate the contents, or not
        if(segments == 0 && remainder_size == 0) {
            contents = nullptr;
        }
        else {
            contents = new T[segments * elements_per_segment + remainder_size];
        }

        //Set sizes
        n_segments = segments;
        n_elems_per_segment = elements_per_segment;
        n_remainder_elems = remainder_size;

        n_contents_elems = (int64_t)segments * (int64_t)elements_per_segment + (int64_t)remainder_size;
    }



    /**
     * Creates a new Segmented Array containing the elements of `initial_contents` split into pieces of size `elements_per_segment`.
     * If `initial_contents.length()` is not evenly divisible by `elements_per_segment`, the resulting array will have a remainder.
     * 
     * @param initial_contents contents to be placed in the new array
     * @param elements_per_segment number of elements per segment in the array. Must be positive
     */
    segmented_array(const std::basic_string<T>& initial_contents, int32_t elements_per_segment) {
        using namespace std;
        static_assert(is_same<T, char>::value || is_same<T, wchar_t>::value, "The array is for types `char` and `wchar_t` only");
        assert(elements_per_segment > 0 && "Number of elements per segment must be positive");

        //Allocate contents. If an error occurs, throw a runtime error
        contents = new T[initial_contents.length()];

        //Set contents to what is given
        for(int64_t i = 0; i < (int64_t)initial_contents.length(); i++) {
            contents[i] = initial_contents[i];
        }

        //Set quantities
        n_segments = (int32_t)initial_contents.length() / elements_per_segment;
        n_elems_per_segment = elements_per_segment;
        n_remainder_elems = initial_contents.length() % elements_per_segment;

        n_contents_elems = (int64_t)initial_contents.length();
    }



    /**
     * Creates a deep copy of `copy_array`, including its unused space (although unused space in the copy is uninitialized).
     * 
     * @param copy_array Segmented Array to copy
     */
    segmented_array(const segmented_array& copy_array) {
        n_segments = copy_array.n_segments;
        n_elems_per_segment = copy_array.n_elems_per_segment;
        n_remainder_elems = copy_array.n_remainder_elems;
        n_contents_elems = copy_array.n_contents_elems;

        contents = new T[n_contents_elems];
        std::copy(copy_array.contents, copy_array.contents + copy_array.size(), contents);
    }

    ///////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////
    //GETTERS

    
    /**
     * @return number of elements used by the array, including unused space
     */
    int64_t capacity() const noexcept {
        return n_contents_elems;
    }



    /**
     * @return number of full-length segments in the array
     * 
     * The remainder does not count as a full-length segment.
     */
    int32_t filled_segment_count() const noexcept {
        return n_segments;
    }



    /**
     * @return length of each full-sized segment in the array
     */
    int32_t filled_segment_length() const noexcept {
        return n_elems_per_segment;
    }



    /**
     * @return number of elements in the remainder. Returns 0 if no remainder exists.
     */
    int32_t remainder_length() const noexcept {
        return n_remainder_elems;
    }



    /**
     * @return number of elements in the array (excluding unused space)
     * 
     * Total elements = number of segments * number of elements per segment + number of remainder elements
     */
    int64_t size() const noexcept {
        return (int64_t)n_segments * (int64_t)n_elems_per_segment + (int64_t)n_remainder_elems;
    }



    /**
     * Returns the element at segment `segment_number` and position `element_number` in the array as a character value.
     * 
     * Segment and element indexing starts at 0. The first element of the first segment is at `{arrayName}.value_at(0,0)`.
     * 
     * The remainder's segment number, if a remainder exists, is `{arrayName}.filled_segment_count()`.
     * 
     * @param segment_number segment to access (0-based indexing). Must be a valid segment number in the array
     * @param element_number element to access in segment `segment_number` (0-based indexing). Must be a valid element number in the array
     * @return character value at segment `segment_number` and position `element_number`
     */
    const T& char_at(int32_t segment_number, int32_t element_number) const noexcept {
        _assert_valid_index(segment_number, element_number);
        return contents[n_elems_per_segment*segment_number + element_number];
    }

    /**
     * Returns the element at segment `segment_number` and position `element_number` in the array as a character value.
     * 
     * Segment and element indexing starts at 0. The first element of the first segment is at `{arrayName}.value_at(0,0)`.
     * 
     * The remainder's segment number, if a remainder exists, is `{arrayName}.filled_segment_count()`.
     * 
     * @param segment_number segment to access (0-based indexing). Must be a valid segment number in the array
     * @param element_number element to access in segment `segment_number` (0-based indexing). Must be a valid element number in the array
     * @return character value at segment `segment_number` and position `element_number`
     */
    T& char_at(int32_t segment_number, int32_t element_number) noexcept {
        _assert_valid_index(segment_number, element_number);
        return contents[n_elems_per_segment*segment_number + element_number];
    }



    /**
     * Returns the element at segment `segment_number` and position `element_number` in the array as an integer value.
     * 
     * Segment and element indexing starts at 0. The first element of the first segment is at `{arrayName}.value_at(0,0)`.
     * 
     * The remainder's segment number, if a remainder exists, is `{arrayName}.filled_segment_count()`.
     * 
     * @param segment_number segment to access (0-based indexing). Must be a valid segment number in the array
     * @param element_number element to access in segment `segment_number` (0-based indexing). Must be a valid element number in the array
     * @return value at segment `segment_number` and position `element_number`, as an integer
     */
    int32_t int_value_at(int32_t segment_number, int32_t element_number) const noexcept {
        _assert_valid_index(segment_number, element_number);
        return (int32_t)contents[n_elems_per_segment*segment_number + element_number];
    }
    
    

    /**
     * Returns the current contents of the array, in order, as a string of `char` or `wchar_t` (depending on the template's type).
     * 
     * Order: `{arrayName}.char_at(0,0)`, `{arrayName}.char_at(0,1)`, `{arrayName}.char_at(0,2)`...
     * `{arrayName}.char_at(1,0)`, `{arrayName}.char_at(1,1)`...
     * 
     * @return in-order string representation of contents
     */
    std::basic_string<T> to_string() const noexcept {
        //Initialize empty output
        std::basic_string<T> output;

        //Append all contents to the output string in order
        for(int64_t i = 0; i < size(); i++) {
            output.push_back(contents[i]);
        }

        //Resize to remove any extra space
        output.resize(size()); 
        return output;
    }
    


    ///////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////
    //SETTERS

    /**
     * Removes all elements and unused space from the array.
     */
    void clear() {
        delete[] contents;
        contents = nullptr;

        n_segments = 0;
        n_remainder_elems = 0;
        n_contents_elems = 0;
    }



    /**
     * Extends the array's unused space by `extension_length` elements.
     * 
     * Useful for repeated append operations, so the capacity rarely has to be increased.
     * 
     * @param extension_length number of elements to add to the array's unused space. Must be non-negative
     */
    void extend_capacity(int32_t extension_length) {
        assert((extension_length >= 0 && "Extension length cannot be negative"));
        if(extension_length == 0) {
            return;
        }
        
        //Make new contents and load it
        T* new_contents = new T[size() + (int64_t)extension_length];
        std::copy(contents, contents + size(), new_contents);
        
        //Switch pointers
        delete[] contents;
        contents = new_contents;
        
        //Update contents size
        n_contents_elems += extension_length;
    }



    /**
     * Adds `new_value` to the end of the remainder.
     * If the remainder is filled, a new segment is created and `new_value` becomes the first value in the segment.
     * 
     * If `shrink_to_fit` is false and the array has too little space to hold the new elements, the array adds twice its new length in unused elements.
     * If `shrink_to_fit` is true, the array adds the new element, then unconditionally removes all unused space.
     * 
     * @param new_value new element to add to the array
     * @param shrink_to_fit whether to remove unused space after the operation. Default: false.
     */
    void push_back(T new_value, bool shrink_to_fit = false) {
        if(shrink_to_fit) {
            _push_back_char_and_resize(new_value);
        }
        else {
            _push_back_char(new_value);
        }
    }

    /**
     * Adds `new_values` to the end of the last segment, creating enough space to fit the new elements.
     * If the remainder is filled, a new segment is created and `new_values` becomes the first value in the segment.
     * 
     * If `shrink_to_fit` is false and the array has too little space to hold the new elements, the array adds twice its new length in unused elements.
     * If `shrink_to_fit` is true, the array adds the new elements, then removes all unused space.
     * 
     * @param new_values new elements to add to the array
     * @param shrink_to_fit whether to remove unused space after the operation. Default: false.
     */
    void push_back(const std::basic_string<T>& new_values, bool shrink_to_fit = false) {
        //Idiot check
        assert((size() <= capacity() && "INTERNAL ERROR- Total element count cannot exceed total size"));

        //Early exit
        if(new_values.length() == 0) {
            return;
        }

        if(shrink_to_fit) {
            _push_back_string_and_resize(new_values);
        }
        else {
            _push_back_string(new_values);
        }
    }
    


    /**
     * Removes `n_elements_to_remove` elements from the final segment(s) of the array.
     * 
     * If `shrink_to_fit` is false, the removed elements are converted to unused space. 
     * If `shrink_to_fit` is true, all unused space is removed after the operation.
     * 
     * @param n_elements_to_remove number of elements to remove. Must be non-negative and no greater than the amount of elements in the array. Default 1.
     * @param shrink_to_fit whether to remove unused space after the operation. Default: false.
     */
    void pop_back(int32_t n_elements_to_remove = 1, bool shrink_to_fit = false) {
        //Idiot check
        assert((size() <= capacity() && "INTERNAL ERROR- Total element count cannot exceed total size"));

        assert((n_elements_to_remove >= 0 && "Number of elements to remove cannot be negative"));
        assert((n_elements_to_remove <= size() && "Number of elements to remove cannot exceed the number of elements in the array"));

        //Early exit
        if(n_elements_to_remove == 0) {
            return;
        }

        if(shrink_to_fit) {
            _pop_back_n_and_resize(n_elements_to_remove);
        }
        else {
            _pop_back_n(n_elements_to_remove);
        }
    }



    /**
     * Sets the element at segment `segment_number` and position `element_number` in the array to `new_value`.
     * 
     * The remainder's segment number, if a remainder exists, is `{arrayName}.filled_segment_count()`.
     * 
     * @param segment_number segment number to change. Must be a valid segment number in the array
     * @param element_number element number to change. Must be a valid element number in the array
     * @param new_value new value to set at segment `segment_number` and element `element_number`
     */
    void set_char_at(int32_t segment_number, int32_t element_number, T new_value) {
        _assert_valid_index(segment_number, element_number);
        contents[n_elems_per_segment*segment_number + element_number] = new_value;
    }



    /**
     * Sets the element at segment `segment_number` and position `element_number` in the array to `new_value`.
     * 
     * The remainder's segment number, if a remainder exists, is `{arrayName}.filled_segment_count()`.
     * 
     * @param segment_number segment number to change. Must be a valid segment number in the array
     * @param element_number element number to change. Must be a valid element number in the array
     * @param new_value value to set at segment `segment_number` and element `element_number`
     */
    void set_int_value_at(int32_t segment_number, int32_t element_number, int32_t new_value) {
        _assert_valid_index(segment_number, element_number);
        contents[n_elems_per_segment*segment_number + element_number] = (T)new_value;
    }



    /**
     * Removes any unused space in the array. 
     * Unused space may be created during append operations or left behind in remove operations.
     */
    void shrink_to_fit() {
        //Idiot check
        assert((size() <= capacity() && "INTERNAL ERROR- Total element count cannot exceed total size"));

        //Create new array pointer with barely enough elements to hold the contents
        T* new_contents = new T[size()];

        //Copy current contents to new array
        std::copy(contents, contents + size(), new_contents);

        //Make the current contents point to the newly allocated memory
        delete[] contents;
        contents = new_contents;

        //Update size of contents
        n_contents_elems = size();
    }


    //////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////
    //OPERATOR OVERLOADS

    
    /**
     * Copies the contents of `other_array` into this array, returning a reference to this array.
     *
     * @param other_array other array to copy
     * @return reference to the newly copied array
     */
    segmented_array& operator=(const segmented_array& other_array) {
        //self assignment check
        if (this == &other_array) {
            return *this;
        }
        
        //remove current contents
        delete[] contents;
        
        //copy all data from the other array into this array
        n_segments = other_array.n_segments;
        n_elems_per_segment = other_array.n_elems_per_segment;
        n_remainder_elems = other_array.n_remainder_elems;
        n_contents_elems = other_array.n_contents_elems;

        contents = new T[other_array.n_contents_elems];

        std::copy(other_array.contents, other_array.contents + other_array.n_contents_elems, contents);
        return *this;
    }
    


    /**
     * Returns whether this array and `other_array` are equal in length, have the same number of elements per segment,
     * and have equal corresponding elements.
     * 
     * @param other_array array to compare to this array
     * @return true if arrays are equal, false otherwise
     */
    bool operator==(const segmented_array& other_array) const {
        //Check lengths
        if(other_array.size() != size()) {
            return false;
        }

        //Check elems. per segment
        if(other_array.filled_segment_length() != filled_segment_length()) {
            return false;
        }

        //Check corresponding elements
        for (int64_t i = 0; i < size(); i++) {
            if(other_array.contents[i] != contents[i]) {
                return false;
            }
        }
        
        return true;
    }



    /**
     * Adds `new_char` to the end of the array.
     * 
     * Equivalent to the `push_back` method.
     * 
     * @param new_char character to add to the end of the array
     */
    void operator+=(T new_char) {
        this->_push_back_char(new_char);
    }

    /**
     * Adds the elements of `new_chars`, in order, to the end of the array.
     * 
     * Equivalent to the `push_back` method.
     * 
     * @param new_chars characters to add to the end of the array
     */
    void operator+=(const std::basic_string<T>& new_chars) {
        this->_push_back_string(new_chars);
    }
    


    /**
     * Returns the element at segment `segment_number` and position `element_number` in the array as a character value.
     * 
     * Segment and element indexing starts at 0. The first element of the first segment is at `{arrayName}.value_at(0,0)`.
     * 
     * The remainder's segment number, if a remainder exists, is `{arrayName}.filled_segment_count()`.
     * 
     * Equivalent to the non-const `char_at` method.
     * 
     * @param segment_number segment to access (0-based indexing). Must be a valid segment number in the array
     * @param element_number element to access in segment `segment_number` (0-based indexing). Must be a valid element number in the array
     * @return character value at segment `segment_number` and position `element_number`
     */
    T& operator() (int32_t segment_number, int32_t element_number) {
        _assert_valid_index(segment_number, element_number);
        return contents[n_elems_per_segment*segment_number + element_number];
    }

    /**
     * Returns the element at segment `segment_number` and position `element_number` in the array as a character value.
     * 
     * Segment and element indexing starts at 0. The first element of the first segment is at `{arrayName}.value_at(0,0)`.
     * 
     * The remainder's segment number, if a remainder exists, is `{arrayName}.filled_segment_count()`.
     * 
     * Equivalent to the const `char_at` method.
     * 
     * @param segment_number segment to access (0-based indexing). Must be a valid segment number in the array
     * @param element_number element to access in segment `segment_number` (0-based indexing). Must be a valid element number in the array
     * @return character value at segment `segment_number` and position `element_number`
     */
    const T& operator() (int32_t segment_number, int32_t element_number) const {
        _assert_valid_index(segment_number, element_number);
        return contents[n_elems_per_segment*segment_number + element_number];
    }
    


    /**
     * Exports the in-order contents of `arr` to the output stream `output_stream`, returning a reference to `output_stream` with `arr` added.
     * 
     * Equivalent to `output_stream << arr.to_string()`.
     * 
     * @param output_stream output stream to export to
     * @param arr Segmented Array to export
     * @return `output_stream` containing the array's information inside
     */
    template<typename CharT, typename Traits, typename Tp>
    friend std::basic_ostream<Tp>& operator<<(std::basic_ostream<CharT, Traits>& output_stream, const segmented_array<Tp>& arr);


    //////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////
    //DESTRUCTOR

    /**
     * Properly destroys a Segmented Array.
     */
    ~segmented_array() {
        delete[] contents;
        contents = nullptr;
    }
};



template<typename CharT, typename Traits, typename T>
std::basic_ostream<T>& operator<<(std::basic_ostream<CharT, Traits>& output_stream, const segmented_array<T>& arr) {
    for(int64_t i = 0; i < arr.size(); i++) {
        output_stream << arr.contents[i];
    }
    return output_stream;
}




}