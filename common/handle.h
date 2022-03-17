#ifndef HANDLE_H
#define HANDLE_H

#include <utility>

#ifdef _MSC_VER
#define NO_UNIQUE_ADDRESS [[msvc::no_unique_address]]
#else
#define NO_UNIQUE_ADDRESS [[no_unique_address]]
#endif

// Easier deleter definitions:
// using MyHandle = Handle<int, FUNCTOR_T(delete_function)>
#define FUNCTOR(f) ([](auto... args) { return f(args...); })
#define FUNCTOR_T(f) decltype(FUNCTOR(f))

// Generic RAII handle type with custom deleter
// Can be used for shaders, buffers, textures, etc.
// Probably not exception safe
template <typename T, typename D>
class Handle {
  public:
    Handle(T handle = T(), D deleter = D())
        : handle_(std::move(handle)), deleter_(std::move(deleter)) {}
    ~Handle() { reset(); }

    Handle(Handle&& other)
        : handle_(other.release()), deleter_(std::move(other.deleter_)) {}
    Handle& operator=(Handle&& other) {
        if (this != &other) {
            reset(other.release());
            deleter_ = std::move(other.deleter_);
        }
        return *this;
    }

    const T& get() const { return handle_; }
    operator const T&() const& { return handle_; }
    // Prevent auto-conversion from temporary (which would result in accidentally
    // deleted handles)
    operator const T&() && = delete;
    const D& get_deleter() const { return deleter_; }

    void reset(T new_handle = T()) {
        if (handle_ != T()) {
            deleter_(handle_);
        }
        handle_ = std::move(new_handle);
    }
    T& reset_ref() {
        reset();
        return handle_;
    }

    T release() {
        T temp = std::move(handle_);
        handle_ = T();
        return temp;
    }

  private:
    T handle_;
    NO_UNIQUE_ADDRESS D deleter_;
};

#endif // HANDLE_H
