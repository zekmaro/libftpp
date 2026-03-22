#pragma once

#include <vector>
#include <cstddef>
#include <cstring>
#include <stdexcept>

class DataBuffer {
	private:
		std::vector<std::byte> data_;
		std::size_t offset_ = 0;

	public:
		DataBuffer() = default;

		void clear() {
			data_.clear();
			offset_ = 0;
		}

		void resetRead() {
			offset_ = 0;
		}

		size_t size() const {
			return data_.size();
		}

		std::byte* data() {
			return data_.data();
		}

		const std::byte* data() const {
			return data_.data();
		}

		template<typename T>
		DataBuffer& operator<<(const T& value) {
			const std::byte* ptr = reinterpret_cast<const std::byte*>(&value);
			data_.insert(data_.end(), ptr, ptr + sizeof(T));
			return *this;
		}

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

