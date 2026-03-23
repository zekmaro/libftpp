#pragma once

#include <vector>
#include <map>
#include <functional>

/**
 * @brief Event notification system — subscribe lambdas to events and notify them.
 *
 * Observer<TEvent> maps events of type TEvent to lists of callbacks.
 * Multiple callbacks can be subscribed to the same event.
 * When notify() is called, all callbacks for that event fire in subscription order.
 *
 * @tparam TEvent The event type. Can be any comparable type (string, enum, int, etc.)
 *               Must support operator< for use as a std::map key.
 *
 * Typical use cases:
 * - Game event system ("player_died", "enemy_spawned")
 * - UI event handling ("button_clicked", "value_changed")
 * - Decoupling systems that need to react to each other without direct dependencies
 *
 * Notes:
 * - Callbacks are called in the order they were subscribed.
 * - Notifying an event with no subscribers is a no-op.
 * - Callbacks take no arguments and return void.
 */
template <typename TEvent>
class Observer {

private:
	std::map<TEvent, std::vector<std::function<void()>>> listeners_;

public:
	Observer() = default;

	/**
	 * @brief Subscribes a callback to an event.
	 *
	 * The callback will be called every time notify() is called with this event.
	 * Multiple callbacks can be subscribed to the same event.
	 *
	 * @param event The event to subscribe to.
	 * @param lambda The callback to invoke when the event fires.
	 */
	void subscribe(const TEvent& event, const std::function<void()>& lambda) {
		listeners_[event].push_back(lambda);
	}

	/**
	 * @brief Fires all callbacks subscribed to an event.
	 *
	 * Calls each subscribed callback in subscription order.
	 * Does nothing if no callbacks are subscribed to this event.
	 *
	 * @param event The event to notify.
	 */
	void notify(const TEvent& event) {
		auto it = listeners_.find(event);
		if (it != listeners_.end()) {
			for (const auto& listener : it->second) {
				listener();
			}
		}
	}

};
