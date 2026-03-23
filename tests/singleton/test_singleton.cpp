#include <iostream>
#include <cassert>
#include <stdexcept>
#include "design_patterns/singleton.hpp"

class AudioManager : public Singleton<AudioManager> {
    friend class Singleton<AudioManager>;
public:
    int volume_;
private:
    AudioManager(int volume) : volume_(volume) {}
};

class Logger : public Singleton<Logger> {
    friend class Singleton<Logger>;
public:
    std::string prefix_;
private:
    Logger(const std::string& prefix) : prefix_(prefix) {}
};

// Test: instantiate and access instance
void test_basic() {
    AudioManager::instantiate(80);
    assert(AudioManager::instance()->volume_ == 80);
    std::cout << "PASS test_basic\n";
}

// Test: instance() always returns the same object
void test_same_instance() {
    AudioManager* a = AudioManager::instance();
    AudioManager* b = AudioManager::instance();
    assert(a == b);
    std::cout << "PASS test_same_instance\n";
}

// Test: instantiate() throws if called twice
void test_double_instantiate_throws() {
    bool threw = false;
    try {
        AudioManager::instantiate(50);
    } catch (const std::runtime_error&) {
        threw = true;
    }
    assert(threw);
    std::cout << "PASS test_double_instantiate_throws\n";
}

// Test: instance() throws if not yet instantiated
void test_instance_before_instantiate_throws() {
    bool threw = false;
    try {
        Logger::instance();
    } catch (const std::runtime_error&) {
        threw = true;
    }
    assert(threw);
    std::cout << "PASS test_instance_before_instantiate_throws\n";
}

// Test: different types have independent instances
void test_independent_types() {
    Logger::instantiate("INFO");
    assert(Logger::instance()->prefix_ == "INFO");
    assert(AudioManager::instance()->volume_ == 80); // still intact
    std::cout << "PASS test_independent_types\n";
}

int main() {
    test_basic();
    test_same_instance();
    test_double_instantiate_throws();
    test_instance_before_instantiate_throws();
    test_independent_types();

    std::cout << "All Singleton tests passed.\n";
    return 0;
}
