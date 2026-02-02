#ifndef POINTERWRAPPER_H
#define POINTERWRAPPER_H

#include <utility>
#include <iostream>

/**
 * PointerWrapper - A template class that wraps a raw pointer
 * 
 */
template<typename T>
class PointerWrapper {
private:
    T* ptr;  // Raw pointer to the managed object

public:
    // ========== CONSTRUCTION AND DESTRUCTION ==========

    PointerWrapper() : ptr(nullptr) {}

    explicit PointerWrapper(T* p) : ptr(p) {}

    ~PointerWrapper() {
        delete ptr;
    };


    PointerWrapper(const PointerWrapper& other) = delete;

    PointerWrapper& operator=(const PointerWrapper& other) = delete;

    PointerWrapper(PointerWrapper&& other) noexcept : ptr(nullptr) {
        ptr = other.release();
    }

    PointerWrapper& operator=(PointerWrapper&& other) noexcept {
        if (this != &other) {
            reset(other.release());
        }
        return *this;
    }

    // ========== ACCESS OPERATIONS ==========

    T& operator*() const {
        if (!ptr) {
            throw std::runtime_error("ptr is null");
        }
        return *ptr;
    };

    T* operator->() const {
        if (ptr != nullptr) {
            return ptr;
        }
        return nullptr;
    }

    T* get() const {
        if (!ptr) {
            throw std::runtime_error("ptr is null");
        }
        return ptr; // Placeholder
    }

    T* release() {
        T* temp = ptr;
        ptr = nullptr;
        return temp;
    }

    void reset(T* new_ptr = nullptr) {
        delete ptr;
        ptr = new_ptr;
    }

    explicit operator bool() const {
        return ptr != nullptr; //placeholder
    }

    void swap(PointerWrapper& other) noexcept {
        std::swap(ptr, other.ptr);
    }
};


template<typename T, typename... Args>
PointerWrapper<T> make_pointer_wrapper(Args&&... args) {
    return PointerWrapper<T>(new T(std::forward<Args>(args)...));
}


template<typename T>
void swap(PointerWrapper<T>& lhs, PointerWrapper<T>& rhs) noexcept {
    T* temp = lhs.ptr;
    lhs.ptr = rhs.ptr;
    rhs.ptr = temp;
}

#endif // POINTERWRAPPER_H