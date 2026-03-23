#pragma once

#include <vector>
#include <cstddef>
#include <cstring>
#include <stdexcept>

/**
 * @brief Polymorphic byte buffer for serialization and deserialization.
 *
 * DataBuffer stores any trivially-copyable object as raw bytes using operator<<,
 * and reads them back in order using operator>>.
 *
 * Usage model:
 * - Write objects in any order with <<. Each object is appended as raw bytes.
 * - Read objects back with >> in the exact same order and types as written.
 * - The buffer has no type information — reading in wrong order yields corrupt data.
 *
 * Read cursor:
 * - A read cursor tracks the current read position.
 * - operator>> advances the cursor by sizeof(T) on each call.
 * - resetRead() moves the cursor back to the start without clearing data.
 * - clear() wipes all data and resets the cursor.
 *
 * Typical use cases:
 * - Game state serialization (save/load)
 * - Network packet construction and parsing
 *
 * Notes:
 * - operator>> throws std::out_of_range if not enough bytes remain.
 * - Reading past the end is always detected and throws — no silent corruption.
 * - The caller is responsible for read/write order consistency.
 */
class DataBuffer {
	private:
		std::vector<std::byte> data_;
		std::size_t offset_ = 0;

	public:
		DataBuffer() = default;

		/**
		 * @brief Clears all data and resets the read cursor to zero.
		 */
		void clear() {
			data_.clear();
			offset_ = 0;
		}

		/**
		 * @brief Resets the read cursor to the start without clearing data.
		 *
		 * Allows re-reading the buffer from the beginning.
		 */
		void resetRead() {
			offset_ = 0;
		}

		/**
		 * @brief Returns the number of bytes currently stored in the buffer.
		 */
		size_t size() const {
			return data_.size();
		}

		/**
		 * @brief Returns a raw pointer to the underlying byte array.
		 *
		 * Useful for sending raw bytes over a network socket or writing to a file.
		 */
		std::byte* data() {
			return data_.data();
		}

		const std::byte* data() const {
			return data_.data();
		}

		/**
		 * @brief Serializes a value into the buffer by appending its raw bytes.
		 *
		 * @tparam T Any trivially-copyable type.
		 * @param value The value to serialize.
		 * @return *this for chaining.
		 */
		template<typename T>
		DataBuffer& operator<<(const T& value) {
			const std::byte* ptr = reinterpret_cast<const std::byte*>(&value);
			data_.insert(data_.end(), ptr, ptr + sizeof(T));
			return *this;
		}

		/**
		 * @brief Deserializes the next value from the buffer at the read cursor.
		 *
		 * Reads sizeof(T) bytes from the current cursor position into value,
		 * then advances the cursor.
		 *
		 * @tparam T Must match the type originally written at this position.
		 * @param value Output parameter to read into.
		 * @return *this for chaining.
		 *
		 * @throws std::out_of_range if not enough bytes remain in the buffer.
		 */
		template<typename T>
		DataBuffer& operator>>(T& value) {
			if (offset_ + sizeof(T) > data_.size()) {
				throw std::out_of_range("Not enough data in buffer");
			}
			std::memcpy(&value, data_.data() + offset_, sizeof(T));
			offset_ += sizeof(T);
			return *this;
		}

};

