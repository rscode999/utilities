#include "segmented_array.cpp"
#include <iostream>

/**
 * Returns the user's input string from `cin` after printing `prompt`.
 * 
 * @param prompt the prompt for the user to input the text
 * @return user input from `cin`
 */
std::string get_string(const std::string& prompt) {
    using namespace std;
    cout << prompt << flush;
    string output;
    getline(cin, output);
    return output;
}


int main() {
    using namespace std;
    using namespace utils;

    segmented_array<char> arr = segmented_array<char>("abcdefghijklmnopqrstuvwxyz", 5);
    string input;

    cout << "commands:" << "\n";
    cout << "  q:  quit\n";
    cout << "  a:  append 'push_back'\n";
    cout << "  at: append and trim 'push_back'\n";
    cout << "  e:  extend capacity\n";
    cout << "  r:  remove 'pop_back'\n";
    cout << "  ra: remove all 'clear'\n";
    cout << "  rt: remove and trim 'pop_back'\n";
    cout << "  s:  set char value\n";
    cout << "  si: set int value\n";
    cout << "  t:  trim 'shrink_to_fit'\n";
    cout << endl;
    

    while(true) {

        cout << "Contents:\n";
        for(int i=0; i<arr.filled_segment_count(); i++) {
            for(int j=0; j<arr.filled_segment_length(); j++) {
                cout << arr.char_at(i, j) << " ";
            }
            cout << "\n";
        }
        for(int i=0; i<arr.remainder_length(); i++) {
            cout << arr.char_at(arr.filled_segment_count(), i) << " ";
        }


        cout << endl;
        cout << "Dimensions: " << arr.filled_segment_count() << ", " << arr.filled_segment_length() << ", " << arr.remainder_length() << endl;
        cout << "Capacity: " << arr.capacity() << "\n" << endl;



        input = get_string("enter a command: ");

        if(input == "q" || input == "clear") {
            break;
        }
        else if(input == "a") {

            if(get_string("single character (c) or string (s)? ") == "c") {
                arr.push_back(get_string("enter a character: ")[0]);
            }
            else {
                arr.push_back(get_string("enter a string: "));
            }
        }
        else if(input == "at") {

            if(get_string("single character (c) or string (s)? ") == "c") {
                arr.push_back(get_string("enter a character: ")[0], true);
            }
            else {
                arr.push_back(get_string("enter a string: "), true);
            }
        }
        else if(input == "e") {
            int extensionLength = 0;
            try {
                extensionLength = stoi(get_string("enter an extension length: "));
                if(extensionLength<0) {
                    throw invalid_argument("negative extension length");
                }
            }
            catch(invalid_argument& e) {
                cout << "invalid input\n" << endl;
                continue;
            }

            arr.extend_capacity(extensionLength);
        }
        else if(input == "r") {
            int amount = 0;
            try {
                amount = stoi(get_string("enter a number of elements to remove: "));
                if(amount<0) {
                    throw invalid_argument("cannot remove negative amount");
                }
            }
            catch(invalid_argument& e) {
                cout << "invalid input\n" << endl;
                continue;
            }
            arr.pop_back(amount);
        }
        else if(input == "ra") {
            arr.clear();
        }
        else if(input == "rt") {
            int amount = 0;
            try {
                amount = stoi(get_string("enter a number of elements to remove: "));
            }
            catch(invalid_argument& e) {
                cout << "invalid input\n" << endl;
                continue;
            }
            arr.pop_back(amount, true);
        }
        else if(input == "s") {
            int segmentNumber;
            int elemNumber;
            char c;
            try {
                segmentNumber = stoi(get_string("enter a segment number: "));
                elemNumber = stoi(get_string("enter an element number: "));
                c = get_string("enter a character: ")[0];
            }
            catch(invalid_argument& e) {
                cout << "invalid input\n" << endl;
                continue;
            }

            arr.set_char_at(segmentNumber, elemNumber, c);
        }
        else if(input == "si") {
            int segmentNumber;
            int elemNumber;
            int value;

            try {
                segmentNumber = stoi(get_string("enter a segment number: "));
                elemNumber = stoi(get_string("enter an element number: "));
                value = stoi(get_string("enter an integer: "));
            }
            catch(invalid_argument& e) {
                cout << "invalid input\n" << endl;
                continue;
            }

            arr.set_int_value_at(segmentNumber, elemNumber, value);
        }
        else if(input == "t") {
            arr.shrink_to_fit();
        }
        else {
            cout << "invalid command\n" << endl;
        }
    }
    
    return 0;
}