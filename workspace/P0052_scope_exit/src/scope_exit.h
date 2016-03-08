#ifndef SCOPE_EXIT_H_
#define SCOPE_EXIT_H_

#include <exception>
#include <utility>
#include <functional> // for reference wrappers
#include <limits> // for maxint
// modeled slightly after Andrescu's talk and article(s)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wterminate"

namespace std {
namespace experimental {
// new policy-based exception proof design by Eric Niebler
namespace detail{
// (c) copyright Eric Niebler, slightly adapted by Peter Sommerlad
struct on_exit_policy
{
    bool execute_ = true;

    void release() noexcept
    {
        execute_ = false;
    }

    bool should_execute() noexcept
    {
        return execute_;
    }
};

struct on_fail_policy
{
    int ec_ = std::uncaught_exceptions();

    void release() noexcept
    {
        ec_ = std::numeric_limits<int>::max();
    }

    bool should_execute() noexcept
    {
        return ec_ < std::uncaught_exceptions();
    }
};

struct on_success_policy
{
    int ec_ = std::uncaught_exceptions() ;

    void release() noexcept
    {
        ec_ = -1;
    }

    bool should_execute() noexcept
    {
        return ec_ >= std::uncaught_exceptions() ;
    }
};
}
template<class EF, class Policy = detail::on_exit_policy>
struct basic_scope_exit;

template<class EF>
using scope_exit = basic_scope_exit<EF, detail::on_exit_policy>;

template<class EF>
using scope_fail = basic_scope_exit<EF, detail::on_fail_policy>;

template<class EF>
using scope_success = basic_scope_exit<EF, detail::on_success_policy>;

namespace detail{
// DETAIL:
template<class Policy, class EF>
auto _make_guard(EF &&ef)
{
    return basic_scope_exit<std::decay_t<EF>, Policy>(std::forward<EF>(ef));
}
}
// Requires: EF is Callable
// Requires: EF is nothrow MoveConstructible OR CopyConstructible
template<class EF, class Policy /*= on_exit_policy*/>
class basic_scope_exit : private Policy
{
    EF ef_;
    template<typename T>
    explicit basic_scope_exit(T &&ef, Policy p)
    try:
        Policy(p), ef_(std::forward<T>(ef))
    {}
    catch(...)
    {
        auto &&guard = detail::_make_guard<Policy>(std::ref(ef));
        throw;
    }

public:

    explicit basic_scope_exit(EF const &ef)
    noexcept(std::is_nothrow_copy_constructible<EF>::value)
    : basic_scope_exit(ef, Policy{})
	  {}
	explicit basic_scope_exit(EF &&ef)
    noexcept(std::is_nothrow_move_constructible<EF>::value)
	: basic_scope_exit(std::move_if_noexcept(ef), Policy{})
	  {}
    basic_scope_exit(basic_scope_exit &&that)
        noexcept(std::is_nothrow_move_constructible<EF>::value)
      : basic_scope_exit(std::move_if_noexcept(that.ef_), (that.release(), that))
    {}
    ~basic_scope_exit()
    {
        if(this->should_execute())
            ef_();
    }

    basic_scope_exit(basic_scope_exit const &) = delete;
    basic_scope_exit &operator=(basic_scope_exit const &) = delete;
    basic_scope_exit &operator=(basic_scope_exit &&) = delete;
    using Policy::release;

};

template<class EF, class Policy>
void swap(basic_scope_exit<EF, Policy> &, basic_scope_exit<EF, Policy> &) = delete;

template<class EF>
auto make_scope_exit(EF &&ef)
noexcept(std::is_nothrow_constructible<std::decay_t<EF>, EF>::value)
{
    return detail::_make_guard<detail::on_exit_policy>(std::forward<EF>(ef));
}

template<class EF>
auto make_scope_fail(EF &&ef)
noexcept(std::is_nothrow_constructible<std::decay_t<EF>, EF>::value)
{
    return detail::_make_guard<detail::on_fail_policy>(std::forward<EF>(ef));
}

template<class EF>
auto make_scope_success(EF &&ef)
noexcept(std::is_nothrow_constructible<std::decay_t<EF>, EF>::value)
{
    return detail::_make_guard<detail::on_success_policy>(std::forward<EF>(ef));
}
#pragma GCC diagnostic pop

// end (c) Eric Niebler

#if 0
template<typename EF>
struct scope_exit {
	// construction
	explicit scope_exit(EF f) noexcept
	:exit_function {f}
	{
		static_assert(std::is_nothrow_copy_constructible<EF>{},"must be copyable");
	}
	// move ctor for factory
	scope_exit(scope_exit &&rhs) noexcept
			:exit_function {rhs.exit_function}
	,execute_on_destruction_flag {rhs.execute_on_destruction_flag}
	{
		rhs.release();
	}
	// release
	~scope_exit()
	{
		if (execute_on_destruction_flag)
			exit_function();
	}
	void release() noexcept {execute_on_destruction_flag=false;}

	scope_exit(scope_exit const &)=delete;
	scope_exit& operator=(scope_exit const &)=delete;
	scope_exit& operator=(scope_exit &&)=delete;
private:
	EF exit_function; // exposition only
	bool execute_on_destruction_flag {true}; // exposition only
};

// usage: auto guard=make_scope_exit([]{ std::cout << "done\n";});
template<typename EF>
auto make_scope_exit(EF exit_function) noexcept {
	return scope_exit<EF>(exit_function);
}
template<typename EF>
auto make_scope_fail(EF exit_function) noexcept {
	return make_scope_exit([=,ec=std::uncaught_exceptions()](){if (ec < uncaught_exceptions()) exit_function();});
}

template<typename EF>
auto make_scope_success(EF exit_function) noexcept {
	return make_scope_exit([=,ec=std::uncaught_exceptions()](){if (!(ec < uncaught_exceptions())) exit_function();});
}
#endif
}
}

#endif /* SCOPE_EXIT_H_ */
