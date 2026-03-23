#pragma once

#include <functional>
#include <stdexcept>
#include <utility>
#include <map>
#include <set>

/**
 * @brief Manages state transitions and per-state actions.
 *
 * StateMachine<TState> tracks a current state and lets you define:
 * - Valid states via addState()
 * - Transition lambdas via addTransition() — run when switching states
 * - Action lambdas via addAction() — run when update() is called
 *
 * Usage:
 *   StateMachine<MyState> sm(MyState::Idle);  // initial state auto-registered
 *   sm.addState(MyState::Running);
 *   sm.addTransition(MyState::Idle, MyState::Running, []() { ... });
 *   sm.addAction(MyState::Running, []() { ... });
 *   sm.transitionTo(MyState::Running);  // runs transition lambda
 *   sm.update();                        // runs action for current state
 *
 * Throws std::invalid_argument if:
 * - addTransition() references an unregistered state
 * - addAction() references an unregistered state
 * - transitionTo() targets an unknown state or no transition is defined
 * - update() is called with no action defined for the current state
 *
 * Notes:
 * - The initial state passed to the constructor is automatically registered.
 * - Transition and action lambdas take no arguments and return void.
 * - TState must support operator< (enums and most value types do).
 */
template <typename TState>
class StateMachine {
private:
	std::map<std::pair<TState, TState>, std::function<void()>> transitions_;
	std::map<TState, std::function<void()>> actions_;
	std::set<TState> states_;
	TState currentState_;

public:
	StateMachine(const TState& initialState) : currentState_(initialState) {
		this->addState(initialState);
	}

	void addState(const TState& state) {
		states_.insert(state);
	}

	void addTransition(const TState& startState, const TState& endState, const std::function<void()>& lambda) {
		if (states_.find(startState) == states_.end() || states_.find(endState) == states_.end()) {
			throw std::invalid_argument("Both states must be added to the state machine before adding a transition.");
		}
		transitions_[{startState, endState}] = lambda;
	}

	void addAction(const TState& state, const std::function<void()>& lambda) {
		if (states_.find(state) == states_.end()) {
			throw std::invalid_argument("State must be added to the state machine before adding an action.");
		}
		actions_[state] = lambda;
	}

	void transitionTo(const TState& state) {
		if (states_.find(currentState_) == states_.end() || states_.find(state) == states_.end()) {
			throw std::invalid_argument("States must be added to the state machine before transitioning.");
		}
		auto transitionIt = transitions_.find({currentState_, state});
		if (transitionIt == transitions_.end()) {
			throw std::invalid_argument("No transition defined from current state to the target state.");
		}
		currentState_ = state;
		transitionIt->second();
	}
	
	void update() {
		auto actionIt = actions_.find(currentState_);
		if (actionIt == actions_.end()) {
			throw std::invalid_argument("No action defined for the current state.");
		}
		actionIt->second();
	}
};
