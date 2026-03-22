#pragma once

#include <type_traits>
#include <cstddef>
#include <cassert>
#include <utility> // std::forward c++11
#include <vector>
#include <limits>
#include <stack>

/**
 * @brief Fixed-size object pool with manual lifetime management.
 *
 * Pool<T> owns raw, aligned memory capable of storing up to N objects of type T.
 * Objects are constructed in-place using placement new and destroyed explicitly.
 *
 * Ownership model:
 * - The Pool owns all raw memory.
 * - Pool::Object is a move-only RAII handle that owns exactly one slot.
 * - Copying Pool::Object is forbidden.
 *
 * Lifetime rules:
 * - resize() destroys all live objects and invalidates all Pool::Object handles.
 * - acquire() constructs a new T in a free slot and returns a handle.
 * - When the handle is destroyed or moved-from, the slot is released.
 *
 * Invariants:
 * - buffer_.size() == usedSlots_.size()
 * - Each slot is either fully constructed or fully free.
 *
 * Notes:
 * - resize() is destructive and must not be called while handles exist.
 * - acquire() aborts if the pool is full.
 */
template <typename T>
class Pool {
    private:
        using Slot = std::aligned_storage_t<sizeof(T), alignof(T)>;

        static constexpr std::size_t    default_capacity = 256;
        std::size_t                     liveCount_ = 0;
        std::stack<std::size_t>         freeSlots_;
        std::vector<uint8_t>            usedSlots_;
        std::vector<Slot>               buffer_;

    private:
        /**
        * @brief Destroys the object in a given slot.
        *
        * Ends the lifetime of the object stored at @p index and marks the slot as free.
        * Does not deallocate memory.
        *
        * @pre index refers to a valid, used slot.
        */
        void destroySlot(std::size_t index) noexcept {
            assert(buffer_.size() == usedSlots_.size());
            assert(index < usedSlots_.size());
            assert(usedSlots_[index] && "Destroying an unused slot");

            T* ptr = reinterpret_cast<T*>(&buffer_[index]);
            ptr->~T();

            usedSlots_[index] = false;
            freeSlots_.push(index);
            --liveCount_;
        }

    public:
        /**
        * @brief RAII handle owning a single pool slot.
        *
        * Pool::Object represents exclusive ownership of one object stored in the pool.
        * Destroying the handle destroys the object and releases the slot.
        *
        * This type is move-only. Copying is forbidden.
        */
        class Object {
            private:
                Pool* pool_;
                std::size_t index_;
                T* ptr_;

            public:
                Object(Pool* pool, std::size_t index, T* ptr)
                    : pool_(pool), index_(index), ptr_(ptr) {}
                

                ~Object() noexcept {
                    if (pool_) {
                        pool_->destroySlot(index_);
                    }
                }

				Object(Object&& other) noexcept
					: pool_(other.pool_),
					index_(other.index_),
					ptr_(other.ptr_) {
					other.pool_ = nullptr;
					other.ptr_ = nullptr;
                    other.index_ = 0;
				}

                Object& operator=(Object&& other) noexcept {
                    if (this != &other) {
                        if (pool_ && ptr_) {
                            pool_->destroySlot(index_);
                        }
                        pool_ = other.pool_;
                        ptr_ = other.ptr_;
                        index_ = other.index_;
    
                        other.pool_ = nullptr;
                        other.ptr_  = nullptr;
                        other.index_ = 0;
                    }
                    return *this;
                }

                Object(const Object&) = delete;
                Object& operator=(const Object&) = delete;


            T* operator->() { return ptr_; }
            T& operator*() { return *ptr_; }
            T* operator->() const { return ptr_; }
            T& operator*() const { return *ptr_; }

        };
    
    public:
        Pool() {
            this->resize(default_capacity);
        }
        ~Pool() noexcept {
            assert(liveCount_ == 0 && "Pool destroyed while objects are still alive");
            resize(0);
        }

        /**
        * @brief Resizes the pool capacity.
        *
        * Allocates raw storage for @p count objects of type T.
        * All currently constructed objects are destroyed.
        *
        * @warning This operation is destructive.
        *          All existing Pool::Object handles become invalid.
        *
        * @param count Number of object slots to allocate.
        */
        void resize(std::size_t count) {
            assert(liveCount_ == 0 && "resize() called while objects are still alive");

            freeSlots_ = std::stack<std::size_t>();
            buffer_.resize(count);
            usedSlots_.assign(count, false);
            for (std::size_t i = 0; i < count; ++i)
                freeSlots_.push(i);
        }

        /**
        * @brief Constructs a new object in the pool.
        *
        * Finds a free slot, constructs T in-place using the provided arguments,
        * and returns a move-only RAII handle owning that slot.
        *
        * @tparam Args Constructor argument types.
        * @param args Arguments forwarded to T's constructor.
        *
        * @return Pool::Object owning the constructed object.
        *
        * @warning Aborts the program if the pool is full.
        */
        template<typename ... Args>
        Object acquire(Args&& ... args) {
            assert(buffer_.size() == usedSlots_.size());
            assert(!freeSlots_.empty() && "Pool is full");

            std::size_t freeIndex = freeSlots_.top();
            freeSlots_.pop();

            new (&buffer_[freeIndex]) T(std::forward<Args>(args)...);
            usedSlots_[freeIndex] = true;
            ++liveCount_;

            T* ptr = reinterpret_cast<T*>(&buffer_[freeIndex]);
            return Object(this, freeIndex, ptr);
        }

};
