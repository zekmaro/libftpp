#include <iostream>
#include <cassert>
#include <string>
#include "design_patterns/observer.hpp"

// Test: subscribed callback fires on notify
void test_basic_notify() {
    Observer<std::string> obs;

    bool fired = false;
    obs.subscribe("event", [&fired]() { fired = true; });
    obs.notify("event");

    assert(fired);
    std::cout << "PASS test_basic_notify\n";
}

// Test: multiple callbacks fire in subscription order
void test_multiple_subscribers() {
    Observer<std::string> obs;

    int order = 0;
    int first = -1, second = -1;

    obs.subscribe("event", [&]() { first = order++; });
    obs.subscribe("event", [&]() { second = order++; });

    obs.notify("event");

    assert(first == 0);
    assert(second == 1);
    std::cout << "PASS test_multiple_subscribers\n";
}

// Test: notifying unknown event is a no-op
void test_notify_unknown_event() {
    Observer<std::string> obs;

    bool fired = false;
    obs.subscribe("event_a", [&fired]() { fired = true; });
    obs.notify("event_b"); // different event

    assert(!fired);
    std::cout << "PASS test_notify_unknown_event\n";
}

// Test: different events are independent
void test_event_independence() {
    Observer<std::string> obs;

    bool a_fired = false;
    bool b_fired = false;

    obs.subscribe("a", [&a_fired]() { a_fired = true; });
    obs.subscribe("b", [&b_fired]() { b_fired = true; });

    obs.notify("a");

    assert(a_fired);
    assert(!b_fired);
    std::cout << "PASS test_event_independence\n";
}

// Test: works with enum event types
void test_enum_event_type() {
    enum class GameEvent { PlayerDied, EnemySpawned };

    Observer<GameEvent> obs;

    bool died = false;
    obs.subscribe(GameEvent::PlayerDied, [&died]() { died = true; });
    obs.notify(GameEvent::PlayerDied);

    assert(died);
    std::cout << "PASS test_enum_event_type\n";
}

int main() {
    test_basic_notify();
    test_multiple_subscribers();
    test_notify_unknown_event();
    test_event_independence();
    test_enum_event_type();

    std::cout << "All Observer tests passed.\n";
    return 0;
}
