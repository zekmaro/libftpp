#include <iostream>
#include <cassert>
#include <stdexcept>
#include "data_stuctures/data_buffer.hpp"

// Test: single value round-trips correctly
void test_single_value() {
    DataBuffer buf;

    int written = 42;
    buf << written;

    int read = 0;
    buf >> read;

    assert(read == written);
    std::cout << "PASS test_single_value\n";
}

// Test: multiple values in order
void test_multiple_values() {
    DataBuffer buf;

    int a = 10;
    float b = 3.14f;
    double c = 2.718;

    buf << a << b << c;

    int ra;
    float rb;
    double rc;

    buf >> ra >> rb >> rc;

    assert(ra == a);
    assert(rb == b);
    assert(rc == c);
    std::cout << "PASS test_multiple_values\n";
}

// Test: read past end throws
void test_read_past_end() {
    DataBuffer buf;

    int x = 1;
    buf << x;

    int a;
    buf >> a; // consume the data

    bool threw = false;
    try {
        int b;
        buf >> b; // nothing left
    } catch (const std::out_of_range&) {
        threw = true;
    }

    assert(threw);
    std::cout << "PASS test_read_past_end\n";
}

// Test: clear resets both data and read cursor
void test_clear() {
    DataBuffer buf;

    buf << 99;
    buf.clear();

    assert(buf.size() == 0);

    buf << 7;

    int x;
    buf >> x;
    assert(x == 7);

    std::cout << "PASS test_clear\n";
}

// Test: resetRead lets you re-read from the start
void test_reset_read() {
    DataBuffer buf;

    buf << 42;

    int a, b;
    buf >> a;
    buf.resetRead();
    buf >> b;

    assert(a == 42);
    assert(b == 42);
    std::cout << "PASS test_reset_read\n";
}

int main() {
    test_single_value();
    test_multiple_values();
    test_read_past_end();
    test_clear();
    test_reset_read();

    std::cout << "All DataBuffer tests passed.\n";
    return 0;
}
