//============================================================================
// Name        : checker_test.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>



#include <utility>


struct on_exit_policy
{
    bool execute_{ true };

    void release() noexcept
    {
        execute_ = false;
    }

    bool should_execute() noexcept // constificator fails
    {
        return execute_;
    }
};


template <typename T>
class bla{
	    template<typename RM, typename DM, typename S>
	    friend
	    auto make_unique_resource_checked(RM &&r, const S &invalid, DM &&d)
	    noexcept(noexcept(make_unique_resource(std::forward<RM>(r), std::forward<DM>(d))));

	bla(T const &){}
	[[gsl::suppress("Rc-explicit")]]
	bla(T&&) noexcept (1 + 1)
	{
	}

};
class foo{
	foo(foo const &){}
	foo(int){}
};
using namespace std;

int main() {
	cout << "!!!Hello World!!!" ; // prints !!!Hello World!!!
	return 0;
}
