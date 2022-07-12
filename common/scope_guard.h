#ifndef SCOPE_GUARD_H
#define SCOPE_GUARD_H

#include <utility>

template <typename F>
class ScopeGuard {
  public:
    explicit ScopeGuard(F exit_func = F()) : exit_func_(std::move(exit_func)) {}
    ~ScopeGuard() { exit_func_(); }
    ScopeGuard(const ScopeGuard&) = delete;
    ScopeGuard& operator=(const ScopeGuard&) = delete;
  private:
    F exit_func_;
};

#endif // SCOPE_GUARD_H
