#include <iostream>
#include <cassert>
#include "data_stuctures/pool.hpp"

// Test: fill the pool completely then release all
void test_fill_and_drain() {
    const std::size_t N = 8;
    Pool<int> pool;
    pool.resize(N);

    std::vector<Pool<int>::Object> objects;
    objects.reserve(N);

    for (std::size_t i = 0; i < N; ++i)
        objects.push_back(pool.acquire(static_cast<int>(i)));

    for (std::size_t i = 0; i < N; ++i)
        assert(*objects[i] == static_cast<int>(i));

    objects.clear(); // all released

    // pool should be fully free again — acquire should work
    auto obj = pool.acquire(99);
    assert(*obj == 99);

    std::cout << "PASS test_fill_and_drain\n";
}

// Test: chaotic acquire/release pattern — free list handles gaps correctly
void test_chaotic_release() {
    Pool<int> pool;
    pool.resize(4);

    auto a = pool.acquire(0);
    auto b = pool.acquire(1);
    auto c = pool.acquire(2);
    auto d = pool.acquire(3);

    // release b and d — creates gaps
    b = std::move(d); // d's slot released, b now owns d's old slot... wait
    // simpler: just destroy them via scope
    {
        auto tmp_b = std::move(b); // b released at end of this scope
        auto tmp_d = std::move(d);
    }

    // a and c still alive, b and d slots free
    assert(*a == 0);
    assert(*c == 2);

    // acquire into freed slots
    auto e = pool.acquire(10);
    auto f = pool.acquire(11);

    assert(*e == 10);
    assert(*f == 11);

    std::cout << "PASS test_chaotic_release\n";
}

// Test: resize to smaller capacity then fill again
void test_resize_smaller() {
    Pool<int> pool;
    pool.resize(100);

    {
        auto obj = pool.acquire(1);
    }

    pool.resize(4);

    std::vector<Pool<int>::Object> objects;
    for (int i = 0; i < 4; ++i)
        objects.push_back(pool.acquire(i));

    for (int i = 0; i < 4; ++i)
        assert(*objects[i] == i);

    std::cout << "PASS test_resize_smaller\n";
}

// Test: operator-> and operator* both work
void test_operators() {
    struct Point { int x, y; };

    Pool<Point> pool;
    auto obj = pool.acquire(Point{3, 7});

    assert(obj->x == 3);
    assert(obj->y == 7);
    assert((*obj).x == 3);

    std::cout << "PASS test_operators\n";
}

int main() {
    test_fill_and_drain();
    test_chaotic_release();
    test_resize_smaller();
    test_operators();

    std::cout << "All edge tests passed.\n";
    return 0;
}
