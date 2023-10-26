---
title: "Make std::ignore a first-class object"
document: P2968R0
date: 2023-09-09
audience: WG21 - Library & Library Evolution 
author:
  - name: Peter Sommerlad
    email: <peter.cpp@sommerlad.ch>
---

# Abstract

All major open source C++ library implementations provide a suitably implemented `std::ignore` allowing a no-op assignment from any object type. However, according to some C++ experts the standard doesn't bless its use beyond `std::tie`. This paper also resolves issue lwg2933 by proposing to make the use of `std::ignore`{.cpp} as the left-hand operand of an assignment expression official.

# History

## R0 initial revision

* Adresses [LWG 2933](https://wg21.link/lwg2933)
* Takes up some mailing list discussion

# Introduction

All major C++ open source standard libraries provide similar implementations for the type of `std::ignore`.
However, the semantics of `std::ignore`, while useful beyond, are only specified in the context of `std::tie`.

## Non-tuple applications of `std::ignore`

Programming guidelines for C++ safety critical systems consider all named functions with a non-void return type similar to having the attribute `[[nodiscard]]`.

As of today, the means to disable diagnostic is a static cast to void spelled with a C-style cast `(void) foo();`. 
This provides a teachability issue, because, while C-style casts are banned, such a C-style cast need to be taught.
None of the guidelines I am aware of, dared to ask for a `static_cast<void>( foo() );` in those situation.

With the semantics provided by the major standard library implementations and the semantics of the example implementation given in the cppreference.com site, it would be much more documenting intent to write

`std::ignore = foo();`

instead of the C-style void-cast.

To summarize the proposed change:

1. better self-documenting code telling the intent
2. Improved teachability of C++ to would-be C++ programmers in safety critical environments

## Comparison Table

::: cmptable

> Code that compiles today will be guaranteed to compile

### Before
```cpp
std::ignore = std::printf("hello ignore\n");
// compiles but is not sanctioned by the standard
```

### After
```cpp
std::ignore = std::printf("hello ignore\n");
// well defined C++
```

:::

# Mailing List discussions

After some initial draft posted to lib-ext@lists.isocpp.org I got some further feedback on motivation and desire to move ignore to a separate header or utility:

Additional motivational usage by Arthur O'Dwyer:   

```c++
  struct DevNullIterator {
    using difference_type = int;
    auto& operator++() { return *this; }
    auto& operator++(int) { return *this; }
    auto operator*() const { return std::ignore; }
  };

  int a[100];
  std::ranges::copy(a, a+100, DevNullIterator());
```

Giuseppe D'Angelo: As an extra, could it be possible to move std::ignore out of `<tuple>` and into `<utility>` or possibly its own header `<ignore>`?
  
Ville Voutilainen suggested a specification as code, such as:

```c++
// 22.4.5, tuple creation functions
struct @_ignore_type_@ { // @_exposition only_@
  decltype(auto) operator=(auto&&) const & { return *this; }
};
inline constexpr @_ignore_type_@ ignore;
```

or even more brief:

```c++
inline constexpr 
struct  { // exposition only
  decltype(auto) operator=(auto&&) const & { return *this; }
} ignore;
```

# Questions to LEWG

* Should `std::ignore`{.cpp} get its own header `<ignore>`? Y/N
* If no: Should `std::ignore`{.cpp} be moved to header `<utility>`? Y/N
  
# Questions to LWG

* Should the specification of the type of `std::ignore`{.cpp} use code or stick to a more generic text (see below)?
* Do we need to mention a "constepxr constructor"? (see parenthesized text in the Wording section)


# Impact on existing code

Since `std::ignore` is already implemented in a suitable way in all major C++ standard libraries, there is no impact on existing code.

However, may be LEWG will decide on being more specific and less hand-wavy on the semantics of the type underlying `std::ignore` and even follow the suggestion to move its definition into another header (`<utility>` or `<ignore>`).

If LWG decides on using code for the specification, libraries might want to adjust their implementation accordingly (which I believe is not required).

# Wording

In [tuple.syn] change the type of ignore from "unspecified" to an exposition-only type


```diff
// 22.4.5, tuple creation functions
+struct @_ignore_type_@; // @_expostion only_@
-inline constexpr @_unspecified_@ ignore;
+inline constexpr @_ignore_type_@ ignore;
```

Add at TBD the following:

::: add

The exposition-only class _`ignore_type`_ provides a ( constexpr constructor and ) constexpr const assignment operator template that allows assignment to `ignore` from any non-void type without having an effect.

:::

# References

* [LWG 2933](https://wg21.link/lwg2933)
* The markdown of this document is available at my [github](https://raw.githubusercontent.com/PeterSommerlad/SC22WG21_Papers/master/drafting/P2968Rx_ignore.md)
