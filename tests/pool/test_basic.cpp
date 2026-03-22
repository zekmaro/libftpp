#include <iostream>
#include <cassert>
#include "data_stuctures/pool.hpp"

// Simple struct to test construction/destruction
struct Foo {
    int value;
    bool* destroyed;

    Foo(int v, bool* d) : value(v), destroyed(d) {}
    ~Foo() { if (destroyed) *destroyed = true; }
};

// Test: acquire constructs the object with correct value
void test_acquire_constructs() {
    Pool<Foo> pool;
    bool destroyed = false;

    {
        auto obj = pool.acquire(42, &destroyed);
        assert(obj->value == 42);
        assert(destroyed == false);
    }
    // obj went out of scope — destructor should have fired
    assert(destroyed == true);

    std::cout << "PASS test_acquire_constructs\n";
}

// Test: slot is reused after release
void test_slot_reuse() {
    Pool<int> pool;

    int* first_ptr = nullptr;
    {
        auto obj = pool.acquire(1);
        first_ptr = &(*obj);
    }
    // slot released — next acquire should reuse it
    {
        auto obj = pool.acquire(2);
        assert(&(*obj) == first_ptr);
    }

    std::cout << "PASS test_slot_reuse\n";
}

// Test: multiple objects alive at the same time
void test_multiple_objects() {
    Pool<int> pool;

    auto a = pool.acquire(1);
    auto b = pool.acquire(2);
    auto c = pool.acquire(3);

    assert(*a == 1);
    assert(*b == 2);
    assert(*c == 3);

    std::cout << "PASS test_multiple_objects\n";
}

// Test: resize resets the pool
void test_resize() {
    Pool<int> pool;

    {
        auto obj = pool.acquire(99);
        assert(*obj == 99);
    }
    // all objects released — safe to resize
    pool.resize(10);

    auto obj = pool.acquire(7);
    assert(*obj == 7);

    std::cout << "PASS test_resize\n";
}

int main() {
    test_acquire_constructs();
    test_slot_reuse();
    test_multiple_objects();
    test_resize();

    std::cout << "All basic tests passed.\n";
    return 0;
}
