#ifndef __ALLOCATOR_H__
#define __ALLOCATOR_H__

#include "Common.h"

template <typename T>
struct Alloc {
    typedef T value_type;

public:
    Alloc() = default;

    template <class U> constexpr Alloc(const Alloc<U>&) noexcept {}

    T* allocate(size_t n) {
        if(n > size_t(-1) / sizeof(T))
            throw bad_alloc();

        if(auto p = static_cast<T*>(malloc(n * sizeof(T))))
            return p;

        throw std::bad_alloc();
    }

    void deallocate(T* p, size_t) noexcept {
        free(p);
    }
};

template <class T, class U>
bool operator==(const Alloc<T>&, const Alloc<U>&) {
    return true;
}

template <class T, class U>
bool operator!=(const Alloc<T>&, const Alloc<U>&) {
    return false;
}

#endif  // __ALLOCATOR_H__
