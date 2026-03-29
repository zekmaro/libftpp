#pragma once

#include <iostream>
#include <mutex>
#include <string>

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
		
	}
};
