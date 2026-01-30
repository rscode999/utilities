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



/**
 * Tests Rule of Three methods: copying constructor, assignment operator, destructor
 */
void test_copying() {
    using namespace std;
    using namespace utils;

    //Test copy constructor
    segmented_array<char> a1 = segmented_array<char>("abcdefg", 5);
    segmented_array<char> a2 = a1;
    cout << "a2 should equal a1's contents" << endl;
    cout << a2.to_string() << endl;

    //Test copying operator
    segmented_array<char> a3 = segmented_array<char>("ABC", 1);
    a1 = a3;
    cout << "a1 should now equal a3" << endl;
    cout << a1.to_string() << endl;

    //Test const copy
    const segmented_array<char> a_const = segmented_array<char>("qwertyuiop", 3);
    a1 = a_const;
    cout << "a1 should equal the const array" << endl;
    cout << a1.to_string() << endl;

    //Test deep copies
    a1 = segmented_array<char>("hello", 3);
    a2 = a1;
    a2.set_char_at(1, 1, '0');
    cout << "a1 and a2 should differ" << endl;
    cout << a1.to_string() << endl;
    cout << a2.to_string() << endl;

    //Test self assignment
    a1 = a1;
    cout << "a1 should not change" << endl;
    cout << a1.to_string() << endl;

    //Test reassignment
    a1 = segmented_array<char>("hello", 3);
    cout << "a1 should equal its newly assigned value" << endl;
    cout << a1.to_string() << endl;

    //Test assignment to empty
    segmented_array<char> a_empty = segmented_array<char>("", 2);
    a1 = a_empty;
    cout << "a1 should now be empty" << endl;
    cout << a1.to_string() << endl;

    //Test chaining
    segmented_array<char> a4 = segmented_array<char>("ABCDE", 3);
    a1 = a2 = a3 = a4;
    cout << "all should equal a4" << endl;
    cout << a1.to_string() << endl;
    cout << a2.to_string() << endl;
    cout << a3.to_string() << endl;
    cout << a4.to_string() << endl;
}


/**
 * Tests wide character's Rule of Three methods: copying constructor, assignment operator, destructor
 */
void test_copying_wchar() {
    using namespace std;
    using namespace utils;

    //Test copy constructor
    segmented_array<wchar_t> a1 = segmented_array<wchar_t>(L"abcdefg", 5);
    segmented_array<wchar_t> a2 = a1;
    wcout << "a2 should equal a1's contents" << endl;
    wcout << a2.to_string() << endl;

    //Test copying operator
    segmented_array<wchar_t> a3 = segmented_array<wchar_t>(L"ABC", 1);
    a1 = a3;
    wcout << "a1 should now equal a3" << endl;
    wcout << a1.to_string() << endl;

    //Test self assignment
    a1 = a1;
    wcout << "a1 should not change" << endl;
    wcout << a1.to_string() << endl;

    //Test reassignment
    a1 = segmented_array<wchar_t>(L"hello", 3);
    wcout << "a1 should equal its newly assigned value" << endl;
    wcout << a1.to_string() << endl;

    //Test assignment to empty
    segmented_array<wchar_t> a_empty = segmented_array<wchar_t>(L"", 2);
    a1 = a_empty;
    wcout << "a1 should now be empty" << endl;
    wcout << a1.to_string() << endl;

    //Test chaining
    segmented_array<wchar_t> a4 = segmented_array<wchar_t>(L"ABCDE", 3);
    a1 = a2 = a3 = a4;
    wcout << "all should equal a4" << endl;
    wcout << a1.to_string() << endl;
    wcout << a2.to_string() << endl;
    wcout << a3.to_string() << endl;
    wcout << a4.to_string() << endl;

    //Test const copy
    const segmented_array<wchar_t> a_const = segmented_array<wchar_t>(L"qwertyuiop", 3);
    a1 = a_const;
    wcout << "a1 should equal the const array" << endl;
    wcout << a1.to_string() << endl;

    //Test deep copies
    a1 = segmented_array<wchar_t>(L"hello", 3);
    a2 = a1;
    a2.set_char_at(1, 1, '0');
    wcout << "a1 and a2 should differ" << endl;
    wcout << a1.to_string() << endl;
    wcout << a2.to_string() << endl;
}



/**
 * Tests the equality operator.
 * 
 * Run after verifying the copying Rule of 3 methods 
 */
void test_equals_operator() {
    using namespace std;
    using namespace utils;

    //equal
    segmented_array<char> a1 = segmented_array<char>("abcdefghijklmnopqrstuvwxyz", 5);
    segmented_array<char> a2 = segmented_array<char>("abcdefghijklmnopqrstuvwxyz", 5);
    cout << "Expected: 1" << endl;
    cout << (a1 == a2) << endl;

    //different lengths
    a2 = segmented_array<char>("abcdefghijklmnopqrstuvwxy", 5);
    cout << "Expected: 0" << endl;
    cout << (a1 == a2) << endl;

    //different amount of elements per segment
    a2 = segmented_array<char>("abcdefghijklmnopqrstuvwxyz", 1);
    cout << "Expected: 0" << endl;
    cout << (a1 == a2) << endl;


    //one mismatched element
    a2 = segmented_array<char>("Abcdefghijklmnopqrstuvwxyz", 5);
    cout << "Expected: 0" << endl;
    cout << (a1 == a2) << endl;

    //                              *
    a2 = segmented_array<char>("abcdEfghijklmnopqrstuvwxyz", 5);
    cout << "Expected: 0" << endl;
    cout << (a1 == a2) << endl;

    //                                                   *
    a2 = segmented_array<char>("abcdefghijklmnopqrstuvwxyZ", 5);
    cout << "Expected: 0" << endl;
    cout << (a1 == a2) << endl;
}



int main() {
    using namespace std;
    using namespace utils;

    // test_copying();
    // test_copying_wchar();
    // test_equals_operator();
    // return 0;

    segmented_array<char> arr = segmented_array<char>("abcdefghijklmnopqrstuvwxyz", 4);
    string input;

    cout << "commands:" << "\n";
    cout << "  q:   quit\n";
    cout << "  a:   append\n";
    cout << "  r:   remove 'pop_back'\n";
    cout << "  rt:  remove and trim 'pop_back'\n";
    cout << "  s:   convert to string\n";
    cout << endl;
    

    while(true) {

        cout << "Contents:\n";
        for(int i=0; i<arr.filled_segment_count(); i++) {
            for(int j=0; j<arr.filled_segment_length(); j++) {
                cout << arr(i, j) << " ";
            }
            cout << "\n";
        }
        for(int i=0; i<arr.remainder_length(); i++) {
            cout << arr(arr.filled_segment_count(), i) << " ";
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
                arr += get_string("enter a character: ")[0];
            }
            else {
                arr += get_string("enter a string: ");
            }
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
        else if(input == "rt") {
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
            arr.pop_back(amount, true);
        }
        else if(input == "s") {
            cout << "As string: " << arr << endl;
        }
        else {
            cout << "invalid command" << endl;
        }
    }

    return 0;
}