#pragma once

#include "../data_stuctures/data_buffer.hpp"

/**
 * @brief Base class for objects that can save and restore their state.
 *
 * Memento is a mixin — inherit from it to give any class the ability to
 * serialize its state into a Snapshot (DataBuffer) and restore it later.
 *
 * Usage model:
 * - Inherit from Memento and implement _saveToSnapshot and _loadFromSnapshot.
 * - Call save() to capture current state — returns a Snapshot the caller owns.
 * - Call load() with a previously saved Snapshot to restore state.
 *
 * The inheriting class controls what gets saved:
 * - _saveToSnapshot: write fields into the snapshot with <<
 * - _loadFromSnapshot: read fields back in the same order with >>
 *
 * Typical use cases:
 * - Game save/load system
 * - Undo/redo functionality
 * - State checkpointing
 *
 * Notes:
 * - The caller owns the Snapshot and decides when to discard it.
 * - load() makes an internal copy — the caller's Snapshot is never modified.
 * - Reading in a different order or type than written yields corrupt data.
 * - The inheriting class must declare Memento as a friend to allow access
 *   to its private _saveToSnapshot and _loadFromSnapshot implementations.
 */
class Memento {
	public:
		using Snapshot = DataBuffer;

		Memento() = default;
		virtual ~Memento() = default;

		/**
		 * @brief Captures the current state of the object into a Snapshot.
		 *
		 * Calls _saveToSnapshot() on the inheriting class to fill the buffer.
		 *
		 * @return A Snapshot containing the serialized state.
		 */
		Snapshot save() {
			Snapshot snapshot;
			this->_saveToSnapshot(snapshot);
			return snapshot;
		}

		/**
		 * @brief Restores the object's state from a previously saved Snapshot.
		 *
		 * Makes an internal copy of the snapshot so the caller's copy is
		 * unmodified and can be reused for multiple loads.
		 *
		 * @param snapshot A Snapshot previously returned by save().
		 */
		void load(const Snapshot& snapshot) {
			Snapshot copy = snapshot;
			this->_loadFromSnapshot(copy);
		}

	private:
		/**
		 * @brief Serialize the object's state into the snapshot.
		 *
		 * Implemented by the inheriting class. Write all fields with <<
		 * in a consistent order.
		 */
		virtual void _saveToSnapshot(Snapshot& snapshot) const = 0;

		/**
		 * @brief Restore the object's state from the snapshot.
		 *
		 * Implemented by the inheriting class. Read all fields with >>
		 * in the exact same order as _saveToSnapshot.
		 */
		virtual void _loadFromSnapshot(Snapshot& snapshot) = 0;

};
