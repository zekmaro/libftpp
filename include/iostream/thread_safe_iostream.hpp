#pragma once

#include <iostream>
#include <mutex>
#include <string>
#include <sstream>

class ThreadSafeIOStream {
	std::string prefix_;
	std::ostringstream buffer_;
	static std::mutex mutex_;

public:
	ThreadSafeIOStream() = default;

	void setPrefix(const std::string& prefix) {
		prefix_ = prefix;
	}

	template<typename T>
	void prompt(const std::string& question, T& dest) {
		std::lock_guard<std::mutex> lock(mutex_);
		std::cout << prefix_ << question;
		std::cin >> dest;
	}

	template<typename T>
    ThreadSafeIOStream& operator<<(const T& value) {
		std::lock_guard<std::mutex> lock(mutex_);
		buffer_ << value;
		size_t new_line_pos_ = buffer_.str().find('\n');
		while (new_line_pos_ != std::string::npos) {
			std::string line = buffer_.str().substr(0, new_line_pos_);
			std::cout << prefix_ << line << "\n";
			std::string remaining = buffer_.str().substr(new_line_pos_ + 1);
			buffer_.str("");
			buffer_.seekp(0);
			buffer_ << remaining;
			new_line_pos_ = buffer_.str().find('\n');
		}
		return *this;
	}
};
