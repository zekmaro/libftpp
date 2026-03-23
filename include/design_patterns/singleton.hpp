#pragma once

#include <stdexcept>
#include <utility>
#include <memory>

/**
 * @brief Ensures only one instance of TType can ever exist.
 *
 * Singleton<TType> is a mixin — inherit from it to make any class a singleton.
 * The inheriting class must declare Singleton<TType> as a friend and keep
 * its constructor private to prevent direct construction.
 *
 * Usage:
 * - Call instantiate(args...) once to create the instance.
 * - Call instance() anywhere to access it.
 * - Calling instantiate() twice throws.
 * - Calling instance() before instantiate() throws.
 *
 * Example:
 *   class AudioManager : public Singleton<AudioManager> {
 *       friend class Singleton<AudioManager>;
 *   private:
 *       AudioManager() {}
 *   };
 *
 *   AudioManager::instantiate();
 *   AudioManager::instance()->play();
 *
 * Notes:
 * - The instance is managed by a unique_ptr — automatically deleted at program exit, no leaks.
 * - Copy and move are deleted — only one instance can exist.
 */
template <typename TType>
class Singleton {
	protected:
		Singleton() = default;
		~Singleton() = default;

	private:
		inline static std::unique_ptr<TType> instance_ = nullptr;

	public:
		Singleton(const Singleton&) = delete;
		Singleton& operator=(const Singleton&) = delete;

		static TType* instance() {
			if (!instance_) {
				throw std::runtime_error("Singleton not instantiated");
			}
			return instance_.get();
		}

		template<typename... Args>
		static void instantiate(Args&&... p_args) {
			if (instance_) {
				throw std::runtime_error("Singleton instance already exists");
			}
			instance_ = std::unique_ptr<TType>(new TType(std::forward<Args>(p_args)...));
		}
};
