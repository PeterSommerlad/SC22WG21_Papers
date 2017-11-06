/*********************************************************************************
 * This file is part of CUTE.
 *
 * CUTE is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CUTE is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with CUTE.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright 2007-2013 Peter Sommerlad
 *
 *********************************************************************************/

#ifndef CUTE_DETERMINE_VERSION_H_
#define CUTE_DETERMINE_VERSION_H_

#if __cplusplus >= 201103L  && ! defined (USE_STD11)
#define USE_STD11 1
#endif

#if defined(__GNUG__) && defined(__GXX_EXPERIMENTAL_CXX0X__) && ! defined(USE_TR1) && ! defined(USE_STD11)
#define USE_STD11 1
#endif

#ifdef _MSC_VER
#if (_MSC_VER >= 1400)
#define USE_STD11 1
#endif
#endif

#if __cplusplus >= 201402L
#define USE_STD14
#endif
#endif /*CUTE_DETERMINE_VERSION_H_*/
