#include <iostream>
#include <cassert>
#include <string>
#include "data_stuctures/pool.hpp"

// Test: move constructor transfers ownership
void test_move_constructor() {
    Pool<int> pool;

    auto a = pool.acquire(10);
    int* raw = &(*a);

    auto b = std::move(a); // a gives up ownership, b takes it

    assert(&(*b) == raw); // same slot
    assert(*b == 10);

    // a is now empty — pool_ is nullptr, won't double-free
    std::cout << "PASS test_move_constructor\n";
}

// Test: move assignment transfers ownership
void test_move_assignment() {
    Pool<int> pool;

    auto a = pool.acquire(1);
    auto b = pool.acquire(2);

    int* b_raw = &(*b);

    b = std::move(a); // b destroys its old object (slot 2), takes slot 1

    assert(*b == 1);
    assert(&(*b) != b_raw); // different slot now

    std::cout << "PASS test_move_assignment\n";
}

// Test: perfect forwarding — rvalue reaches constructor
void test_perfect_forwarding() {
    Pool<std::string> pool;

    std::string s = "hello";

    // lvalue — copy
    auto a = pool.acquire(s);
    assert(*a == "hello");
    assert(s == "hello"); // s still intact

    // rvalue — move
    auto b = pool.acquire(std::move(s));
    assert(*b == "hello");
    assert(s.empty()); // s was moved from

    std::cout << "PASS test_perfect_forwarding\n";
}

// Test: slot freed after move-from is reusable
void test_moved_from_slot_reused() {
    Pool<int> pool2;
    auto x = pool2.acquire(1);
    auto y = std::move(x);
    // x is now empty (pool_ == nullptr), y owns the slot
    assert(*y == 1);

    std::cout << "PASS test_moved_from_slot_reused\n";
}

int main() {
    test_move_constructor();
    test_move_assignment();
    test_perfect_forwarding();
    test_moved_from_slot_reused();

    std::cout << "All move tests passed.\n";
    return 0;
}
