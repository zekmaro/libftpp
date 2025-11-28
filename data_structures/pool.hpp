#pragma once

#include <cstddef>
#include <type_traits>
#include <vector>

#define DEFAULT_CAPACITY 256


template <typename T>
class Pool {
    private:
        using Storage = std::aligned_storage_t<sizeof(T), alignof(T)>;

        std::vector<Storage>    buffer_;
        std::vector<bool>       usedSlots_;
        std::size_t             capacity_ = 0;

    private:
        void destroySlot(std::size_t index) {
            //
        }

    public:
        class Object {
            private:
                Pool* pool_;
                std::size_t index_;
                T* ptr_;
            
            public:
                Object(Pool* pool, std::size_t index, T* ptr)
                    : pool_(pool), index_(index), ptr_(ptr) {}
                
                ~Object() {
                    if (pool_) {
                        //
                    }
                }

				Object(Object&& other) noexcept
					: pool_(other.pool_),
					index_(other.index_),
					ptr_(other.ptr_)
				{
					other.pool_ = nullptr;
					other.ptr_ = nullptr;
				}
                Object& operator=(Object&& other) noexcept {
                    if (this != &other) {
                        if (pool_) {
                            pool_->destroySlot(index_);
                        }
                        pool_ = other.pool_;
                        ptr_ = other.ptr_;
                        index_ = other.index_;
    
                        other.pool_ = nullptr;
                        other.ptr_  = nullptr;
                        other.index_ = static_cast<std::size_t>(-1);
                    }
                    return *this;
                }

                Object(const Object&) = delete;
                Object& operator=(const Object&) = delete;


            T* operator->() { return ptr_; }
            T& operator*() { return *ptr_; }
        };
    
    public:
        Pool() {
            this->resize(DEFAULT_CAPACITY);
        }
        ~Pool();

        void resize(std::size_t count) {
            for (std::size_t i = 0; i < capacity_; i++) {
                if (usedSlots_[i] == true) {
                    reinterpret_cast<T*>(&buffer_[i])->~T();
                }
            }
            buffer_.resize(count);
            usedSlots_.assign(count, false);
            capacity_ = count;
        }

        template<typename ... Args>
        Object acquire(Args&& ... args) {
            std::size_t freeIndex = static_cast<std::size_t>(-1);
            for (std::size_t i = 0; i < capacity_; i++) {
                if (usedSlots_[i] == false) {
                    freeIndex = i;
                    break;
                }
            }
            if (freeIndex == static_cast<std::size_t>(-1)) {
                this->resize(capacity_ * 2);
                this->acquire(args...);
            }
            usedSlots_[freeIndex] = true;
            return Object(args...);
        }

};
