# Make assert() macro user friendly

## problem in C++

```cpp
#include <cassert>
#include <type_traits>

using Int=int;

void f() {
 assert(std::is_same<int,Int>::value); // a surprisig compile error
}
```

## problem in C

```cpp
#include <assert.h>

void f() {
    assert((int[2]){1,2}[0]); // compile error
    struct A {int x,y;};
    assert((struct A){1,2}.x); // compile error
}
```


## proposed solution (NDEBUG)

```{.cpp}
#define assert(...) ((void)0)
```


## proposed solution C

```{.cpp}
static inline int 
__check_single_argument_passed_to_assert(int b) { return b; }
#define assert(...)  \
    ((void) (__check_single_argument_passed_to_assert(__VA_ARGS__) ?\
     ((void)0) : __assert (#_VA_ARGS__, __FILE__, __LINE__)))
```

## proposed solution C++

```{.cpp}
#define assert(...)  \
    ((void) (bool( __VA_ARGS__) ?
     ((void)0) : __assert (#_VA_ARGS__, __FILE__, __LINE__)))

````

## Experience

::: incremental

* changed local system header accordingly
* compiled major open source software packages with it successfully (homebrew installation on older macos, which requires compile-yourself)
  * python, gcc, llvm, inkscape,...
* And I know it was used, because compilation failed when I had bugs...

:::
