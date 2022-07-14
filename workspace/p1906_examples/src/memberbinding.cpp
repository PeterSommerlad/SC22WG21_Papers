#include <array>
#include <type_traits>
#include <iostream>

using one = char;
static_assert(1==sizeof(one));
using two = std::array<char,2>;
static_assert(sizeof(two)==2);
using three = std::array<char,3>;
static_assert(sizeof(three)==3);
using four = std::array<char,4>;
static_assert(sizeof(four)==4);

struct Y{
	one m() { return {};}

	one
	mf() { return {};}
	two mf() const { return {};}

	one mfc() const { return {};}

    one mfr() & { return {};}
    two mfr() && { return {};}
    three mfr() const & { return {};}
    four mfr() const && { return {};}

    one mfl() & { return {};}
    two mfl() const & { return {};}

    one mflo() & { return {};}


    one mflc() const & { return {};}

    one mfx() && { return {};}
    two mfx() const && { return {};}

    one mfxnc() && { return {};}

};
static_assert(1 == sizeof(std::declval<Y>().m()));
//static_assert(1 == sizeof(std::declval<Y const>().m()));
static_assert(1 == sizeof(std::declval<Y&>().m()));
static_assert(1 == sizeof(std::declval<Y&&>().m()));
//static_assert(1 == sizeof(std::declval<Y const &>().m()));
//static_assert(1 == sizeof(std::declval<Y const &&>().m()));


static_assert(1 == sizeof(std::declval<Y>().mf()));
static_assert(2 == sizeof(std::declval<Y const>().mf()));
static_assert(1 == sizeof(std::declval<Y&>().mf()));
static_assert(1 == sizeof(std::declval<Y&&>().mf()));
static_assert(2 == sizeof(std::declval<Y const &>().mf()));
static_assert(2 == sizeof(std::declval<Y const &&>().mf()));

static_assert(1 == sizeof(std::declval<Y>().mfc()));
static_assert(1 == sizeof(std::declval<Y const>().mfc()));
static_assert(1 == sizeof(std::declval<Y&>().mfc()));
static_assert(1 == sizeof(std::declval<Y&&>().mfc()));
static_assert(1 == sizeof(std::declval<Y const &>().mfc()));
static_assert(1 == sizeof(std::declval<Y const &&>().mfc()));


static_assert(2 == sizeof(std::declval<Y>().mfr()));
static_assert(4 == sizeof(std::declval<Y const>().mfr()));
static_assert(1 == sizeof(std::declval<Y &>().mfr()));
static_assert(2 == sizeof(std::declval<Y &&>().mfr()));
static_assert(3 == sizeof(std::declval<Y const &>().mfr()));
static_assert(4 == sizeof(std::declval<Y const &&>().mfr()));

static_assert(2 == sizeof(std::declval<Y>().mfl()));
static_assert(2 == sizeof(std::declval<Y const>().mfl()));
static_assert(1 == sizeof(std::declval<Y&>().mfl()));
static_assert(2 == sizeof(std::declval<Y&&>().mfl()));
static_assert(2 == sizeof(std::declval<Y const &>().mfl()));
static_assert(2 == sizeof(std::declval<Y const &&>().mfl()));

static_assert(1 == sizeof(std::declval<Y>().mflc()));
static_assert(1 == sizeof(std::declval<Y const>().mflc()));
static_assert(1 == sizeof(std::declval<Y&>().mflc()));
static_assert(1 == sizeof(std::declval<Y&&>().mflc()));
static_assert(1 == sizeof(std::declval<Y const &>().mflc()));
static_assert(1 == sizeof(std::declval<Y const &&>().mflc()));

//static_assert(1 == sizeof(std::declval<Y>().mflo())); // would bind to a value parameter, because that is a Y&
//static_assert(1 == sizeof(std::declval<Y const>().mflo()));
static_assert(1 == sizeof(std::declval<Y&>().mflo()));
//static_assert(1 == sizeof(std::declval<Y&&>().mflo()));
//static_assert(1 == sizeof(std::declval<Y const &>().mflo()));
//static_assert(1 == sizeof(std::declval<Y const &&>().mflo()));

static_assert(1 == sizeof(std::declval<Y>().mfx()));
static_assert(2 == sizeof(std::declval<Y const>().mfx()));
//static_assert(1 == sizeof(std::declval<Y&>().mfx()));
static_assert(1 == sizeof(std::declval<Y&&>().mfx()));
//static_assert(2 == sizeof(std::declval<Y const &>().mfx()));
static_assert(2 == sizeof(std::declval<Y const &&>().mfx()));

static_assert(1 == sizeof(std::declval<Y>().mfxnc()));
//static_assert(1 == sizeof(std::declval<Y const>().mfxnc()));
//static_assert(1 == sizeof(std::declval<Y&>().mfxnc()));
static_assert(1 == sizeof(std::declval<Y&&>().mfxnc()));
//static_assert(1 == sizeof(std::declval<Y const &>().mfxnc()));
//static_assert(1 == sizeof(std::declval<Y const &&>().mfxnc()));



void foo(){
    auto const & x = std::min(42,43);
    auto p = std::array{1,2,3,4,5,6}.data();
    std::cout << *p << x;

}
