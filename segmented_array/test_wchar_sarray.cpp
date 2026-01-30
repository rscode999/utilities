#include "segmented_array.cpp"
#include <iostream>

/**
 * Returns the user's input string from `wcin` after printing `prompt`.
 * 
 * @param prompt the prompt for the user to input the text
 * @return user input from `wcin`
 */
std::wstring get_wstring(const std::wstring& prompt) {
    using namespace std;
    wcout << prompt << flush;
    wstring output;
    getline(wcin, output);
    return output;
}


int main() {
    using namespace std;
    using namespace utils;

    segmented_array<wchar_t> arr = segmented_array<wchar_t>(L"abcdefghijklmnopqrstuvwxyz", 5);
    wstring input;
    wcout << "Initial contents as a string: " << arr << endl;

    wcout << "commands:" << "\n";
    wcout << "  q:  quit\n";
    wcout << "  a:  append 'push_back'\n";
    wcout << "  at: append and trim 'push_back'\n";
    wcout << "  e:  extend capacity\n";
    wcout << "  r:  remove 'pop_back'\n";
    wcout << "  ra: remove all\n";
    wcout << "  rt: remove and trim 'pop_back'\n";
    wcout << "  s:  set char value\n";
    wcout << "  si: set int value\n";
    wcout << "  t:  trim 'shrink_to_fit'\n";
    wcout << endl;

    while(true) {

        wcout << "Contents:\n";
        for(int i=0; i<arr.filled_segment_count(); i++) {
            for(int j=0; j<arr.filled_segment_length(); j++) {
                wcout << arr.char_at(i, j) << " ";
            }
            wcout << "\n";
        }
        for(int i=0; i<arr.remainder_length(); i++) {
            wcout << arr.char_at(arr.filled_segment_count(), i) << " ";
        }


        wcout << endl;
        wcout << "Dimensions: " << arr.filled_segment_count() << ", " << arr.filled_segment_length() << ", " << arr.remainder_length() << endl;
        wcout << "Capacity: " << arr.capacity() << "\n" << endl;



        input = get_wstring(L"enter a command: ");

        if(input == L"q" || input == L"clear") {
            break;
        }
        else if(input == L"a") {

            if(get_wstring(L"single character (c) or string (s)? ") == L"c") {
                arr.push_back(get_wstring(L"enter a character: ")[0]);
            }
            else {
                arr.push_back(get_wstring(L"enter a string: "));
            }
        }
        else if(input == L"at") {

            if(get_wstring(L"single character (c) or string (s)? ") == L"c") {
                arr.push_back(get_wstring(L"enter a character: ")[0], true);
            }
            else {
                arr.push_back(get_wstring(L"enter a string: "), true);
            }
        }
        else if(input == L"e") {
            int extensionLength = 0;
            try {
                extensionLength = stoi(get_wstring(L"enter an extension length: "));
                if(extensionLength<0) {
                    throw invalid_argument("negative extension length");
                }
            }
            catch(invalid_argument& e) {
                wcout << "invalid input\n" << endl;
                continue;
            }

            arr.extend_capacity(extensionLength);
        }
        else if(input == L"r") {
            int amount = 0;
            try {
                amount = stoi(get_wstring(L"enter a number of elements to remove: "));
                if(amount<0) {
                    throw invalid_argument("cannot remove negative amount");
                }
            }
            catch(invalid_argument& e) {
                wcout << "invalid input\n" << endl;
                continue;
            }
            arr.pop_back(amount);
        }
        else if(input == L"ra") {
            arr.clear();
        }
        else if(input == L"rt") {
            int amount = 0;
            try {
                amount = stoi(get_wstring(L"enter a number of elements to remove: "));
            }
            catch(invalid_argument& e) {
                wcout << "invalid input\n" << endl;
                continue;
            }
            arr.pop_back(amount, true);
        }
        else if(input == L"s") {
            int segmentNumber;
            int elemNumber;
            char c;
            try {
                segmentNumber = stoi(get_wstring(L"enter a segment number: "));
                elemNumber = stoi(get_wstring(L"enter an element number: "));
                c = get_wstring(L"enter a character: ")[0];
            }
            catch(invalid_argument& e) {
                wcout << "invalid input\n" << endl;
                continue;
            }

            arr.set_char_at(segmentNumber, elemNumber, c);
        }
        else if(input == L"si") {
            int segmentNumber;
            int elemNumber;
            int value;

            try {
                segmentNumber = stoi(get_wstring(L"enter a segment number: "));
                elemNumber = stoi(get_wstring(L"enter an element number: "));
                value = stoi(get_wstring(L"enter an integer: "));
            }
            catch(invalid_argument& e) {
                wcout << "invalid input\n" << endl;
                continue;
            }

            arr.set_int_value_at(segmentNumber, elemNumber, value);
        }
        else if(input == L"t") {
            arr.shrink_to_fit();
        }
        else {
            wcout << "invalid command\n" << endl;
        }
    }

    return 0;
}