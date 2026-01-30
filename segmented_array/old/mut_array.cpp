#include <cstdlib>
#include <iostream>
#include <string>
using namespace std;

/*
Header file for an array that can give its own size and resize itself.

Chris P. Bacon
August 31, 2024
*/



/**
 * Thrown to indicate that a precondition was not met.
 */
class assertion_error : exception {
    public: 
        /**
         * Creates a new assertion error
         */
        assertion_error() {
        }

        /**
         * Creates a new assertion error and prints `msg`.
         */
        assertion_error(string msg) {
            cout << "(" << msg << ") ";
        }
};


/**
 * Represents an array capable of giving its own size and resizing itself.
 * 
 * Uses a pointer to an array to store its contents.
 */
class mut_array {
    private:
        /**
         * Pointer to the array's contents.
         */
        char* contents;

        /**
         * The number of elements in the array
         */
        long long nElements;

    public:
        /**
         * Creates a new array whose contents hold the corresponding indices of `input`.
         */
        mut_array(string input) {
            nElements = input.length();

            contents = (char*) malloc(input.length() * sizeof(char));
            for(long long i=0LL; i<input.length(); i++) {
                contents[i] = input[i];
            }

        }

        /**
         * Creates a new array of length `initialLen`. All indices are initialized to (char)0.
         * 
         * `initialLen` cannot be negative.
         */
        mut_array(long long initialLen) {
            if(initialLen < 0) {
                throw assertion_error("Initial array length cannot be negative");
            }

            contents = (char*) malloc(initialLen * sizeof(char));
            for(long long i=0LL; i<initialLen; i++) {
                contents[i] = (char)0;
            }

            nElements = initialLen;
        }

        /**
         * Returns element `index` of the array as an integer value.
         * 
         * `index` must be less than the array's length.
         */
        int pos(long long index) {
            if(index<0 || index>nElements) {
                throw assertion_error("Array index is out of range");
            }

            return (int)contents[index];
        }

        /**
         * Returns the number of elements in the array
         */
        long long length() {
            return nElements;
        }

        /**
         * Sets element `index` to `newValue`.
         * 
         * `index` must be less than the array's length.
         */
        void set(long long index, char newValue) {
            if(index<0 || index>nElements) {
                throw assertion_error("Array index is out of range");
            }

            contents[index] = newValue;
        }

        /**
         * Converts all English ASCII letters to their numeric values.
         * 
         * Numeric values: a=0, b=1, c=2... z=25. A=0, B=1, C=2... Z=25
         * All other characters are ignored.
         */
        void lettersToNumValue() {
            for(long long i=0LL; i<nElements; i++) {
                if(contents[i]>=65 && contents[i]<=90) {
                    contents[i] -= 65;
                }
                else if(contents[i]>=97 && contents[i]<=122) {
                    contents[i] -= 97;
                }
            }
        }

        /**
         * Converts all numeric values to their corresponding English ASCII letters.
         * 
         * Numeric values: a=0, b=1, c=2... z=25.
         * All other characters are ignored.
         * Letters will always be lowercase.
         */
        void numValueToLetters() {
            for(long long i=0LL; i<nElements; i++) {
                if(contents[i]>=0 && contents[i]<=25) {
                    contents[i] += 97;
                }
            }
        }

        /**
         * Adds `newElem` to the end of the array.
         */
        void append(char newElem) {
            //Increase the length
            nElements++;

            //Make new array
            char* newContents = (char*)malloc(nElements * sizeof(char));

            //Fill the new array
            for(long long i=0LL; i<nElements-1; i++) {
                newContents[i] = contents[i];
            }
            newContents[nElements-1] = newElem;

            //Remove the pointer to the old array
            free(contents);

            //Reassign the array
            contents = newContents;
        }

        /**
         * Adds all elements of `newElems` to the end of the array.
         */
        void append(string newElems) {
            //Reassigns the length at the end for safety reasons.

            //Make new array
            char* newContents = (char*)malloc((nElements+newElems.length()) * sizeof(char));

            //Copy existing elements to the new array
            for(long long i=0LL; i<nElements; i++) {
                newContents[i] = contents[i];
            }
            //Copy new elements to the new array
            for(long long i=nElements; i<nElements+newElems.length(); i++) {
                newContents[i] = newElems[i];
            }

            //Remove the pointer to the old array
            free(contents);

            //Reassign the array
            contents = newContents;

            //Increase element count
            nElements += newElems.length();
        }

        /**
         * Removes `endElemCount` elements from the end of the array.
         * 
         * `endElemCount` cannot be negative and cannot exceed the current length of the array.
         */
        void remove(long long endElemCount) {
            if(endElemCount<0) {
                throw assertion_error("Cannot remove a negative number of elements");
            }
            if(endElemCount>nElements) {
                throw assertion_error("Cannot remove more elements than the array's current capacity");
            }

            //Decrease the length
            nElements -= endElemCount;

            //Make new array
            char* newContents = (char*)malloc(nElements * sizeof(char));

            //Fill the new array
            for(long long i=0LL; i<nElements; i++) {
                newContents[i] = contents[i];
            }

            //Remove the pointer to the old array
            free(contents);

            //Reassign the array
            contents = newContents;
        }

        /**
         * Removes all the elements in the array.
         */
        void removeAll() {
            nElements = 0;

            char* newContents = (char*)malloc(0);

            //Remove the pointer to the old array
            free(contents);

            //Reassign the array
            contents = newContents;
        }

        /**
         * Properly removes an array.
         */
        ~mut_array() {
            free(contents);
            nElements = 0;
        }
};



/**
 * Prompts user to press ENTER or RETURN, then returns `returnCode`.
 * 
 * Use only for exiting the program.
 */
int promptExit(int returnCode) {
    cout << "Press ENTER or RETURN to exit.";

    string s;
    getline(cin, s);

    return returnCode;
}