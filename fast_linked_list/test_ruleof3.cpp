#include <iostream>
#include "fast_linked_list.cpp"

using namespace std;
using namespace utils;


void test_longer_assign() {
    fast_linked_list<int> l1 = fast_linked_list<int>();
    l1.push_back(0);
    l1.push_back(1);

    cout << l1 << endl;
    cout << "size=" << l1.size() << endl;
    cout << "last accessed index=" << l1.last_accessed_index() << endl;
    cout << "last accessed value=" << l1.last_accessed_element() << endl;

    fast_linked_list<int> l2;
    l2.push_back(22);
    cout << "l2: " << l2 << endl;

    l2 = l1;
    cout << "Assignment: equals {0, 1}, size=2, last accessed index = 0, last accessed value = 0" << endl;

    cout << l2 << endl;
    cout << "size=" << l2.size() << endl;
    cout << "last accessed index=" << l2.last_accessed_index() << endl;
    cout << "last accessed value=" << l2.last_accessed_element() << endl;
}



void test_longer_assign_lastaccessed() {
    fast_linked_list<int> l1 = fast_linked_list<int>();
    l1.push_back(10);
    l1.push_back(20);
    l1.push_back(30);
    // l1.push_back(40);
    l1.at(1, true); //Set last accessed to index 1

    cout << l1 << endl;
    cout << "size=" << l1.size() << endl;
    cout << "last accessed index=" << l1.last_accessed_index() << endl;
    cout << "last accessed value=" << l1.last_accessed_element() << endl;

    fast_linked_list<int> l2;
    l2.push_back(69);

    l2 = l1;
    cout << "Assignment: equals {10, 20, 30}, size=3, last accessed index = 1, last accessed value = 20" << endl;

    cout << l2 << endl;
    cout << "size=" << l2.size() << endl;
    cout << "last accessed index=" << l2.last_accessed_index() << endl;
    cout << "last accessed value=" << l2.last_accessed_element() << endl;

    fast_linked_list<int> l3;
    l3 = l1;
    cout << "Assignment to currently empty list: equals {10, 20, 30}, size=3, last accessed index = 1, last accessed value = 20" << endl;

    cout << l3 << endl;
    cout << "size=" << l3.size() << endl;
    cout << "last accessed index=" << l3.last_accessed_index() << endl;
    cout << "last accessed value=" << l3.last_accessed_element() << endl;


    fast_linked_list<int> l1_extra;
    for (int i = 0; i < 100; i += 10) {
        l1_extra.push_back(i);
    }
    l1_extra.at(7);
    fast_linked_list<int> l2_extra;
    l2_extra.push_back(67);
    
    l2_extra = l1_extra;
    cout << "Assignment to longer list: equals {0, 10, 20... 90}, size=10, last accessed index = 7, last accessed value = 70" << endl;

    cout << l2_extra << endl;
    cout << "size=" << l2_extra.size() << endl;
    cout << "last accessed index=" << l2_extra.last_accessed_index() << endl;
    cout << "last accessed value=" << l2_extra.last_accessed_element() << endl;
}



void test_shorter_assign() {
    fast_linked_list<int> l1 = fast_linked_list<int>();
    l1.push_back(1);

    fast_linked_list<int> l2 = fast_linked_list<int>();
    l2.push_back(10);
    l2.push_back(20);
    l2.push_back(30);

    l2 = l1;
    cout << "Assignment to shorter list: equals {1}, size=1, last accessed index = 0, last accessed value = 1" << endl;

    cout << l2 << endl;
    cout << "size=" << l2.size() << endl;
    cout << "last accessed index=" << l2.last_accessed_index() << endl;
    cout << "last accessed value=" << l2.last_accessed_element() << endl;
}



void test_shorter_assign_lastaccessed() {
    fast_linked_list<int> l1 = fast_linked_list<int>();
    for (int i = 0; i < 100; i += 10) {
        l1.push_back(i);
    }
    l1.at(8);

    fast_linked_list<int> l2 = fast_linked_list<int>();
    for (int i = 0; i < 200; i += 10) {
        l2.push_back(i);
    }
    l2.at(10);

    l2 = l1;
    cout << "Assignment to shorter list: equals {0, 10, 20... 90}, size=10, last accessed index = 8, last accessed value = 80" << endl;

    cout << l2 << endl;
    cout << "size=" << l2.size() << endl;
    cout << "last accessed index=" << l2.last_accessed_index() << endl;
    cout << "last accessed value=" << l2.last_accessed_element() << endl;


    fast_linked_list<int> l3;
    l1 = l3;
    cout << "Assignment to empty list: equals {}, size=0, last accessed index and value should throw std::out_of_range" << endl;

    cout << l1 << endl;
    cout << "size=" << l1.size() << endl;

    try {
        cout << "test failed: " << l1.last_accessed_index() << endl;
        return;
    }
    catch(out_of_range& e) {
        cout << "last accessed index successfully threw std::out_of_range" << endl;
    }

    try {
        cout << "test failed: " << l1.last_accessed_element() << endl;
        return;
    }
    catch(out_of_range& e) {
        cout << "last accessed value successfully threw std::out_of_range" << endl;
    }
}



void test_self_assign() {
    fast_linked_list<int> l1 = fast_linked_list<int>();
    for (int i = 0; i < 100; i += 10) {
        l1.push_back(i);
    }
    l1.at(8);

    l1 = l1;
    cout << "Assignment to self: equals {0, 10, 20... 90}, size=10, last accessed index = 8, last accessed value = 80" << endl;

    cout << l1 << endl;
    cout << "size=" << l1.size() << endl;
    cout << "last accessed index=" << l1.last_accessed_index() << endl;
    cout << "last accessed value=" << l1.last_accessed_element() << endl;

}



void test_chain_assign() {
    fast_linked_list<int> l1 = fast_linked_list<int>();
    for (int i = 0; i < 100; i += 10) {
        l1.push_back(i);
    }
    l1.at(8);

    fast_linked_list<int> l2;

    fast_linked_list<int> l3;
    for (int i = 0; i < 200; i += 10) {
        l3.push_back(i);
    }
    l3.at(10);

    fast_linked_list<int> l4;
    for (int i = 0; i < 200; i += 10) {
        l4.push_back(i);
    }
    l4.at(3);

    l4 = l3 = l2 = l1;

    cout << "Chain assign: equals {0, 10, 20... 90}, size=10, last accessed index = 8, last accessed value = 80" << endl;

    cout << l1 << endl;
    cout << l2 << endl;
    cout << l3 << endl;
    cout << l4 << endl;
    cout << "size=" << l1.size() << endl;
    cout << "size=" << l2.size() << endl;
    cout << "size=" << l3.size() << endl;
    cout << "size=" << l4.size() << endl;
    cout << "last accessed index=" << l1.last_accessed_index() << endl;
    cout << "last accessed index=" << l2.last_accessed_index() << endl;
    cout << "last accessed index=" << l3.last_accessed_index() << endl;
    cout << "last accessed index=" << l4.last_accessed_index() << endl;
    cout << "last accessed value=" << l1.last_accessed_element() << endl;
    cout << "last accessed value=" << l2.last_accessed_element() << endl;
    cout << "last accessed value=" << l3.last_accessed_element() << endl;
    cout << "last accessed value=" << l4.last_accessed_element() << endl;
}



void test_assign_deep_copy() {
    fast_linked_list<int> l1;
    l1.push_back(10);
    l1.push_back(20);
    l1.push_back(30);

    fast_linked_list<int> l2;
    l2 = l1;
    l2.set(1, 100);

    cout << "Deep copy check: equals {10, 100, 30}, size=3, last accessed index = 1, last accessed value = 100" << endl;

    cout << l2 << endl;
    cout << "size=" << l2.size() << endl;
    cout << "last accessed index=" << l2.last_accessed_index() << endl;
    cout << "last accessed value=" << l2.last_accessed_element() << endl;
}


void test_copyconstructor() {
    fast_linked_list<int> l1;
    for (int i = 0; i < 100; i += 10) {
        l1.push_back(i);
    }
    l1.at(8);

    fast_linked_list<int> l2 = l1;
    cout << "Copy constructor: equals {0, 10, 20... 90}, size=10, last accessed index = 8, last accessed value = 80" << endl;

    cout << l2 << endl;
    cout << "size=" << l2.size() << endl;
    cout << "last accessed index=" << l2.last_accessed_index() << endl;
    cout << "last accessed value=" << l2.last_accessed_element() << endl;


    l1.pop_back();
    fast_linked_list<int> l3 = l1;
    cout << "Copy constructor: equals {0, 10, 20... 80}, size=9, last accessed index = 8, last accessed value = 80" << endl;

    cout << l3 << endl;
    cout << "size=" << l3.size() << endl;
    cout << "last accessed index=" << l3.last_accessed_index() << endl;
    cout << "last accessed value=" << l3.last_accessed_element() << endl;
}



void test_copyconstructor_one_empty() {
    
    fast_linked_list<int> one_elem;
    one_elem.push_back(10);

    fast_linked_list<int> one_elem_2 = one_elem;
    one_elem_2.set(0, 10000);
    cout << "Copy constructor on one element list: equals {10000}, size=1, last accessed index = 0, last accessed value = 10000" << endl;
    cout << one_elem_2 << endl;
    cout << "size=" << one_elem_2.size() << endl;
    cout << "last accessed index=" << one_elem_2.last_accessed_index() << endl;
    cout << "last accessed value=" << one_elem_2.last_accessed_element() << endl;
    cout << "Deep copy check, should equal 10: " << one_elem.at(0) << endl;

    fast_linked_list<int> empty;
    fast_linked_list<int> empty2 = empty;
    cout << "Copy constructor on empty list: equals {}, size=0, last accessed index and last accessed value cause std::out_of_range" << endl;
    cout << empty2 << endl;
    cout << "size=" << empty2.size() << endl;

    try {
        cout << "last accessed index failed: " << empty2.last_accessed_index() << endl;
    }
    catch(out_of_range& e) {
        cout << "last accessed index successfully threw out_of_range" << endl;
    }

    try {
        cout << "last accessed value failed" << empty2.last_accessed_element() << endl;
    }
    catch(out_of_range& e) {
        cout << "last accessed value successfully threw out_of_range" << endl;
    }

    empty.push_back(1);
    cout << "Deep copy check, should stlil be empty: " << empty2 << endl;
}


int main() {
    // test_longer_assign();
    // test_longer_assign_lastaccessed();
    // test_shorter_assign();
    // test_shorter_assign_lastaccessed();
    // test_self_assign();
    // test_chain_assign();
    // test_assign_deep_copy();
    // test_copyconstructor();
    // test_copyconstructor_one_empty();
}