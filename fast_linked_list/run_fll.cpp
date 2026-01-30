#include "fast_linked_list.cpp"
#include <iostream>
#include <string>

/**
 * Prints `msg`, then returns the user's input from the standard input.
 * 
 * @param msg message to prompt the user with
 * @return user input from standard input
 */
std::string get_string(const std::string& msg) {
    using namespace std;
    cout << msg << flush;
    string output;
    getline(cin, output);
    return output;
}




/**
 * Prints the list, prompts the user for a command, and updates the list based on the user input
 */
int main() {
    using namespace std;
    using namespace utils;


    fast_linked_list<char> test_list = fast_linked_list<char>();

    /*
    commands:
    q:  quit
    ab: add back
    af: add front
    al: add last accessed
    g:  get ("at")
    l:  last accessed
    n:  get number of elements
    rb: remove back 
    rf: remove front
    rl: remove last accessed
    s:  set
    */


    while(true) {

        cout << "List: " << test_list << endl;

        string command = get_string("enter command: ");

        try {

            if(command=="q" || command=="clear") {
                break;
            } 

            else if(command == "ab") {
                char item = get_string("enter letter: ")[0];
                test_list.push_back(item);
            }

            else if (command == "af") {
                char item = get_string("enter letter: ")[0];
                test_list.push_front(item);
            }

            else if(command == "al") {
                char item = get_string("enter letter: ")[0];
                test_list.push_last_accessed(item);
            }

            else if(command == "g") {
                int index = stof(get_string("enter index: "));
                cout << "Value: " << test_list[index];
            }

            else if(command == "l") {
                cout << "Last accessed: " << test_list.last_accessed_element() << " at index " << test_list.last_accessed_index();
            }
            
            else if(command == "n") {
                cout << "Number of elements: " << test_list.size();
            }

            else if(command == "rb") {
                test_list.pop_back();
            }   

            else if(command == "rf") {
                test_list.pop_front();
            }

            else if(command == "rl") {
                test_list.pop_last_accessed();
            }

            else if(command == "s") {
                int index = stof(get_string("enter index: "));
                char item = get_string("enter letter: ")[0];
                test_list[index] = item;
            }

            else {
                cout << "Invalid command";
            }

        }
        catch(invalid_argument& e) {
            cout << "Invalid argument entered";
        }
        catch(out_of_range& e) {
            cout << e.what();
        }

        cout << "\n" << endl;
    }
    test_list[0] = 'a';
}