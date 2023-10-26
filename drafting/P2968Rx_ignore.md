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

Programming guidelines for C++ safety-critical systems consider all named functions with a non-void return type similar to having the attribute `[[nodiscard]]`.

As of today, the means to disable diagnostic is a static cast to void spelled with a C-style cast `(void) foo();`. 
This provides a teachability issue, because, while C-style casts are banned, such a C-style cast need to be taught.
None of the guidelines I am aware of, dared to ask for a `static_cast<void>( foo() );` in those situation.

With the semantics provided by the major standard library implementations and the semantics of the example implementation given in the cppreference.com site, it would be much more documenting intent to write

`std::ignore = foo();`

instead of the C-style void-cast.

To summarize the proposed change:

1. better self-documenting code telling the intent
2. Improved teachability of C++ to would-be C++ programmers in safety critical environments

# Mailing List discussions

After some initial drafts posted to lib-ext@lists.isocpp.org I got some further feedback on motivation and desire to move `ignore` to a separate header or utility:

Additional motivating usage by Arthur O'Dwyer:

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

Giuseppe D'Angelo: As an extra, could it be possible to move `std::ignore` out of `<tuple>` and into `<utility>` or possibly its own header `<ignore>`?
  
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

All three major standard library vendors (libstdc++, Microsoft, libc++) implement `std::ignore` in roughly the same
way, but there are some minor differences that could be ironed out by this proposal.

```c++
// libstdc++
struct __ignore_t {
  constexpr const __ignore_t&
    operator=(const auto&) const
      { return *this; }
};
inline constexpr __ignore_t ignore{};

// Microsoft STL
struct __ignore_t {
  constexpr const __ignore_t&
    operator=(const auto&) const noexcept
      { return *this; }
};
inline constexpr __ignore_t ignore{};

// libc++
template<class T>
struct __ignore_t {
  constexpr const __ignore_t&
    operator=(auto&&) const
      { return *this; }
};
inline constexpr __ignore_t<unsigned char> ignore = __ignore_t<unsigned char>();
```

The programmer can detect whether `ignore`'s type is a template. We suggest either mandating that `ignore`'s type not be a template,
or finding a way to leave it unspecified.

The difference between `operator=(const auto&)` and `operator=(auto&&)` is that the former will bind to non-volatile
bit-field glvalues, whereas the latter will not. We want to support non-volatile bit-fields, so we specify the signature of
`ignore::operator=` to take `(const auto&)` instead of `(auto&&)`. (Neither signature binds to <em>volatile</em> bit-field glvalues;
that is <a href="https://cplusplus.github.io/LWG/issue3978">[LWG3978]</a>. Our wording makes
`std::ignore = vbf;` obviously ill-formed, which resolves LWG3978.)

The difference between `noexcept` and not is negligible; we could mandate that `std::ignore::operator=` be noexcept,
or we could leave it non-noexcept. The vendor is already permitted to strengthen the noexceptness of any standard library
function; Microsoft takes advantage of this freedom already. We suggest that as long as we're touching this area anyway,
we should go ahead and add the `noexcept`.


# Wording

In <a href="https://eel.is/c++draft/tuple.syn">[tuple.syn]</a> change the type of `ignore` from "unspecified" to an exposition-only non-template type:

```diff
+// [utility.ignore], ignore
+struct @_ignore_t_@ { // @_exposition only_@
+  constexpr const @_ignore_t_@&
+    operator=(const auto&) const noexcept
+      { return *this; }
+};
+inline constexpr @_ignore_t_@ ignore;

 // [tuple.creation], tuple creation functions
-inline constexpr @_unspecified_@ ignore;
```

In <a href="https://eel.is/c++draft/tuple.creation">[tuple.creation]</a>, remove the normative text about `ignore`:

```diff
 template<class... TTypes>
   constexpr tuple<TTypes&...> tie(TTypes&... t) noexcept;
 5. Returns: tuple<TTypes&...>(t...).
-When an argument in t is ignore, assigning any value to the corresponding tuple element has no effect.
 6. [Example 2: tie functions allow one to create tuples that unpack tuples into variables.
 ignore can be used for elements that are not needed:
     int i; std::string s;
     tie(i, ignore, s) = make_tuple(42, 3.14, "C++");
     // i == 42, s == "C++"
 — end example]
```

In <a href="https://eel.is/c++draft/utility.syn">[utility.syn]</a>, add `ignore` to the `<utility>` header also:

```diff
 template<class... T>
   using index_sequence_for = make_index_sequence<sizeof...(T)>;

+// [utility.ignore], ignore
+struct @_ignore_t_@ { // @_exposition only_@
+  constexpr const @_ignore_t_@&
+    operator=(const auto&) const noexcept
+      { return *this; }
+};
+inline constexpr @_ignore_t_@ ignore;

 // [pairs], class template pair
 template<class T1, class T2>
   struct pair;
```

We don't think a feature-test macro is required for this patch, as it's just barely visible to the programmer, and there
is no situation where the programmer would want to do something different based on the absence of the feature.

:::

# References

* [LWG 2933](https://wg21.link/lwg2933)
* The markdown of this document is available at my [github](https://raw.githubusercontent.com/PeterSommerlad/SC22WG21_Papers/master/drafting/P2968Rx_ignore.md)
