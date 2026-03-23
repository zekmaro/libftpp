#include <iostream>
#include <cassert>
#include <stdexcept>
#include "design_patterns/state_machine.hpp"

enum class State { Idle, Running, Paused };

// Test: basic transition and update
void test_basic() {
    StateMachine<State> sm(State::Idle);
    sm.addState(State::Running);

    bool transitioned = false;
    bool updated = false;

    sm.addTransition(State::Idle, State::Running, [&]() { transitioned = true; });
    sm.addAction(State::Running, [&]() { updated = true; });

    sm.transitionTo(State::Running);
    assert(transitioned);

    sm.update();
    assert(updated);

    std::cout << "PASS test_basic\n";
}

// Test: update runs action for current state
void test_update_runs_current_action() {
    StateMachine<State> sm(State::Idle);

    int count = 0;
    sm.addAction(State::Idle, [&]() { ++count; });

    sm.update();
    sm.update();
    sm.update();

    assert(count == 3);
    std::cout << "PASS test_update_runs_current_action\n";
}

// Test: transitionTo unknown state throws
void test_transition_to_unknown_state_throws() {
    StateMachine<State> sm(State::Idle);

    bool threw = false;
    try {
        sm.transitionTo(State::Running); // Running not registered
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);
    std::cout << "PASS test_transition_to_unknown_state_throws\n";
}

// Test: transitionTo with no defined transition throws
void test_missing_transition_throws() {
    StateMachine<State> sm(State::Idle);
    sm.addState(State::Running);
    // no addTransition call

    bool threw = false;
    try {
        sm.transitionTo(State::Running);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);
    std::cout << "PASS test_missing_transition_throws\n";
}

// Test: update with no action throws
void test_missing_action_throws() {
    StateMachine<State> sm(State::Idle);
    // no addAction for Idle

    bool threw = false;
    try {
        sm.update();
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);
    std::cout << "PASS test_missing_action_throws\n";
}

// Test: multiple transitions work correctly
void test_multiple_transitions() {
    StateMachine<State> sm(State::Idle);
    sm.addState(State::Running);
    sm.addState(State::Paused);

    std::string log;
    sm.addTransition(State::Idle,    State::Running, [&]() { log += "start "; });
    sm.addTransition(State::Running, State::Paused,  [&]() { log += "pause "; });
    sm.addTransition(State::Paused,  State::Running, [&]() { log += "resume "; });

    sm.transitionTo(State::Running);
    sm.transitionTo(State::Paused);
    sm.transitionTo(State::Running);

    assert(log == "start pause resume ");
    std::cout << "PASS test_multiple_transitions\n";
}

int main() {
    test_basic();
    test_update_runs_current_action();
    test_transition_to_unknown_state_throws();
    test_missing_transition_throws();
    test_missing_action_throws();
    test_multiple_transitions();

    std::cout << "All StateMachine tests passed.\n";
    return 0;
}
