/*
 * FixedAllocator.h
 *
 *  Created on: 2 May 2018
 *      Author: sop
 */

#ifndef SRC_FIXEDALLOCATOR_H_
#define SRC_FIXEDALLOCATOR_H_

#include <array>

#include <cstddef>

#include <new>

template <class T>
struct fixed_allocator
{
    using value_type    = T;
	static constexpr size_t SIZE=1024;
	static inline std::array<value_type,SIZE> mem;
	static inline size_t current=0;
	static value_type* loc() { return mem.data();}
	static void  reset() {current=0;}

//     using pointer       = value_type*;
//     using const_pointer = typename std::pointer_traits<pointer>::template
//                                                     rebind<value_type const>;
//     using void_pointer       = typename std::pointer_traits<pointer>::template
//                                                           rebind<void>;
//     using const_void_pointer = typename std::pointer_traits<pointer>::template
//                                                           rebind<const void>;

//     using difference_type = typename std::pointer_traits<pointer>::difference_type;
//     using size_type       = std::make_unsigned_t<difference_type>;

//     template <class U> struct rebind {typedef fixed_allocator<U> other;};

    fixed_allocator() noexcept {}  // not required, unless used
    template <class U> fixed_allocator(fixed_allocator<U> const&) noexcept {}

    value_type*  // Use pointer if pointer is not a value_type*
    allocate(std::size_t n)
    {
    		current = current +n;
    		if (current >= SIZE) throw std::bad_alloc{};
    		value_type* res= mem.data() + (current-n);
        return res;//static_cast<value_type*>(::operator new (n*sizeof(value_type)));
    }

    void
    deallocate(value_type* p, std::size_t) noexcept  // Use pointer if pointer is not a value_type*
    { // no op
        //::operator delete(p);
    }

//     value_type*
//     allocate(std::size_t n, const_void_pointer)
//     {
//         return allocate(n);
//     }

//     template <class U, class ...Args>
//     void
//     construct(U* p, Args&& ...args)
//     {
//         ::new(p) U(std::forward<Args>(args)...);
//     }

//     template <class U>
//     void
//     destroy(U* p) noexcept
//     {
//         p->~U();
//     }

//     std::size_t
//     max_size() const noexcept
//     {
//         return std::numeric_limits<size_type>::max();
//     }

//     fixed_allocator
//     select_on_container_copy_construction() const
//     {
//         return *this;
//     }

//     using propagate_on_container_copy_assignment = std::false_type;
//     using propagate_on_container_move_assignment = std::false_type;
//     using propagate_on_container_swap            = std::false_type;
//     using is_always_equal                        = std::is_empty<fixed_allocator>;
};

template <class T, class U>
bool
operator==(fixed_allocator<T> const&, fixed_allocator<U> const&) noexcept
{
    return true;
}

template <class T, class U>
bool
operator!=(fixed_allocator<T> const& x, fixed_allocator<U> const& y) noexcept
{
    return !(x == y);
}



#endif /* SRC_FIXEDALLOCATOR_H_ */
