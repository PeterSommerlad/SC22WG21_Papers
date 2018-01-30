/*
 * string_functions.h
 *
 *  Created on: 20 Oct 2017
 *      Author: sop
 */

#ifndef SRC_STRING_FUNCTIONS_H_
#define SRC_STRING_FUNCTIONS_H_
#include <string>


// original, naive remove_ctrl()
std::string remove_ctrl(std::string s);

// remove_ctrl() with operator replaced by mutating assignment
std::string remove_ctrl_mutating(std::string s);
std::string remove_ctrl_mutating_it(std::string s);

std::string remove_ctrl_mutating_it_end(std::string s);

// remove_ctrl_mutating() with space reserved in result
std::string remove_ctrl_reserve(std::string s);
std::string remove_ctrl_reserve_it(std::string s);

// remove_ctrl_reserve() with reference arg instead of value arg
std::string remove_ctrl_refs(const std::string& s);

// remove_ctrl_reserve() with reference arg instead of value arg
std::string remove_ctrl_refs_it(const std::string& s);

// remove_ctrl_refs() with reference return value
void remove_ctrl_ref_result(std::string& result, const std::string& s);

// remove_ctrl_ref_ret with iterators
void remove_ctrl_ref_result_it(std::string& result, const std::string& s);

// remove_ctrl_ref_result() done with buffers instead of strings
void remove_ctrl_cstrings(char* destp, const char* sourcep, size_t length);

//  copy whole substrings to reduce allocations
std::string remove_ctrl_block(std::string s);

// remove_ctrl_block() with mutating concatenation, reserved size
std::string remove_ctrl_block_mutate(std::string s);

std::string remove_ctrl_block_append(std::string s);

std::string remove_ctrl_block_append_it(std::string s);

std::string remove_ctrl_block_append_cache(std::string s);

std::string remove_ctrl_block_append_cache_it(std::string s);

// remove_ctrl_block_mutate() with reference arg
std::string remove_ctrl_blocks(const std::string& s);
//  remove_ctrl_blocks() with reference return value
void remove_ctrl_block_ret(std::string& result, const std::string& s);

void remove_ctrl_block_ret_it(std::string& result, const std::string& s);

//  cleverly reduce the size of a string so it doesn't have to be reallocated
std::string remove_ctrl_erase(std::string s);
std::string remove_ctrl_erase_it(std::string s);
std::string remove_ctrl_erase_alg(std::string s);



#endif /* SRC_STRING_FUNCTIONS_H_ */
