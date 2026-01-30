#include <iostream>
#include <string>
#include <stdlib.h>
#include "mut_array.cpp"
using namespace std;


int main() {

    mut_array arr = mut_array(10);
    for(int i=0; i<arr.length(); i++) {
        cout << arr.pos(i) << " ";
    }
    cout << "Length: " << arr.length() << endl;


    //Set each element to its index
    for(int s=0; s<10; s++) {
        arr.set(s, s);
    }
    for(int i=0; i<arr.length(); i++) {
        cout << arr.pos(i) << " ";
    }
    cout << "Length: " << arr.length() << endl;  

    //Add 5 more elements
    for(int s=0; s<5; s++) {
        arr.append(127);
    }
    for(int i=0; i<arr.length(); i++) {
        cout << arr.pos(i) << " ";
    }
    cout << "Length: " << arr.length() << endl;
    
    
    //Remove 10 elements
    arr.remove(10);
    for(int i=0; i<arr.length(); i++) {
        cout << arr.pos(i) << " ";
    }
    cout << "Length: " << arr.length() << endl;

    //Remove all elements
    arr.removeAll();
    for(int i=0; i<arr.length(); i++) {
        cout << arr.pos(i) << " ";
    }
    cout << "Length: " << arr.length() << endl;

    //Add more elements to the array
    for(int s=0; s<5; s++) {
        arr.append(127);
    }
    for(int i=0; i<arr.length(); i++) {
        cout << arr.pos(i) << " ";
    }
    cout << "Length: " << arr.length() << endl;


    //Convert array to a string representation
    arr.removeAll();
    arr.append("ABCDEFGHIJKL");
    for(int i=0; i<arr.length(); i++) {
        cout << arr.pos(i) << " ";
    }
    cout << "Length: " << arr.length() << endl;


    return promptExit(0);
}