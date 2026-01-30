#include <cassert>
#include <cstdint>
#include <iostream>
#include <stdexcept>


/**
 * Returns the distance between `a` and `b` on the integer number line. The output is never negative.
 * @param a first number to compare
 * @param b second number to compare
 * @return absolute difference of the inputs
 * 
 */
inline int32_t distance(int32_t a, int32_t b) {
    return (a>b) ? a-b : b-a;
}




namespace utils {




/**
 * Linked list that supports addition and removal at both the front and the back.
 * 
 * The list maintains a reference to the most recently used element for faster operations.
 * 
 * The previously used element can be changed by reading or writing to a list element that is not the first or last element.
 * If the previously accessed element is removed, the new previous element becomes the one at the start or end of the list.
 * 
 * @param T datatype of the stored items
 */
template<typename T>
class fast_linked_list {

private:
    /**
     * A node in a fast linked list containing data (`data`), a pointer to a predecessor node (`prev`), 
     * and a pointer to a successor node (`next`).
     * 
     * A node at the start of the list has a `nullptr` predecessor.
     * A node at the end of the list has a `nullptr` successor.
     */
    struct Node {
        Node* prev;
        Node* next;
        T data;
    };


    /**
     * Pointer to the starting node
     */
    Node* start_node;

    /**
     * Pointer to the ending node
     */
    Node* end_node;

    /**
     * Pointer to the last node used
     */
    Node* last_used_node;

    /**
     * Index number (0-based indexing) of the last node used
     */
    int32_t last_used_index;

    /**
     * Number of elements in the list. Non-negative.
     */
    int32_t n_elements;



    /**
     * Returns a pointer to the node at list index `index`.
     * 
     * Private helper method.
     * 
     * @param index index to get in the list. Must be on the interval [ 0, `list.size()`-1 ]
     * @return pointer to the index's node
     */
    Node* get_list_position(int32_t index) const;


    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////


public:

    /**
     * Creates an empty Fast Linked List.
     */
    fast_linked_list() {
        start_node = nullptr;
        end_node = nullptr;
        last_used_node = nullptr;

        n_elements = 0;
        last_used_index = 0;
    }



    /**
     * Creates a Fast Linked List containing the items in `initial_contents` in order.
     * 
     * The last used index is set to `initial_contents.size() / 2`.
     * 
     * @param initial_contents std::vector of items to load the list with
     */
    fast_linked_list(const std::vector<T>& initial_contents) {
        for(int32_t i = 0; i < (int32_t)initial_contents.size(); i++) {
            this->push_back(initial_contents[i]);

            if(i == initial_contents.size() / 2) {
                last_used_index = (int32_t)initial_contents.size() / 2;
                last_used_node = end_node;
            }
        }
        n_elements = initial_contents.size();
    }



    /**
     * Copies `other_list` into a new list.
     * @param other_list list to copy into this list
     */
    fast_linked_list(const fast_linked_list& other_list) : fast_linked_list() {
        Node* other_list_current = other_list.start_node;

        while (other_list_current != nullptr) {

            this->push_back(other_list_current->data);
            //push_back increases this->n_elements upon each call

            //check for last accessed element copy
            if(this->n_elements-1 == other_list.last_used_index) {
                last_used_index = n_elements-1;
                last_used_node = this->end_node;
            }
        
            other_list_current = other_list_current->next;
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    //METHOD DECLARATIONS


    /**
     * Returns a reference to the value at index `index` (0-based indexing).
     * If `setting_last_accessed` is true, the last accessed value will be set to the one at `index`.
     * 
     * If `index` is negative or at least the list's size, throws `std::out_of_range`.
     * 
     * The last accessed value will *not* be changed if the first or last index is retrieved,
     * regardless of the value of `setting_last_accessed`.
     * 
     * @param index index to get value from
     * @param setting_last_accessed whether to change the last accessed value to the one at `index`
     * @return value at the specified index
     * @throws `std::out_of_range` if `index` is out of the array's bounds
     * 
     */
    T& at(int32_t index, bool setting_last_accessed = true);

    /**
     * Returns the index number of the element (0-based indexing) last retrieved or modified.
     * 
     * If the list is empty, throws a `std::out_of_range`.
     * 
     * @return index of the element marked for quick access
     * @throws `std::out_of_range` if the list is empty
     */
    int32_t last_accessed_index();

    /**
     * Returns a reference to the element last retrieved or modified.
     * 
     * If the list is empty, throws `std::out_of_range`.
     * 
     * @return reference to the previous element retrieved or edited
     * @throws `std::out_of_range` if the list is empty
     */
    T& last_accessed_element();

    /**
     * Removes the final element of the list.
     * 
     * If the last accessed value is the final index,
     * the new last accessed value becomes the new end of the list.
     * 
     * Throws `std::out_of_range` if the list is empty.
     * 
     * @throws `std::out_of_range` if the list is empty
     */
    void pop_back();

    /**
     * Removes the first element of the list.
     * 
     * If the last accessed value is the first index,
     * the new last accessed value becomes the new front of the list.
     * 
     * Throws `std::out_of_range` if the list is empty.
     * 
     * @throws `std::out_of_range` if the list is empty
     */
    void pop_front();

    /**
     * Removes the last accessed element.
     * 
     * The new last accessed element becomes the element after the removed element.
     * If the last accessed element is the final element, the new last accessed element becomes
     * the new end of the list.
     * 
     * If the list is empty, throws `std::out_of_range`.
     * 
     * @throws `std::out_of_range` if the list is empty
     */
    void pop_last_accessed();

    /**
     * Adds `new_value` to the end of the list.
     * @param new_value value to add to the end
     * 
    */
    void push_back(T new_value);

    /**
     * Adds `new_value` to the front of the list.
     * 
     * The last accessed index will increase by 1. The last accessed value remains the same.
     * 
     * @param new_value value to add to the end
     * 
     */
    void push_front(T new_value);

    /**
     * Adds `new_value` at the index of the last accessed element.
     * 
     * All elements at or after the last acccessed element will have their indices increase by 1.
     * 
     * The newly added element becomes the new last accessed element.
     * 
     * If the list is empty, throws `std::out_of_range`.
     * 
     * @param new_value value to add to the last accessed
     * @throws `std::out_of_range` if the list is empty
     */
    void push_last_accessed(T new_value);


    /**
     * Sets the value at index `index` (0-based indexing) to `new_value`.
     * If `setting_last_accessed` is true, the last accessed index will be set to `index`.
     * 
     * If `index` is negative or at least the list's size, throws `std::out_of_range`.
     * 
     * The last accessed value will *not* be changed if the first or last index is set,
     * regardless of the value of `setting_last_accessed`.
     * 
     * @param index index to change in the list
     * @param new_value value to set
     * @param setting_last_accessed whether to change the last accessed value to the one at `index`. Default: true
     * @throws `std::out_of_range` if `index` is out of the array's bounds
     * 
     */
    void set(int32_t index, T new_value, bool setting_last_accessed = true);

    /**
     * 
     * @return the number of elements in the list
     */
    int32_t size(); 


    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    //OPERATOR OVERLOADS

    /**
     * Copies `other_list` into this Fast Linked List object, returning a reference to this list after the modification.
     * 
     * The location of `other_list`'s last accessed value is also copied.
     * 
     * @param other_list list to copy into this list
     * @return reference to this (modified) list
     */
    fast_linked_list& operator=(const fast_linked_list& other_list) {
        using namespace std;

        //self-assignment check
        if(this == &other_list) {
            return *this;
        }

        Node* current = start_node;
        int32_t current_index = 0;

        Node* other_current = other_list.start_node;

        //while both have not run out of elements
        while(current != nullptr && other_current != nullptr) {
            // cout << current->data << " " << endl;
            // cout << other_current->data << " (other)" << endl;
            
            //directly transfer data without making or deleting nodes
            current->data = other_current->data;
            //IMPORTANT: here, this list's nodes remain the same. The only thing that changes is their data.

            //check for last accessed
            if(current_index == other_list.last_used_index) {
                this->last_used_index = current_index;
                this->last_used_node = current;
            }
            
            //advance nodes in both this list and the other list
            current = current->next;
            // current_to_delete = current;

            other_current = other_current->next;

            current_index++;
        }

        //if the other list is longer than this list
        while(other_current != nullptr) {
            // cout << other_current->data << "** (other)" << endl;

            this->push_back(other_current->data);
            other_current = other_current->next;

            //check for last accessed
            if(current_index == other_list.last_used_index) {
                this->last_used_index = current_index;
                this->last_used_node = this->end_node;
            }
            current_index++;
        }

        // cout << this->n_elements << "x" << endl;
        // cout << other_list.n_elements << "x" << endl;

        //if this list is longer than the other list, delete all extra nodes
        while(this->n_elements > other_list.n_elements) {
            this->pop_back();
        }

        return *this;
    }
    


    /**
     * Returns `true` if this list and `other_list` have the same number of elements,
     * and each corresponding element is equal.
     * Returns `false` otherwise.
     * 
     * @param other_list list to compare to
     * @return whether the lists are equal (length and elements)
     */
    bool operator==(const fast_linked_list& other_list) const {
        //check size
        if(other_list.n_elements != this->n_elements) {
            return false;
        }

        Node* current_node = this->start_node;
        Node* other_current_node = other_list.start_node;
        
        //check each element
        while(current_node != nullptr && other_list.start_node != nullptr) {
            if(current_node->data != other_current_node->data) {
                return false;
            }

            current_node = current_node->next;
            other_current_node = other_current_node->next;
        }

        return true;
    }



    /**
     * Returns a non-constant reference to the value at index `index` (0-based indexing), and sets the last accessed value to the one at `index`.
     * If `index` is negative or at least the list's size, throws `out_of_range`.
     * 
     * The last accessed value will *not* be changed if the first or last index is retrieved.
     * 
     * Equivalent to `{listName}.at(index, true)`.
     * 
     * @param index the index to get the value from
     * @return reference to value at index `index`
     * @throws `out_of_range` if `index` is out of the array's bounds
     * 
     */
    T& operator[](int32_t index) {
        return at(index, true);
    }


    
    /**
     * Exports `fll` to `output_stream`, returning a reference to `output_stream` with `fll` inside.
     * 
     * The contents of `fll` are surrounded in curly braces and separated by commas.
     * 
     * @param output_stream stream to send the list into
     * @param fll list to export
     * @return reference to `output_stream` with `fll` inside
     */
    template<typename CharT, typename Traits, typename Tp>
    friend std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& output_stream, const fast_linked_list<Tp>& fll);
    

    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    //DESTRUCTOR


    /**
     * Properly destroys a Fast Linked List.
     */
    ~fast_linked_list() {
        Node* current = start_node;
        Node* delete_node = start_node;

        while(current != nullptr) {
            current = current->next;
            delete delete_node;
            delete_node = current;
        }
    }
};



/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//METHOD DEFINITIONS


template<typename T>
typename fast_linked_list<T>::Node* fast_linked_list<T>::get_list_position(int32_t index) const {
    assert((n_elements >= 0 && "INTERNAL ERROR- Number of elements cannot be negative"));
    assert((index >= 0 && index < n_elements && "INTERNAL ERROR- index is out of bounds"));
    assert((last_used_node != nullptr && "INTERNAL ERROR- Last accessed node cannot be null"));
    assert((last_used_index >= 0 && last_used_index < n_elements && "INTERNAL ERROR- Last accessed index is out of list's bounds"));
    
    //Determine iteration direction, start node, and start index
    bool iterating_forward = true;
    int32_t current_index = 0;
    Node* current_node = start_node;

    //Past last accessed index: compare last accessed index and end
    if(index > last_used_index) {
        //closer to last accessed index: iterate forward starting from last accessed
        if(distance(index, last_used_index) < distance(index, n_elements-1)) { 
            iterating_forward = true;
            current_index = last_used_index;
            current_node = last_used_node;
        }
        //closer to end: iterate backward starting from end
        else {
            iterating_forward = false;
            current_index = n_elements-1;
            current_node = end_node;
        }
    }
    //At or before last accessed: compare start and last accesed
    else {
        //closer to last accessed index: iterate backward starting from last accessed
        if(distance(index, last_used_index) < distance(index, 0)) {
            iterating_forward = false;
            current_index = last_used_index;
            current_node = last_used_node;
        }
        //closer to start: iterate forward from the start (already set in initialization)
    }

    // cout << "iteration forward: " << iterating_forward << endl;
    // cout << "index: " << current_index << endl;
    // cout << "data: " << current->data << endl;

    //Move through the array toward the desired index
    while(current_node != nullptr) {
        //Copy the current node's contents when the correct index is reached
        if(current_index == index) {
            return current_node;
        }

        //Move to the next node and update the index, depending on iteration direction
        if(iterating_forward) {
            current_node = current_node->next;
            current_index++;
        }
        else {
            current_node = current_node->prev;
            current_index--;
        }
    }

    //Execution should never get here
    if(current_index <= 0) {
        throw std::runtime_error("INTERNAL ERROR: Underrun");
    }
    throw std::runtime_error("INTERNAL ERROR: Overrun");
}




template<typename T>
T& fast_linked_list<T>::at(int32_t index, bool setting_last_accessed) {
    //check index range
    if(!(index >= 0 && index < n_elements)) {
        throw std::out_of_range("Element retrieval index must be at least 0 and less than the array's size");
    }

    //Get node at the desired position
    Node* position = get_list_position(index);

    //Set last accessed, if selected and the user didn't get the first or last index
    if(setting_last_accessed && !(index==0 || index==n_elements-1)) {
        last_used_node = position;
        last_used_index = index;
    }

    //Take data from the node and return it
    T& data = position->data;
    return data;
}



template<typename T>
inline int32_t fast_linked_list<T>::last_accessed_index() {
    //check for empty list
    if(n_elements == 0) {
        throw std::out_of_range("Cannot access the last accessed index of an empty list");
    }
    //check index range
    assert((last_used_index >= 0 && last_used_index < n_elements && "INTERNAL ERROR- Last accessed index out of range"));

    return last_used_index;
}



template<typename T>
inline T& fast_linked_list<T>::last_accessed_element() {
    //check for empty list
    if(n_elements == 0) {
        throw std::out_of_range("Cannot access the last used element of an empty list");
    }
    //check index range
    assert((last_used_index >= 0 && last_used_index < n_elements && "INTERNAL ERROR- Last accessed index out of range"));

    return last_used_node->data;
}



template<typename T>
void fast_linked_list<T>::pop_back() {
    assert((n_elements >= 0 && "INTERNAL ERROR- Number of elements cannot be negative"));
    if(n_elements == 0) {
        throw std::out_of_range("Cannot remove elements from the back of an empty list");
    }

    if(n_elements == 1) {
        //delete the only node
        Node* delete_node = end_node; 
        delete delete_node;
        
        //set everything to nullptr
        start_node = nullptr;
        end_node = nullptr;  
        last_used_node = nullptr;
    }   
    else {
        Node* delete_node = end_node; //save the last element
        end_node = end_node->prev; //move the end one element forward
        delete delete_node; //remove the last element
        end_node->next = nullptr; //set the end to nullptr
    }

    n_elements--;

    //change last accessed if needed
    if(last_used_index >= n_elements) {
        last_used_node = end_node;
        last_used_index = n_elements - 1;
    }
}



template<typename T>
void fast_linked_list<T>::pop_front() {
    assert((n_elements >= 0 && "INTERNAL ERROR- Number of elements cannot be negative"));
    if(n_elements == 0) {
        throw std::out_of_range("Cannot remove elements from the front of an empty list");
    }
    
    if(n_elements == 1) {
        //delete the only node
        Node* delete_node = start_node;
        delete delete_node;

        //set everything to nullptr
        start_node = nullptr;
        end_node = nullptr;
        last_used_node = nullptr;
        last_used_index = 0;
    }
    else {
        //make new node
        Node* delete_node = start_node;
        start_node = start_node->next;
        delete delete_node;
        start_node->prev = nullptr;
    }
    
    n_elements--;

    //move last accessed down one index
    if(last_used_index > 0) {
        last_used_index -= 1;
    }
    //if last accessed is at the start, move the pointer up
    else if(last_used_index == 0) {
        last_used_node = start_node;
    }
}



template<typename T>
void fast_linked_list<T>::pop_last_accessed() {

    //assertions
    assert((n_elements >= 0 && "INTERNAL ERROR- Number of elements cannot be negative"));
    assert((last_used_index >= 0 && last_used_index < n_elements && "INTERNAL ERROR- Last accessed index out of range"));

    //no elements: throw exception
    if(n_elements == 0) {
        throw std::out_of_range("Cannot remove the last accessed element of an empty list");
    }
    //single element
    else if(n_elements == 1) {
        //delete the only node
        Node* delete_node = start_node;
        delete delete_node;

        //set everything to nullptr
        start_node = nullptr;
        end_node = nullptr;
        last_used_node = nullptr;
        last_used_index = 0;
    }
    //last accessed at start: essentially pop front
    else if(last_used_index == 0) {
        //delete the start node
        Node* delete_node = start_node;
        start_node = start_node->next;
        delete delete_node;
        start_node->prev = nullptr;

        //reconfigure last accessed
        last_used_node = start_node;
        last_used_index = 0;
    }
    //last accessed at end: essentially pop back
    else if(last_used_index == n_elements-1) {
        Node* delete_node = end_node; //save the last element
        end_node = end_node->prev; //move the end one element forward
        delete delete_node; //remove the last element
        end_node->next = nullptr; //set the end to nullptr

        //reconfigure last accessed
        last_used_node = end_node;
        last_used_index = n_elements - 2; //because the number of elements will decrease
    }
    //last accessed in middle: add to the last accessed
    else {
        //configure node to delete
        Node* delete_node = last_used_node;

        //reconfigure connections
        last_used_node->prev->next = last_used_node->next;
        last_used_node->next->prev = last_used_node->prev;

        //delete the node
        last_used_node = last_used_node->next;
        delete delete_node;
    }

    n_elements--;
}


template<typename T>
void fast_linked_list<T>::push_back(T new_value) {
    assert((n_elements >= 0 && "INTERNAL ERROR- Number of elements cannot be negative"));

    //configure a new node with the desired value
    Node* new_node = new Node(); 
    new_node->data = new_value;
    new_node->prev = nullptr;
    new_node->next = nullptr;

    //no elements in list: make the new node the only node
    if(n_elements == 0) {
        start_node = new_node;
        end_node = new_node;
        last_used_node = start_node;
        last_used_index = 0;
    }
    //elements in list: put the new node at the end
    else {
        end_node->next = new_node;
        new_node->prev = end_node;

        end_node = new_node;
    }

    n_elements++;
}



template<typename T>
void fast_linked_list<T>::push_front(T new_value) {
    assert((n_elements >= 0 && "INTERNAL ERROR- Number of elements cannot be negative"));

    //configure a new node with the desired value
    Node* new_node = new Node();
    new_node->data = new_value;
    new_node->prev = nullptr;
    new_node->next = nullptr;

    //no elements: set first and only node to the new node
    if(n_elements==0) {
        start_node = new_node;
        end_node = new_node;
        last_used_node = start_node;
        last_used_index = 0;
    }
    //elements: add node to front
    else {
        new_node->next = start_node;
        start_node->prev = new_node;

        start_node = new_node;

        last_used_index++;
    }

    n_elements++;
}



template<typename T>
void fast_linked_list<T>::push_last_accessed(T new_value) {
    assert((n_elements >= 0 && "INTERNAL ERROR- Number of elements cannot be negative"));

    //no elements: throw exception
    if(n_elements == 0) {
        throw std::out_of_range("Cannot add values in front of the last used element, in an empty list");
    }
    assert((last_used_index>=0 && last_used_index<n_elements && "INTERNAL ERROR- Last accessed index out of range"));

    //configure a new node with the desired value
    Node* new_node = new Node();
    new_node->data = new_value;
    new_node->prev = nullptr;
    new_node->next = nullptr;

    //last accessed at start: essentially push front
    if(last_used_index == 0) {
        new_node->next = start_node;
        start_node->prev = new_node;
        start_node = new_node;

        //reconfigure last accessed
        last_used_node = start_node;
        last_used_index = 0;
    }
    //last accessed at end: essentially push back
    else if(last_used_index == n_elements-1) {
        end_node->next = new_node;
        new_node->prev = end_node;
        end_node = new_node;

        //reconfigure last accessed
        last_used_node = end_node;
        last_used_index = n_elements; //no need to subtract 1 because n_elements will be incremented later
    }
    //last accessed in middle: add to the last accessed
    else {
        //configure new node
        new_node->next = last_used_node;
        new_node->prev = last_used_node->prev;

        //configure last accessed
        last_used_node->prev->next = new_node;
        last_used_node->prev = new_node;

        //fix last accessed
        last_used_node = new_node;
    }

    n_elements++;
}



template<typename T>
void fast_linked_list<T>::set(int32_t index, T new_value, bool setting_last_accessed) {
    if(!(index>=0 && index<n_elements)) {
        throw std::out_of_range("Index must be at least 0 and less than the list's size");
    }

    //Find the node at the specified index
    Node* pos = get_list_position(index);

    //Reconfigure last accessed, if desired and the value set is not the first or last index
    if(setting_last_accessed && !(index==0 || index==n_elements-1)) {
        last_used_node = pos;
        last_used_index = index;
    }

    //Update the node
    pos->data = new_value;
}



template<typename T>
inline int32_t fast_linked_list<T>::size() {
    return n_elements;
}



template<typename CharT, typename Traits, typename T>
std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& output_stream, const fast_linked_list<T>& fll) {
    assert((fll.n_elements>=0 && "INTERNAL ERROR- Number of elements cannot be negative"));

    const CharT open[]  = { CharT('{'), CharT('\0') };
    const CharT close[] = { CharT('}'), CharT('\0') };
    const CharT comma[] = { CharT(','), CharT(' '), CharT('\0') };

    if(fll.n_elements == 0) {
        output_stream << open << close;
        return output_stream;
    }

    output_stream << open;
    
    auto* current_node = fll.start_node;
    while(current_node->next != nullptr) {
        output_stream << current_node->data << comma;
        current_node = current_node->next;
    }

    output_stream << current_node->data << close;

    return output_stream;
}




}