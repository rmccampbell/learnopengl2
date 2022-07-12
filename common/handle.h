#ifndef HANDLE_H
#define HANDLE_H

#include <utility>

#ifdef _MSC_VER
#define NO_UNIQUE_ADDRESS [[msvc::no_unique_address]]
#else
#define NO_UNIQUE_ADDRESS [[no_unique_address]]
#endif

// Easier deleter definitions:
// using MyHandle = Handle<int, functor<delete_function>>
// Works for normal functions and global function pointers
template <auto& F>
struct functor {
    auto operator()(auto... args) { return F(args...); }
};

// For glDeleteBuffers and friends that take a pointer and size
// using MyHandle = Handle<int, gl_delete_array_functor<glDeleteBuffers>>
template <auto& F>
struct gl_delete_array_functor {
    void operator()(auto x) { F(1, &x); }
};

// Generic RAII handle type with custom deleter
// Can be used for shaders, buffers, textures, etc.
// Treats T() as a null/empty handle
// Probably not exception safe
template <typename T, typename D>
class Handle {
  public:
    // Construct from handle type and optional deleter (only needed for stateful deleters)
    Handle(T handle = T(), D deleter = D())
        : handle_(std::move(handle)), deleter_(std::move(deleter)) {}
    // Deletes the underlying handle
    ~Handle() { reset(); }
    // Move construct from another handle
    Handle(Handle&& other)
        : handle_(other.release()), deleter_(std::move(other.deleter_)) {}
    // Move assign from another handle
    Handle& operator=(Handle&& other) {
        if (this != &other) {
            reset(other.release());
            deleter_ = std::move(other.deleter_);
        }
        return *this;
    }

    // Get the raw handle
    const T& get() const { return handle_; }
    // Implicit conversion to raw handle
    operator const T&() const& { return handle_; }
    // Rvalue version deleted to prevent auto-conversion from temporaries, which would
    // result in "dangling" handles
    operator const T&() && = delete;
    // Get the deleter
    const D& get_deleter() const { return deleter_; }
    // Deletes the stored handle, optionally replacing it with a new handle
    void reset(T new_handle = T()) {
        if (handle_ != T()) {
            deleter_(handle_);
        }
        handle_ = std::move(new_handle);
    }
    // Resets and returns a mutable reference to the raw handle, for use as an out param
    T& reset_as_ref() {
        reset();
        return handle_;
    }
    // Relinquishes ownership of the raw handle and returns it, replacing it with an empty
    // handle
    [[nodiscard]] T release() {
        T temp = std::move(handle_);
        handle_ = T();
        return temp;
    }

  private:
    T handle_;
    NO_UNIQUE_ADDRESS D deleter_;
};

#endif // HANDLE_H
