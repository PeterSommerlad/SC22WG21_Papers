#ifndef SRC__SCOPE_GUARD_COMMON_H_
#define SRC__SCOPE_GUARD_COMMON_H_
// Copyright(c) Eric Niebler 2016
// slightly adapted by Peter Sommerlad
#include <utility>

namespace std{
namespace experimental{
// contribution by (c) Eric Niebler 2016, slightly adapted by Peter Sommerlad
namespace detail {
namespace hidden{
template<typename T>
constexpr std::conditional_t<
    (!std::is_nothrow_move_assignable<T>::value &&
      std::is_copy_assignable<T>::value),
    T const &,
    T &&>
_move_assign_if_noexcept(T &x) noexcept
{
    return std::move(x);
}
}
template<typename T>
class _box
{
    T value;
    _box(T const &t) noexcept(noexcept(T(t)))
      : value(t)
    {}
    _box(T &&t) noexcept(noexcept(T(std::move_if_noexcept(t))))
      : value(std::move_if_noexcept(t))
    {}

public:
    template<typename TT,
        typename = std::enable_if_t<std::is_constructible<T, TT>::value>>
    explicit _box(TT &&t, auto &&guard) noexcept(noexcept(_box((T &&) t)))
      : _box((T &&) t)
    {
        guard.release();
    }
    T &get() noexcept
    {
        return value;
    }
    T const &get() const noexcept
    {
        return value;
    }
    T &&move() noexcept
    {
        return std::move(value);
    }
    void reset(T const &t) noexcept(noexcept(value = t))
    {
        value = t;
    }
    void reset(T &&t) noexcept(noexcept(value = hidden::_move_assign_if_noexcept(t)))
    {
        value = hidden::_move_assign_if_noexcept(t);
    }

};

template<typename T>
class _box<T &>
{
    std::reference_wrapper<T> value;
public:
    template<typename TT,
        typename = std::enable_if_t<std::is_convertible<TT, T &>::value>>
    _box(TT &&t, auto &&guard) noexcept(noexcept(static_cast<T &>((TT &&) t)))
      : value(static_cast<T &>(t))
    {
        guard.release();
    }
    T &get() const noexcept
    {
        return value.get();
    }
    T &move() const noexcept
    {
        return get();
    }
    void reset(T &t) noexcept
    {
        value = std::ref(t);
    }
};


template<bool B>
using _bool = std::integral_constant<bool, B>;


}}}


#endif /* SRC__SCOPE_GUARD_COMMON_H_ */
