/*
 * april_fool.h
 *
 *  Created on: 28 Mar 2018
 *      Author: sop
 */

#ifndef SRC_APRIL_FOOL_H_
#define SRC_APRIL_FOOL_H_
#include <utility>
namespace std_ex{

//constexpr inline auto Deref = @\seebelow@ ; // unary *
//  constexpr inline auto Address = @\seebelow@ ; // unary \&
//  constexpr inline auto Negate = @\seebelow@ ; // unary -
//  constexpr inline auto Posate = @\seebelow@ ; // unary +
//  constexpr inline auto Not = @\seebelow@ ; // ! not
//  constexpr inline auto Cmpl = @\seebelow@ ; // \~ cmpl

  constexpr inline auto Deref =
    [](auto&& l) constexpr
      noexcept(noexcept(* std::declval<decltype(l)>()))
  -> decltype(* std::forward<decltype(l)>(l))
    {
      return * std::forward<decltype(l)>(l);
    };
  constexpr inline auto Address =
    [](auto&& l) constexpr
      noexcept(noexcept(&std::declval<decltype(l)>()))
  -> decltype(& std::forward<decltype(l)>(l))
    {
      return & std::forward<decltype(l)>(l);
    };
  constexpr inline auto Negate =
    [](auto&& l) constexpr
      noexcept(noexcept(-std::declval<decltype(l)>()))
  -> decltype(- std::forward<decltype(l)>(l))
    {
      return - std::forward<decltype(l)>(l);
    };
  constexpr inline auto Posate =
    [](auto&& l) constexpr
      noexcept(noexcept(+std::declval<decltype(l)>()))
  -> decltype(+ std::forward<decltype(l)>(l))
    {
      return + std::forward<decltype(l)>(l);
    };
  constexpr inline auto Not =
    [](auto&& l) constexpr
      noexcept(noexcept(!std::declval<decltype(l)>()))
  -> decltype(! std::forward<decltype(l)>(l))
    {
      return ! std::forward<decltype(l)>(l);
    };

  constexpr inline auto Cmpl =
    [](auto&& l) constexpr
      noexcept(noexcept(~std::declval<decltype(l)>()))
  -> decltype(~ std::forward<decltype(l)>(l))
    {
      return ~ std::forward<decltype(l)>(l);
    };

// left associative binary
//  constexpr inline auto PtrMemb = @\seebelow@ ; // ->*
//  constexpr inline auto RefMemb = @\seebelow@ ; // .*
//
//  constexpr inline auto Plus = @\seebelow@ ; // +
//  constexpr inline auto Minus = @\seebelow@ ; // -
//  constexpr inline auto Times = @\seebelow@ ; // *
//  constexpr inline auto Divide = @\seebelow@ ; // /
//  constexpr inline auto Remainder = @\seebelow@ ; // \&
//
//  constexpr inline auto Equal = @\seebelow@ ; // ==
//  using equalTea = decltype(Equal); // to replace equal_to<>
//  constexpr inline auto Not_eq = @\seebelow@ ; // !=
//  constexpr inline auto Bigger = @\seebelow@ ; // >
//  using moreTea = decltype(Bigger); // to replace greater<>
//  constexpr inline auto Smaller = @\seebelow@ ; // <
//  using lessTea = decltype(Smaller); // to replace less<>
//  constexpr inline auto Maybe_bigger = @\seebelow@ ; // >=
//  constexpr inline auto Sometimes_smaller = @\seebelow@ ; // <=
//  constexpr inline auto Spaceship = @\seebelow@ ; // <=>
//
//  constexpr inline auto And = @\seebelow@ ; // \&\& and
//  constexpr inline auto Or = @\seebelow@ ; // || or
//
//  constexpr inline auto Bitand = @\seebelow@ ; // \& bit_and
//  constexpr inline auto Bitor = @\seebelow@ ; // | bit_or
//  constexpr inline auto Xor = @\seebelow@ ; // \^{} xor
//  constexpr inline auto Lshift = @\seebelow@ ; // <{}<
//  constexpr inline auto Rshift = @\seebelow@ ; // >{}>

  constexpr auto PtrMemb=[](auto&&... l) constexpr
  noexcept(noexcept((... ->* std::declval<decltype(l)>())))
  -> decltype((... ->* std::forward<decltype(l)>(l)
  		))
  {
  	return (... ->* std::forward<decltype(l)>(l));
  };
  constexpr auto RefMemb=[](auto&&... l) constexpr
  noexcept(noexcept((... .* std::declval<decltype(l)>())))
  -> decltype((... .* std::forward<decltype(l)>(l)
  		))
  {
  	return (... .* std::forward<decltype(l)>(l));
  };

  constexpr auto Plus=[](auto&&... l) constexpr
  noexcept(noexcept((... + std::declval<decltype(l)>())))
  -> decltype((... + std::forward<decltype(l)>(l)
  		))
  {
  	return (...+std::forward<decltype(l)>(l));
  };
  constexpr auto Minus=[](auto&&... l) constexpr
  noexcept(noexcept((... - std::declval<decltype(l)>())))
  -> decltype((... - std::forward<decltype(l)>(l)
  		))
  {
  	return (... - std::forward<decltype(l)>(l));
  };


  constexpr auto Times=[](auto&&... l) constexpr
  noexcept(noexcept((... * std::declval<decltype(l)>())))
  -> decltype((... * std::forward<decltype(l)>(l)
  		))
  {
  	return (...*std::forward<decltype(l)>(l));
  };
  constexpr auto Divide=[](auto&&... l) constexpr
  noexcept(noexcept((... / std::declval<decltype(l)>())))
  -> decltype((... / std::forward<decltype(l)>(l)
  		))
  {
  	return (... / std::forward<decltype(l)>(l));
  };
  constexpr auto Remainder=[](auto&&... l) constexpr
  noexcept(noexcept((... % std::declval<decltype(l)>())))
  -> decltype((... % std::forward<decltype(l)>(l)
  		))
  {
  	return (... % std::forward<decltype(l)>(l));
  };
  constexpr auto Equal=[](auto&&... l) constexpr
  noexcept(noexcept((... == std::declval<decltype(l)>())))
  -> decltype((... == std::forward<decltype(l)>(l)
  		))
  {
  	return (... == std::forward<decltype(l)>(l));
  };

  using equalTea = decltype(Equal);
  constexpr auto Not_eq=[](auto&&... l) constexpr
  noexcept(noexcept((... not_eq std::declval<decltype(l)>())))
  -> decltype((... not_eq std::forward<decltype(l)>(l)
  		))
  {
  	return (... not_eq std::forward<decltype(l)>(l));
  };

  using not_equalTea = decltype(Not_eq);

  constexpr auto Bigger=[](auto&&... l) constexpr
  noexcept(noexcept((... > std::declval<decltype(l)>())))
  -> decltype((... > std::forward<decltype(l)>(l)
  		))
  {
  	return (... > std::forward<decltype(l)>(l));
  };

  using moreTea = decltype(Bigger);

  constexpr auto Smaller=[](auto&&... l) constexpr
  noexcept(noexcept((... < std::declval<decltype(l)>())))
  -> decltype((... < std::forward<decltype(l)>(l)
  		))
  {
  	return (... < std::forward<decltype(l)>(l));
  };

  using lessTea = decltype(Smaller);
  constexpr auto Maybe_bigger=[](auto&&... l) constexpr
  noexcept(noexcept((... >= std::declval<decltype(l)>())))
  -> decltype((... >= std::forward<decltype(l)>(l)
  		))
  {
  	return (... >= std::forward<decltype(l)>(l));
  };

  constexpr auto Sometimes_smaller=[](auto&&... l) constexpr
  noexcept(noexcept((... <= std::declval<decltype(l)>())))
  -> decltype((... <= std::forward<decltype(l)>(l)
  		))
  {
  	return (... <= std::forward<decltype(l)>(l));
  };

  // no C++20 compiler at hand...
//  constexpr auto Spaceship=[](auto&&... l) constexpr
//  noexcept(noexcept((... <=> std::declval<decltype(l)>())))
//  -> decltype((... <=> std::forward<decltype(l)>(l)
//  		))
//  {
//  	return (... <=> std::forward<decltype(l)>(l));
//  };

  constexpr auto And=[](auto&&... l) constexpr
  noexcept(noexcept((... && std::declval<decltype(l)>())))
  -> decltype((... && std::forward<decltype(l)>(l)
  		))
  {
  	return (... && std::forward<decltype(l)>(l));
  };

  constexpr auto Or=[](auto&&... l) constexpr
  noexcept(noexcept((... || std::declval<decltype(l)>())))
  -> decltype((... || std::forward<decltype(l)>(l)
  		))
  {
  	return (... || std::forward<decltype(l)>(l));
  };


constexpr auto Lshift=[](auto&&... l) constexpr
noexcept(noexcept((... << std::declval<decltype(l)>())))
-> decltype((... << std::forward<decltype(l)>(l)
		))
{
	return (... << std::forward<decltype(l)>(l));
};
constexpr auto Assign=[](auto&&... l) constexpr
noexcept(noexcept((std::declval<decltype(l)>() = ... )))
//-> decltype((std::forward<decltype(l)>(l) = ... )) // ICE with gcc 7.3
-> decltype(auto)
{
	return (std::forward<decltype(l)>(l) = ... );
};
constexpr auto AddAssign=[](auto&&... l) constexpr
noexcept(noexcept((std::declval<decltype(l)>() += ... )))
-> decltype((std::forward<decltype(l)>(l) += ... ))
{
	return (std::forward<decltype(l)>(l) += ... );
};


// ternary operator

constexpr auto Wtf=[](auto&&c, auto&& l, auto&& r) constexpr
noexcept(noexcept(std::declval<decltype(c)>() ? std::declval<decltype(l)>() : std::declval<decltype(r)>()))
-> decltype(std::declval<decltype(c)>() ? std::declval<decltype(l)>() : std::declval<decltype(r)>())
{
	return std::forward<decltype(c)>(c) ?
			std::forward<decltype(l)>(l) : std::forward<decltype(r)>(r);
};


}

#endif /* SRC_APRIL_FOOL_H_ */
