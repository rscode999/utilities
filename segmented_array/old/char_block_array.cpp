#include <cassert>
#include <cstdlib>
#include <iostream>
#include <string>
using namespace std;

/*
Header file for a 2D array that can give its own size, change its size, and assert its own preconditions.

Chris P. Bacon
September 1, 2024
*/






//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//BLOCK ARRAY CLASS DEFINITION


/**
 * Acts like a 2D array. Data is divided into blocks of a set length.
 * 
 * The final block may hold less elements than the other blocks. If so, the final block holds the "remainder".
 * When counting blocks, the remainder doesn't count as a block.
 * 
 * Block and element indexing start at 0.
 * 
 * Example data arrangement:
 * [[1,2,3], (block 0)
 *  [4,5,6], (block 1)
 *  [7,8]]   (block 2 "remainder")
 * This block array has 2 full-length blocks and 3 elements per block. The length of its remainder is 2.
 * 
 * Uses a pointer to an array to store its contents.
 */
class char_block_array {

    private:

        /**
         * Number of full-length blocks in the array. The remainder does not count as a block.
         */
        int nBlocks;

        /**
         * Number of elements in each full-length block in the array
         * 
         * Should not be changed after the array's construction.
         */
        int nElemsPerBlock;

        /**
         * Number of elements in the remainder block
         */
        int nRemainderElems;

        /**
         * A pointer to the data stored in the array.
         * 
         * In memory, every `nElemsPerBlock` elements are one block.
         * How the blocks look in the array:
         * (block 0) 0, 1, 2, 3, 4, 5... (block 1) 0, 1, 2, 3, 4, 5,]... (block 2) 0, 1, 2, 3, 4, 5...
         */
        char* contents; 


        /**
         * Returns true if `blockNumber` and `elemNumber` are valid indices in the array, false otherwise
         * 
         * @param blockNumber block number to check
         * @param elemNumber element number to check
         * @return whether `blockNumber` and `elemNumber` are valid indices
         */
        bool isValidIndex(int blockNumber, int elemNumber) {
            //No remainder
            if(nRemainderElems==0) {
                if(blockNumber<0 || blockNumber>=nBlocks
                || elemNumber<0  || elemNumber>=nElemsPerBlock) {
                    return false;
                }
            }
            //Remainder present
            else {
                //Block number is valid
                if(blockNumber<0 || blockNumber>=nBlocks+1) {
                    return false;
                }

                //Element number is valid
                if(elemNumber<0 || 
                (blockNumber==nBlocks && elemNumber>=nRemainderElems) || //Check if in remainder
                (blockNumber<nBlocks && elemNumber>=nElemsPerBlock)) { //Check if not in remainder
                    return false;
                }
            }

            return true;
        }



    public:

        /**
         * Creates a new block array with `blocks` blocks and `elemsPerBlock` elements per block.
         * All elements are initialized to (char)0.
         * 
         * @param blocks number of blocks to start with in the array. Must be positive
         * @param elemsPerBlock number of elements per block to start with. Must be positive
         */
        char_block_array(int blocks, int elemsPerBlock) {
            assert(blocks>0 && elemsPerBlock>0);

            contents = (char*) malloc(blocks * elemsPerBlock * sizeof(char));
            for(int i=0; i<blocks*elemsPerBlock; i++) {
                contents[i] = (char)0;
            }

            nBlocks = blocks;
            nElemsPerBlock = elemsPerBlock;
            nRemainderElems = 0;
        }

        /**
         * Creates a new block array containing the elements of `initContents` split into pieces of size `elemsPerBlock`.
         * If `initContents.length()` is not evenly divisible by `elemsPerBlock`, the resulting array will have a remainder.
         * 
         * @param initContents contents to load the array with
         * @param elemsPerBlock number of elements per block to start with. Must be positive
         */
        char_block_array(string initContents, int elemsPerBlock) {
            assert(elemsPerBlock>0);

            contents = (char*) malloc(initContents.length() * sizeof(char));
            for(int i=0; i<initContents.length(); i++) {
                contents[i] = initContents[i];
            }

            nBlocks = initContents.length() / elemsPerBlock;
            nElemsPerBlock = elemsPerBlock;
            nRemainderElems = initContents.length() % elemsPerBlock;
        }

        ///////////////////////////////////////////////////////
        //GETTERS

        /**
         * Returns the number of full-sized blocks in the array.
         * In this method, the remainder does not count as a block.
         * 
         * @return number of blocks in the array (excludes the remainder)
         */
        int filledBlockCount() {
            return nBlocks;
        }

        /**
         * Returns the length of each full-sized block in the array
         * 
         * @return length of a filled block
         */
        int filledBlockLength() {
            return nElemsPerBlock;
        }

        /**
         * Returns the number of elements in the remainder
         * 
         * @return length of the remainder
         */
        int remainderLength() {
            return nRemainderElems;
        }

        /**
         * Returns the total number of elements in the array.
         * Total elements = number of blocks * number of elements per block + number of remainder elements
         * 
         * @return total number of elements
         */
        long long totalElementCount() {
            return (long long)nBlocks * (long long)nElemsPerBlock + (long long)nRemainderElems;
        }


        /**
         * Returns the element at block `blockNumber` and position `elemNumber` in the array.
         * The value returned is converted to an integer.
         * 
         * Block and element index numbers start at 0.
         * 
         * Note: if there is a remainder, the final block will have less than `blockLength()` elements.
         * 
         * @param blockNumber block to access. Must be a valid index in the array
         * @param elemNumber element to access. Must be a valid index in the array
         * @return value at (`blockNumber`, `elemNumber`) as an integer
         */
        int valueAt(int blockNumber, int elemNumber) {
            assert(isValidIndex(blockNumber, elemNumber));

            return (int)contents[nElemsPerBlock*blockNumber + elemNumber];
        }

        /**
         * Returns the element at block `blockNumber` and position `elemNumber` in the array.
         * The value returned is a character.
         * 
         * Block and element index numbers start at 0.
         * 
         * Note: if there is a remainder, the final block will have less than `blockLength()` elements.
         * 
         * @param blockNumber block to access. Must be a valid index in the array
         * @param elemNumber element to access. Must be a valid index in the array
         * @return value at (`blockNumber`, `elemNumber`) as a character
         */
        char charAt(int blockNumber, int elemNumber) {
            assert(isValidIndex(blockNumber, elemNumber));

            return contents[nElemsPerBlock*blockNumber + elemNumber];
        }

        ///////////////////////////////////////////////////////////////
        //SETTERS

        /**
         * Sets the element at block `blockNumber` and position `elemNumber` in the array to `newValue`.
         * 
         * Note: if there is a remainder, the final block will have less than `blockLength()` elements.
         * 
         * @param blockNumber block to change. Must be a valid index in the array
         * @param elemNumber element to change. Must be a valid index in the array
         * @param newValue value to set at (`blockNumber`, `elemNumber`)
         */
        void setValue(int blockNumber, int elemNumber, char newValue) {
            assert(isValidIndex(blockNumber, elemNumber));

            contents[nElemsPerBlock*blockNumber + elemNumber] = newValue;
        }

        /**
         * Adds `newValue` to the end of the last block of the array.
         * If the last block is filled, the method creates a new block and adds `newValue` to it.
         * 
         * @param newValue the new value to add to the last block
         */
        void append(char newValue) {
            
            //Make expanded array and copy current elements into it
            char* newContents = (char*) malloc((totalElementCount() + 1) * sizeof(char));
            for(int i=0; i<totalElementCount(); i++) {
                newContents[i] = contents[i];
            }
            //Add the new element
            newContents[totalElementCount()] = newValue;

            //Switch contents to new array
            free(contents);
            contents = newContents;

            //Update remainder and element counts
            nRemainderElems++;
            if(nRemainderElems == nElemsPerBlock) {
                nRemainderElems=0;
                nBlocks++;
            }
        }

        /**
         * Adds the elements of `newValues` to the end of the array.
         * Creates enough new blocks and elements to hold the new values.
         * 
         * @param newValues new elements to add to the last block(s)
         */
        void append(string newValues) {
            //Make expanded array and copy current elements into it
            char* newContents = (char*) malloc((totalElementCount() + newValues.length()) * sizeof(char));
            for(int i=0; i<totalElementCount(); i++) {
                newContents[i] = contents[i];
            }
            
            //Copy new values into the array
            for(int i=0; i<newValues.length(); i++) {
                newContents[totalElementCount() + i] = newValues[i];
            }

            //Switch contents to new array
            free(contents);
            contents = newContents;

            //Update remainder and elem. counts
            nBlocks = nBlocks + ((nRemainderElems + newValues.length()) / nElemsPerBlock);
            nRemainderElems = (nRemainderElems + newValues.length()) % nElemsPerBlock;
        }

        /**
         * Removes `endElementsToRemove` elements from the end of the array.
         * 
         * @param endElementsToRemove number of elements to remove. Must be non-negative. 
         * Cannot be greater than the total number of elements in the array.
         */
        void remove(int endElementsToRemove) {
            assert(endElementsToRemove>=0 && endElementsToRemove<=totalElementCount());

            //Make smaller array and copy current elements into it
            char* newContents = (char*) malloc((totalElementCount() - endElementsToRemove) * sizeof(char));
            for(int i=0; i<totalElementCount()-endElementsToRemove; i++) {
                newContents[i] = contents[i];
            } 

            //Switch contents to new array
            free(contents);
            contents = newContents;

            //Update counts
            nRemainderElems = (nRemainderElems - endElementsToRemove) % nElemsPerBlock;
            if(nRemainderElems < 0) {
                nRemainderElems += nElemsPerBlock;
            }

            nBlocks = nBlocks - ((nRemainderElems + endElementsToRemove) / nElemsPerBlock);
        }

        /**
         * Removes all elements from the array
         */
        void removeAll() {
            char* newContents = (char*) malloc(0 * sizeof(char));

            free(contents);
            contents = newContents;

            nBlocks=0;
            nRemainderElems=0;
        }


        //////////////////////////////////////////////////////////
        //UTILITIES

        
        /**
         * Prints the number of filled blocks, number of elements per block, and number of elements in the remainder, followed by a new line
         */
        void printCurrentDimensions() {
            cout << "filled blocks=" << nBlocks << ", elements per filled block=" << nElemsPerBlock << ", remainder size=" << nRemainderElems << endl;
        }

        /**
         * Prints each block on a new line separated by spaces. All values appear as integer values.
         */
        void printValueContents() {
            for(int i=0; i<filledBlockCount(); i++) {
                for(int j=0; j<filledBlockLength(); j++) {
                    cout << valueAt(i,j) << " ";
                }
                cout << endl;
            }
            for(int r=0; r<remainderLength(); r++) {
                cout << valueAt(filledBlockCount(), r) << " ";
            }
            cout << endl;
        }

        /**
         * Prints each block on a new line separated by spaces. All values appear as characters.
         */
        void printCharContents() {
            for(int i=0; i<filledBlockCount(); i++) {
                for(int j=0; j<filledBlockLength(); j++) {
                    cout << charAt(i,j) << " ";
                }
                cout << endl;
            }
            for(int r=0; r<remainderLength(); r++) {
                cout << charAt(filledBlockCount(), r) << " ";
            }
            cout << endl;
        }
        

        //////////////////////////////////////////////////////////
        //DESTRUCTOR

        /**
         * Properly destroys a block array.
         */
        ~char_block_array() {
            free(contents);
            nBlocks=0;
            nRemainderElems=0;
        }
};