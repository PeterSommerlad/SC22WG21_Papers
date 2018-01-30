# include <iostream>
# include <string>
#include <cstring>
#include <algorithm>
# include "stopwatch11.h"
# include "test_driver.h"

// original, naive remove_ctrl()
std::string remove_ctrl(std::string s) {
    std::string result;
    for (size_t i=0; i<s.length(); ++i) {
        if (s[i] >= 0x20)
            result = result + s[i];
    }
    return result;
}

// remove_ctrl() with operator replaced by mutating assignment
std::string remove_ctrl_mutating(std::string s) {
    std::string result;
    for (size_t i=0; i<s.length(); ++i) {
        if (s[i] >= 0x20)
            result += s[i];
    }
    return result;
}
std::string remove_ctrl_mutating_it(std::string s) {
    std::string result;
	for (auto it=s.begin(); it != s.end(); ++it) {
		if (*it >= 0x20)
			result += *it;
    }
    return result;
}

std::string remove_ctrl_mutating_it_end(std::string s) {
    std::string result;
	for (auto it=s.begin(),end=s.end();it != end; ++it) {
		if (*it >= 0x20)
			result += *it;
    }
    return result;
}

// remove_ctrl_mutating() with space reserved in result
std::string remove_ctrl_reserve(std::string s) {
    std::string result;
    result.reserve(s.length());
    for (size_t i=0; i<s.length(); ++i) {
        if (s[i] >= 0x20)
            result += s[i];
    }
    return result;
}
std::string remove_ctrl_reserve_it(std::string s) {
    std::string result;
    result.reserve(s.length());
	for (auto it=s.begin(),end=s.end();it != end; ++it) {
		if (*it >= 0x20)
			result += *it;
    }
    return result;
}

// remove_ctrl_reserve() with reference arg instead of value arg
std::string remove_ctrl_refs(std::string const& s) {
    std::string result;
    result.reserve(s.length());
    for (size_t i=0; i<s.length(); ++i) {
        if (s[i] >= 0x20)
            result += s[i];
    }
    return result;
}

// remove_ctrl_reserve() with reference arg instead of value arg
std::string remove_ctrl_refs_it(std::string const& s) {
    std::string result;
    result.reserve(s.length());
	for (auto it=s.begin(),end=s.end();it != end; ++it) {
		if (*it >= 0x20)
			result += *it;
    }
    return result;
}

// remove_ctrl_refs() with reference return value
void remove_ctrl_ref_result(
	std::string& result,
    std::string const& s) {
    result.clear();
    result.reserve(s.length());
    for (size_t i=0; i<s.length(); ++i) {
        if (s[i] >= 0x20)
            result += s[i];
    }
}

// remove_ctrl_ref_ret with iterators
void remove_ctrl_ref_result_it(
	std::string& result,
    std::string const& s) {
    result.clear();
    result.reserve(s.length());
	for (auto it=s.begin(),end=s.end();it != end; ++it) {
		if (*it >= 0x20)
			result += *it;
    }
}

// remove_ctrl_ref_result() done with buffers instead of strings
void remove_ctrl_cstrings(char* destp, char const* sourcep, size_t length) {
    for (size_t i=0; i<length; ++i) {
        if (sourcep[i] >= 0x20)
            *destp++ = sourcep[i];
    }
    *destp = 0;
}

//  copy whole substrings to reduce allocations
std::string remove_ctrl_block(std::string s) {
    std::string result;
    for (size_t b=0,i=b,e=s.length(); b < e; b = i+1) {
        for (i=b; i<e; ++i) {
            if (s[i] < 0x20) break;
        }
        result = result + s.substr(b,i-b);
    }
    return result;
}

// remove_ctrl_block() with mutating concatenation, reserved size
std::string remove_ctrl_block_mutate(std::string s) {
    std::string result;
    result.reserve(s.length());
    for (size_t b=0,i=b,e=s.length(); b < e; b = i+1) {
        for (i=b; i<e; ++i) {
            if (s[i] < 0x20) break;
        }
        result += s.substr(b,i-b);
    }
    return result;
}

std::string remove_ctrl_block_append(std::string s) {
    std::string result;
    result.reserve(s.length());
    for (size_t b=0,i=b; b < s.length(); b = i+1) {
        for (i=b; i<s.length(); ++i) {
            if (s[i] < 0x20) break;
        }
        result.append(s, b, i-b);
    }
    return result;
}

std::string remove_ctrl_block_append_it(std::string s) {
    std::string result;
    result.reserve(s.length());
    for (auto b=s.begin(),i=b; b != s.end(); b = i+1) {
        for (i=b; i != s.end(); ++i) {
            if (*i < 0x20) break;
        }
        result.append(b, i);
    }
    return result;
}

std::string remove_ctrl_block_append_cache(std::string s) {
    std::string result;
    result.reserve(s.length());
    for (size_t b=0,i=b,e=s.length(); b < e; b = i+1) {
        for (i=b; i<e; ++i) {
            if (s[i] < 0x20) break;
        }
        result.append(s, b, i-b);
    }
    return result;
}

std::string remove_ctrl_block_append_cache_it(std::string s) {
    std::string result;
    result.reserve(s.length());
    for (auto b=s.begin(),i=s.begin(),e=s.end(); b != e; b = i+1) {
        for (i=b; i != e; ++i) {
            if (*i < 0x20) break;
        }
        result.append(b, i);
    }
    return result;
}

// remove_ctrl_block_mutate() with reference arg
std::string remove_ctrl_blocks(std::string const& s) {
    std::string result;
    result.reserve(s.length());
    for (size_t b=0,i=b,e=s.length(); b < e; b = i+1) {
        for (i=b; i<e; ++i) {
            if (s[i] < 0x20) break;
        }
        result.append(s, b,i-b);
    }
    return result;
}
//  remove_ctrl_blocks() with reference return value
void remove_ctrl_block_ret(std::string& result, std::string const& s) {
    result.clear();
    result.reserve(s.length());
    for (size_t b=0,i=b,e=s.length(); b < e; b = i+1) {
        for (i=b; i<e; ++i) {
            if (s[i] < 0x20) break;
        }
        result.append(s, b, i-b);
    }
}

void remove_ctrl_block_ret_it(std::string& result, std::string const& s) {
    result.clear();
    result.reserve(s.length());
    for (auto b=s.begin(),i=b,e=s.end(); b != e; b = i+1) {
        for (i=b; i != e; ++i) {
            if (*i < 0x20) break;
        }
        result.append(b, i);
    }
}

//  cleverly reduce the size of a string so it doesn't have to be reallocated
std::string remove_ctrl_erase(std::string s) {
    for (size_t i = 0; i < s.length(); ) 
        if (s[i] < 0x20)
            s.erase(i,1);
        else ++i;
    return s;
}
std::string remove_ctrl_erase_it(std::string s) {
    for (auto i = s.begin(); i != s.end(); ) 
        if (*i < 0x20)
            s.erase(i);
        else ++i;
    return s;
}
std::string remove_ctrl_erase_alg(std::string s) {
	s.erase(remove_if(cbegin(s),cend(s),begin(s),[](char c){return c<0x20;}),s.end());
	return s;
}

int test_strings(int test_no, unsigned long multiplier) {
	typedef unsigned counter_t;
	counter_t iterations = 1000*multiplier;

    std::string s("\07Now is the time\07 for all good men\r\n to come to the aid of their country. \07");
    std::string test("Now is the time for all good men to come to the aid of their country. ");
    std::string result;
    s = s + s + s;
    test = test + test + test;

    bool rc = true;

    switch (test_no) {
    default: return -1;
    case 0:  return 12;
    case 1:
#       if defined _WIN32
#           if defined _DEBUG
                std::cout << "Windows debug build" << std::endl;
#           else
                std::cout << "Windows release build" << std::endl;
#           endif
#       else
            std::cout << "Linux build" << std::endl;
#       endif   
        std::cout << s.length() << " character argument to remove_ctrl()" << std::endl;
        std::cout << iterations << " iterations" << std::endl;
        result = remove_ctrl(s);
        rc &= (result.compare(test) == 0);
        result = remove_ctrl_mutating(s);
        rc &= (result.compare(test) == 0);
        result = remove_ctrl_mutating_it(s);
        rc &= (result.compare(test) == 0);
        result = remove_ctrl_reserve(s);
        rc &= (result.compare(test) == 0);
        result = remove_ctrl_reserve_it(s);
        rc &= (result.compare(test) == 0);
        result = remove_ctrl_refs(s);
        rc &= (result.compare(test) == 0);
        result = remove_ctrl_refs_it(s);
        rc &= (result.compare(test) == 0);
        remove_ctrl_ref_result(result, s);
        rc &= (result.compare(test) == 0);
        remove_ctrl_ref_result_it(result, s);
        rc &= (result.compare(test) == 0);
        {
            char a[1000]; 
            memset(a, 0, sizeof(a));
            remove_ctrl_cstrings(a, s.data(), s.length());
            rc &= (strcmp(a, test.data()) == 0);
//            memset(a, 0, sizeof(a));
//            remove_ctrl_6(a, s.data(), s.length());
//            rc &= (strcmp(a, test.data()) == 0);
        }
        result = remove_ctrl_block(s);
        rc &= (result.compare(test) == 0);
        result = remove_ctrl_block_mutate(s);
        rc &= (result.compare(test) == 0);
        result = remove_ctrl_block_append(s);
        rc &= (result.compare(test) == 0);
        result = remove_ctrl_block_append_it(s);
        rc &= (result.compare(test) == 0);
        result = remove_ctrl_block_append_cache(s);
        rc &= (result.compare(test) == 0);
        result = remove_ctrl_block_append_cache_it(s);
        rc &= (result.compare(test) == 0);
        result = remove_ctrl_blocks(s);
        rc &= (result.compare(test) == 0);
        remove_ctrl_block_ret(result, s);
        rc &= (result.compare(test) == 0);
        remove_ctrl_block_ret(result, s);
        rc &= (result.compare(test) == 0);
        remove_ctrl_block_ret_it(result, s);
        rc &= (result.compare(test) == 0);
        result = remove_ctrl_erase(s);
        rc &= (result.compare(test) == 0);
        result = remove_ctrl_erase_it(s);
        rc &= (result.compare(test) == 0);
        break;

    case 2:
		{
            Stopwatch sw("remove_ctrl()");
            for (counter_t i = 0; i < iterations; ++i)
                result = remove_ctrl(s);
        }
        break;

    case 3: 
		{   Stopwatch sw("remove_ctrl_mutating()");
			for (counter_t i = 0; i < iterations; ++i)
				result = remove_ctrl_mutating(s);
        }
		{   Stopwatch sw("remove_ctrl_mutating_it()");
			for (counter_t i = 0; i < iterations; ++i)
				result = remove_ctrl_mutating_it(s);
        }
		{   Stopwatch sw("remove_ctrl_mutating_it_end()");
			for (counter_t i = 0; i < iterations; ++i)
				result = remove_ctrl_mutating_it_end(s);
        }
        break;

    case 4: 
		{   Stopwatch sw("remove_ctrl_reserve()");
			for (counter_t i = 0; i < iterations; ++i)
				result = remove_ctrl_reserve(s);
        }
		{   Stopwatch sw("remove_ctrl_reserve_it()");
			for (counter_t i = 0; i < iterations; ++i)
				result = remove_ctrl_reserve_it(s);
        }
        break;

    case 5: 
		{   Stopwatch sw("remove_ctrl_refs()");
			for (counter_t i = 0; i < iterations; ++i)
				result = remove_ctrl_refs(s);
        }
		{   Stopwatch sw("remove_ctrl_refs_it()");
			for (counter_t i = 0; i < iterations; ++i)
				result = remove_ctrl_refs_it(s);
        }
        break;

    case 6: 
		{   Stopwatch sw("remove_ctrl_ref_result()");
			for (counter_t i = 0; i < iterations; ++i)
				remove_ctrl_ref_result(result, s);
        }
		{   Stopwatch sw("remove_ctrl_ref_it()");
			for (counter_t i = 0; i < iterations; ++i)
				remove_ctrl_ref_result_it(result, s);
        }
        break;

    case 7: {
            Stopwatch sw("remove_ctrl_cstrings()");
			for (counter_t i = 0; i < iterations; ++i) {
				char a[1000];
                remove_ctrl_cstrings(a, s.data(), s.length());
            }
        }
        break;

    case 8: {
            Stopwatch sw("remove_ctrl_block()");
			for (counter_t i = 0; i < iterations; ++i)
				result = remove_ctrl_block(s);
        }
        break;

    case 9: 
		{   Stopwatch sw("remove_ctrl_block_mutate()");
			for (counter_t i = 0; i < iterations; ++i)
				result = remove_ctrl_block_mutate(s);
        }
		{   Stopwatch sw("remove_ctrl_block_append()");
			for (counter_t i = 0; i < iterations; ++i)
				result = remove_ctrl_block_append(s);
        }
		{   Stopwatch sw("remove_ctrl_block_append_it()");
			for (counter_t i = 0; i < iterations; ++i)
				result = remove_ctrl_block_append_it(s);
        }
		{   Stopwatch sw("remove_ctrl_block_append_cache()");
			for (counter_t i = 0; i < iterations; ++i)
				result = remove_ctrl_block_append_cache(s);
        }
		{   Stopwatch sw("remove_ctrl_block_append_cache_it()");
			for (counter_t i = 0; i < iterations; ++i)
				result = remove_ctrl_block_append_cache_it(s);
        }
        break;

    case 10: {
            Stopwatch sw("remove_ctrl_blocks()");
			for (counter_t i = 0; i < iterations; ++i)
				result = remove_ctrl_blocks(s);
        }
        break;

    case 11: 
		{    Stopwatch sw("remove_ctrl_block_ret()");
			for (counter_t i = 0; i < iterations; ++i)
				remove_ctrl_block_ret(result, s);
        }
		{    Stopwatch sw("remove_ctrl_block_ret_it()");
			for (counter_t i = 0; i < iterations; ++i)
				remove_ctrl_block_ret_it(result, s);
        }
        break;

    case 12:
		{   Stopwatch sw("remove_ctrl_erase()");
			for (counter_t i = 0; i < iterations; ++i)
				result = remove_ctrl_erase(s);
        }
		{   Stopwatch sw("remove_ctrl_erase_it()");
			for (counter_t i = 0; i < iterations; ++i)
				result = remove_ctrl_erase_it(s);
        }
		{   Stopwatch sw("remove_ctrl_erase_alg()");
			for (counter_t i = 0; i < iterations; ++i)
				result = remove_ctrl_erase_alg(s);
        }
        break;
    }
    return rc ? 1 : 0;
}


int main(int argc, char** argv) {
	test_strings(12,1000);
//	test_driver(test_strings, argc, argv);
	return 0;
}
