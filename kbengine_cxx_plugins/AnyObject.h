#pragma once
#include <typeinfo>
#include <typeindex>
#include <utility>
#include <stdexcept>
#include <memory>

class bad_any_cast : public std::bad_cast {
public:
    const char* what() const noexcept override {
        return "bad_any_cast";
    }
};

class AnyObject {
    static const size_t SBO_SIZE = 32;
    static const size_t SBO_ALIGN = alignof(std::max_align_t);
    using Storage = typename std::aligned_storage<SBO_SIZE, SBO_ALIGN>::type;

private:
    struct VTable {
        void (*destroy)(void*);
        void (*move)(void*, void*);
        const std::type_info& (*type)();
        bool copyable;
        void (*copy)(const void*, void*);
    };

    template <typename T, bool CanCopy = std::is_copy_constructible<T>::value>
    struct VTableImpl;

    // -------- copyable types --------
    template <typename T>
    struct VTableImpl<T, true> {
        static void destroy(void* obj) {
            reinterpret_cast<T*>(obj)->~T();
        }

        static void move(void* src, void* dst) {
            new(dst) T(std::move(*reinterpret_cast<T*>(src)));
            destroy(src);
        }

        static void copy(const void* src, void* dst) {
            new(dst) T(*reinterpret_cast<const T*>(src));
        }

        static const std::type_info& type() {
            return typeid(T);
        }

        static const VTable table;
    };

    // -------- move-only types (unique_ptr<T>, etc) --------
    template <typename T>
    struct VTableImpl<T, false> {
        static void destroy(void* obj) {
            reinterpret_cast<T*>(obj)->~T();
        }

        static void move(void* src, void* dst) {
            new(dst) T(std::move(*reinterpret_cast<T*>(src)));
            destroy(src);
        }

        static const std::type_info& type() {
            return typeid(T);
        }

        // 注意：copy=nullptr, copyable=false，避免编译时实例化 copy path
        static const VTable table;
    };

    bool use_sbo = false;
    const VTable* vtable = nullptr;
    std::type_index type_index{typeid(void)};

    union {
        Storage sbo;
        void* heap_ptr;
    };

public:
    AnyObject() {}
    AnyObject(std::nullptr_t) {}

    template <typename T>
    AnyObject(T&& value) {
        emplace<typename std::decay<T>::type>(std::forward<T>(value));
    }

    AnyObject(const AnyObject& other) {
        copy_from(other);
    }

    AnyObject(AnyObject&& other) noexcept {
        move_from(std::move(other));
    }

    AnyObject& operator=(const AnyObject& other) {
        if (this != &other) {
            reset();
            copy_from(other);
        }
        return *this;
    }

    AnyObject& operator=(AnyObject&& other) noexcept {
        if (this != &other) {
            reset();
            move_from(std::move(other));
        }
        return *this;
    }

    ~AnyObject() {
        reset();
    }

    template <typename T, typename... Args>
    void emplace(Args&&... args) {
        reset();

        using U = typename std::decay<T>::type;
        vtable = &VTableImpl<U>::table;
        type_index = typeid(U);

        if (sizeof(U) <= SBO_SIZE && alignof(U) <= SBO_ALIGN) {
            use_sbo = true;
            new(&sbo) U(std::forward<Args>(args)...);
        } else {
            use_sbo = false;
            heap_ptr = ::operator new(sizeof(U));
            new(heap_ptr) U(std::forward<Args>(args)...);
        }
    }

    void reset() {
        if (!vtable) return;

        if (use_sbo) {
            vtable->destroy(&sbo);
        } else {
            vtable->destroy(heap_ptr);
            ::operator delete(heap_ptr);
        }

        vtable = nullptr;
        type_index = typeid(void);
        use_sbo = false;
    }

    template <typename T>
    T& get() {
        if (!is<T>()) throw bad_any_cast();
        return use_sbo ? *reinterpret_cast<T*>(&sbo)
                       : *reinterpret_cast<T*>(heap_ptr);
    }

    template <typename T>
    bool is() const {
        return type_index == std::type_index(typeid(T));
    }

    bool has_value() const { return vtable != nullptr; }

private:
    void copy_from(const AnyObject& other) {
        if (!other.vtable) return;

        if (!other.vtable->copyable)
            throw std::runtime_error("AnyObject: stored type is not copyable");

        vtable = other.vtable;
        type_index = other.type_index;
        use_sbo = other.use_sbo;

        if (use_sbo) {
            vtable->copy(&other.sbo, &sbo);
        } else {
            heap_ptr = ::operator new(sizeof_sbo(other));
            vtable->copy(other.heap_ptr, heap_ptr);
        }
    }

    size_t sizeof_sbo(const AnyObject& other) const {
        return other.use_sbo ? SBO_SIZE : sizeof(void*);
    }

    void move_from(AnyObject&& other) {
        if (!other.vtable) return;

        vtable = other.vtable;
        type_index = other.type_index;
        use_sbo = other.use_sbo;

        if (use_sbo) {
            vtable->move(&other.sbo, &sbo);
        } else {
            heap_ptr = other.heap_ptr;
            other.heap_ptr = nullptr;
        }

        other.vtable = nullptr;
        other.type_index = typeid(void);
        other.use_sbo = false;
    }
};


template <typename T>
const AnyObject::VTable AnyObject::VTableImpl<T, true>::table = {
    &destroy,
    &move,
    &type,
    true,
    &copy
};

template <typename T>
const AnyObject::VTable AnyObject::VTableImpl<T, false>::table = {
    &destroy,
    &move,
    &type,
    false,      // copyable = false
    nullptr     // copy = nullptr (avoid copy instantiation)
};
