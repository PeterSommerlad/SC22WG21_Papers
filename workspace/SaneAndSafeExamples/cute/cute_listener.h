/*********************************************************************************
 * This file is part of CUTE.
 *
 * Copyright (c) 2007-2018 Peter Sommerlad, IFS
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 *********************************************************************************/

#ifndef CUTE_LISTENER_H_
#define CUTE_LISTENER_H_
#include "cute_base.h"
#include "cute_suite.h"
namespace cute {
	struct null_listener{ // defines Contract of runner parameter
		void begin(suite const &, char const * /*info*/, size_t /*n_of_tests*/){}
		void end(suite const &, char const * /*info*/){}
		void start(test const &){}
		void success(test const &,char const * /*msg*/){}
		void failure(test const &,test_failure const &){}
		void error(test const &,char const * /*what*/){}
	};
}
#endif /* CUTE_LISTENER_H_ */

