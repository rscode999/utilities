#include <iostream>
#include <string>
#include "char_block_array.cpp"
using namespace std;

int main() {
    //Make a block array
    char_block_array arr = char_block_array("ABCDEFGHIJKL", 5);
    arr.printCharContents();
    cout << endl;
    
    //Append characters to it
    for(int i=0; i<10; i++) {
        arr.append('a');
    }
    arr.printCharContents();
    cout << endl;
    
    //Append a string to it
    arr.append("HELLO_WORLD");
    arr.printCurrentDimensions();
    arr.printCharContents();
    arr.printValueContents();
    cout << endl;

    //Remove a character
    arr.remove(1);
    arr.printCurrentDimensions();
    arr.printCharContents();
    cout << endl;

    //Remove zero characters
    arr.remove(0);
    arr.printCurrentDimensions();
    arr.printCharContents();
    cout << endl;

    //Remove a lot of characters
    arr.remove(17);
    arr.printCurrentDimensions();
    arr.printCharContents();
    cout << endl;

    //Remove a character
    arr.remove(1);
    arr.printCurrentDimensions();
    arr.printCharContents();
    cout << endl;

    //Remove everything
    arr.removeAll();
    arr.printCurrentDimensions();
    arr.printCharContents();
    cout << endl;

    //Add stuff back
    arr.append("abcdefghijklmnop");
    arr.printCurrentDimensions();
    arr.printCharContents();
    cout << endl;

    //Remove the stuff
    arr.remove(arr.totalElementCount());
    arr.printCurrentDimensions();
    arr.printCharContents();
    cout << endl;


    //Make another array
    char_block_array arr2 = char_block_array(5, 3);
    arr2.printCurrentDimensions();
    arr2.printValueContents();
    cout << endl;
}