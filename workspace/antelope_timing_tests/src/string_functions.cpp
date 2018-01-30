#define USE_ASCII
#include "string_functions.h"
#include <algorithm>
#include <cctype>

// original, naive remove_ctrl()
std::string remove_ctrl(std::string s) {
	std::string result;
	for (size_t i = 0; i < s.length(); ++i) {
		if (s[i] >= 0x20)
			result = result + s[i];
	}
	return result;
}

std::string remove_ctrl_erase_alg(std::string s) {
	s.erase(remove_if(begin(s), end(s), [](char c) {
		return c < 0x20;//::iscntrl(c);//
	}), s.end());
	return s;
}

std::string remove_ctrl_erase_it(std::string s) {
	for (auto i = s.begin(); i != s.end();)
		if (*i < 0x20)
			s.erase(i);
		else
			++i;
	return s;
}

//  cleverly reduce the size of a string so it doesn't have to be reallocated
std::string remove_ctrl_erase(std::string s) {
	for (size_t i = 0; i < s.length();)
		if (s[i] < 0x20)
			s.erase(i, 1);
		else
			++i;
	return s;
}

void remove_ctrl_block_ret_it(std::string& result, const std::string& s) {
	result.clear();
	result.reserve(s.length());
	for (auto b = s.begin(), i = b, e = s.end(); b != e; b = i + 1) {
		for (i = b; i != e; ++i) {
			if (*i < 0x20)
				break;
		}
		result.append(b, i);
	}
}

//  remove_ctrl_blocks() with reference return value
void remove_ctrl_block_ret(std::string& result, const std::string& s) {
	result.clear();
	result.reserve(s.length());
	for (size_t b = 0, i = b, e = s.length(); b < e; b = i + 1) {
		for (i = b; i < e; ++i) {
			if (s[i] < 0x20)
				break;
		}
		result.append(s, b, i - b);
	}
}

// remove_ctrl_block_mutate() with reference arg
std::string remove_ctrl_blocks(const std::string& s) {
	std::string result;
	result.reserve(s.length());
	for (size_t b = 0, i = b, e = s.length(); b < e; b = i + 1) {
		for (i = b; i < e; ++i) {
			if (s[i] < 0x20)
				break;
		}
		result.append(s, b, i - b);
	}
	return result;
}

std::string remove_ctrl_block_append_cache_it(std::string s) {
	std::string result;
	result.reserve(s.length());
	for (auto b = s.begin(), i = s.begin(), e = s.end(); b != e; b = i + 1) {
		for (i = b; i != e; ++i) {
			if (*i < 0x20)
				break;
		}
		result.append(b, i);
	}
	return result;
}

std::string remove_ctrl_block_append_cache(std::string s) {
	std::string result;
	result.reserve(s.length());
	for (size_t b = 0, i = b, e = s.length(); b < e; b = i + 1) {
		for (i = b; i < e; ++i) {
			if (s[i] < 0x20)
				break;
		}
		result.append(s, b, i - b);
	}
	return result;
}

std::string remove_ctrl_block_append_it(std::string s) {
	std::string result;
	result.reserve(s.length());
	for (auto b = s.begin(), i = b; b != s.end(); b = i + 1) {
		for (i = b; i != s.end(); ++i) {
			if (*i < 0x20)
				break;
		}
		result.append(b, i);
	}
	return result;
}

std::string remove_ctrl_block_append(std::string s) {
	std::string result;
	result.reserve(s.length());
	for (size_t b = 0, i = b; b < s.length(); b = i + 1) {
		for (i = b; i < s.length(); ++i) {
			if (s[i] < 0x20)
				break;
		}
		result.append(s, b, i - b);
	}
	return result;
}

// remove_ctrl_block() with mutating concatenation, reserved size
std::string remove_ctrl_block_mutate(std::string s) {
	std::string result;
	result.reserve(s.length());
	for (size_t b = 0, i = b, e = s.length(); b < e; b = i + 1) {
		for (i = b; i < e; ++i) {
			if (s[i] < 0x20)
				break;
		}
		result += s.substr(b, i - b);
	}
	return result;
}

//  copy whole substrings to reduce allocations
std::string remove_ctrl_block(std::string s) {
	std::string result;
	for (size_t b = 0, i = b, e = s.length(); b < e; b = i + 1) {
		for (i = b; i < e; ++i) {
			if (s[i] < 0x20)
				break;
		}
		result = result + s.substr(b, i - b);
	}
	return result;
}

// remove_ctrl_ref_result() done with buffers instead of strings
void remove_ctrl_cstrings(char* destp, const char* sourcep, size_t length) {
	for (size_t i = 0; i < length; ++i) {
		if (sourcep[i] >= 0x20)
			*destp++ = sourcep[i];
	}
	*destp = 0;
}

// remove_ctrl_ref_ret with iterators
void remove_ctrl_ref_result_it(std::string& result, const std::string& s) {
	result.clear();
	result.reserve(s.length());
	for (auto it = s.begin(), end = s.end(); it != end; ++it) {
		if (*it >= 0x20)
			result += *it;
	}
}

// remove_ctrl_refs() with reference return value
void remove_ctrl_ref_result(std::string& result, const std::string& s) {
	result.clear();
	result.reserve(s.length());
	for (size_t i = 0; i < s.length(); ++i) {
		if (s[i] >= 0x20)
			result += s[i];
	}
}

// remove_ctrl_reserve() with reference arg instead of value arg
std::string remove_ctrl_refs_it(const std::string& s) {
	std::string result;
	result.reserve(s.length());
	for (auto it = s.begin(), end = s.end(); it != end; ++it) {
		if (*it >= 0x20)
			result += *it;
	}
	return result;
}

// remove_ctrl_reserve() with reference arg instead of value arg
std::string remove_ctrl_refs(const std::string& s) {
	std::string result;
	result.reserve(s.length());
	for (size_t i = 0; i < s.length(); ++i) {
		if (s[i] >= 0x20)
			result += s[i];
	}
	return result;
}

std::string remove_ctrl_reserve_it(std::string s) {
	std::string result;
	result.reserve(s.length());
	for (auto it = s.begin(), end = s.end(); it != end; ++it) {
		if (*it >= 0x20)
			result += *it;
	}
	return result;
}

// remove_ctrl_mutating() with space reserved in result
std::string remove_ctrl_reserve(std::string s) {
	std::string result;
	result.reserve(s.length());
	for (size_t i = 0; i < s.length(); ++i) {
		if (s[i] >= 0x20)
			result += s[i];
	}
	return result;
}

std::string remove_ctrl_mutating_it_end(std::string s) {
	std::string result;
	for (auto it = s.begin(), end = s.end(); it != end; ++it) {
		if (*it >= 0x20)
			result += *it;
	}
	return result;
}

std::string remove_ctrl_mutating_it(std::string s) {
	std::string result;
	for (auto it = s.begin(); it != s.end(); ++it) {
		if (*it >= 0x20)
			result += *it;
	}
	return result;
}

// remove_ctrl() with operator replaced by mutating assignment
std::string remove_ctrl_mutating(std::string s) {
	std::string result;
	for (size_t i = 0; i < s.length(); ++i) {
		if (s[i] >= 0x20)
			result += s[i];
	}
	return result;
}

