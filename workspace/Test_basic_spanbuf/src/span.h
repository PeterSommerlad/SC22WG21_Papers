#ifndef SRC_SPAN_H_
#define SRC_SPAN_H_
// an experimental minimal implementation of the span<T> proposal
// not guaranteed to work with anythin except span<char> :-), because I use it to implement spanstream/spanbuf
// (c) 2016 Peter Sommerlad
//#include <bits/c++config.h>
#include <iterator>
#include <array>
#include <type_traits>

namespace std //_GLIBCXX_VISIBILITY(default)
{
//_GLIBCXX_BEGIN_NAMESPACE_VERSION
//_GLIBCXX_BEGIN_NAMESPACE_CXX11
namespace experimental{
// type byte not defined yet:
enum class byte:unsigned char{};

// [views.constants], constants 
constexpr ptrdiff_t dynamic_extent = -1;

//// [span], class template span
//template <class ElementType, ptrdiff_t Extent = dynamic_extent> class span;
//
// 
//// [span.comparison], span comparison operators 
//template <class ElementType, ptrdiff_t Extent>
//	constexpr bool operator==(span<ElementType, Extent> const & l, span<ElementType, Extent> const & r) const noexcept;
//  
//template <class ElementType, ptrdiff_t Extent>
//	constexpr bool operator!=(span<ElementType, Extent> const & l, span<ElementType, Extent> const & r) const noexcept;
// 
//template <class ElementType, ptrdiff_t Extent>
//	constexpr bool operator<(span<ElementType, Extent> const & l, span<ElementType, Extent> const & r) const noexcept;
// 
//template <class ElementType, ptrdiff_t Extent>
//	constexpr bool operator<=(span<ElementType, Extent> const & l, span<ElementType, Extent> const & r) const noexcept;
//  template <class ElementType, ptrdiff_t Extent>
//	constexpr bool operator>(span<ElementType, Extent> const & l, span<ElementType, Extent> const & r) const noexcept;
//  template <class ElementType, ptrdiff_t Extent>
//	constexpr bool operator>=(span<ElementType, Extent> const & l, span<ElementType, Extent> const & r) const noexcept;
//  // [span.objectrep], views of object representation 
//  template <class ElementType, ptrdiff_t Extent>
//  constexpr span<char, ((Extent == dynamic_extent) ? dynamic_extent :
//     (sizeof(ElementType) * Extent))> as_writeable_bytes(span<ElementType, Extent> ) noexcept;
//  
  // A view over a contiguous, single-dimension sequence of objects 
template <typename ElementType, ptrdiff_t Extent = dynamic_extent> 
class span {
  public:
    
   
   // constants and types
  using element_type = ElementType;
  using index_type = ptrdiff_t;
  using difference_type = ptrdiff_t;
  using size_type = size_t;
  using pointer = element_type*;
  using reference = element_type&;
  using iterator = element_type*; /*implementation-defined */;
  using const_iterator = element_type const*; /*implementation-defined */;
  using reverse_iterator = std::reverse_iterator<iterator>;  
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;  
  
  constexpr static index_type extent = Extent;
  // [span.cons], span constructors, copy, assignment, and destructor 
  constexpr span(){} // should SFINAE on Extent
  constexpr span(nullptr_t){} // should SFINAE on Extent 
  constexpr span(pointer ptr, index_type count):data_{ptr},size_{count}{}
  constexpr span(pointer firstElem, pointer lastElem):data_{firstElem},size_{lastElem-firstElem}{
	  // check preconditions
  }
  template <size_t N, typename = enable_if_t<N==Extent||Extent==dynamic_extent>>
  constexpr span(element_type (&arr)[N]):data_{&arr[0]},size_{N}{} // should SFINAE on Extent==N or Extent == dynamic_extent
  template <size_t N, typename = enable_if_t<N==Extent||Extent==dynamic_extent>>
  constexpr span(array<std::remove_const_t<element_type>, N>& arr)
  :data_{&arr[0]},size_{N}{ } // should SFINAE on Extent==N or Extent == dynamic_extent
  template <size_t N, typename = enable_if_t<is_const<element_type>::value && (N==Extent||Extent==dynamic_extent) >>
  constexpr span(array<remove_const_t<element_type>, N> const & arr):data_{&arr[0]},size_{N}{} // should SFINAE on Extent==N or Extent == dynamic_extent

// TODO: too complex sfinae rules for the moment  
//  template<class Container>
//    constexpr span(Container& cont):data_{cont.data()},size_{cont.size()}{} // SFINAE on requirements
//  template <class Container, typename = enable_if_t<is_const<element_type>::value>>
//	constexpr span(Container const &);
		
  constexpr span(span const & other) noexcept = default;
  constexpr span(span&& other) noexcept = default;
  
//  template<class OtherElementType, ptrdiff_t OtherExtent>
//  constexpr span(span<OtherElementType, OtherExtent> const & other);
//  template<class OtherElementType, ptrdiff_t OtherExtent>
//  constexpr span(span<OtherElementType, OtherExtent>&& other); 
  	  ~span() noexcept = default;
		constexpr span& operator=(span const & other) noexcept = default;
		constexpr span& operator=(span&& other) noexcept = default;
     // [span.sub], span subviews
template <ptrdiff_t Count>
  constexpr span<element_type, Count> first() const { check_incl(Count); return {data_,Count};} 
template <ptrdiff_t Count>
  constexpr span<element_type, Count> last() const { check_incl(Count); return {Count==0?data_:data_+size()-Count,Count};}
template <ptrdiff_t Offset, ptrdiff_t Count = dynamic_extent>
  constexpr span<element_type, Count> subspan() const{
	check(Offset);
	if (Count == dynamic_extent || (Count >= 0 && Offset + Count <= size())){
		return {data_+Offset, Count==dynamic_extent?size()-Offset:Count};
	} else {
			check(-1);
	}
}

constexpr span<element_type, dynamic_extent> first(index_type count) const;
constexpr span<element_type, dynamic_extent> last(index_type count)  const;
constexpr span<element_type, dynamic_extent> subspan(index_type
   offset, index_type count = dynamic_extent) const;
  // [span.obs], span observers
constexpr index_type length() const noexcept { return size();} 
constexpr index_type size() const noexcept { return size_;}
constexpr index_type length_bytes() const noexcept { return size_bytes();}
constexpr index_type size_bytes() const noexcept{ return size()*sizeof(ElementType);}
constexpr bool empty() const noexcept { return 0==size();}
// [span.elem], span element access
constexpr reference operator[](index_type idx) const { check(idx); return data_[idx];}
//constexpr reference operator()(index_type idx) const; 
constexpr pointer data() const noexcept { return data_;}
// [span.iter], span iterator support
  
iterator begin() const noexcept{ return data_;}
iterator end() const noexcept { return data_+size();}
const_iterator cbegin() const noexcept{ return data_;}
const_iterator cend() const noexcept { return data_+size();}
reverse_iterator rbegin() const noexcept {return reverse_iterator{data_+size()};}
reverse_iterator rend() const noexcept {return reverse_iterator{data_};}
const_reverse_iterator crbegin() const noexcept {return reverse_iterator{data_+size()};}
const_reverse_iterator crend() const noexcept {return reverse_iterator{data_};}

  private:
void check(index_type idx) const {
	if (idx < 0 || idx >= size()) throw std::out_of_range{"span: " + to_string(idx)+">="+ to_string(size())};
}
void check_incl(index_type idx) const {
	if (idx < 0 || idx > size()) throw std::out_of_range{"span: " + to_string(idx)+">"+ to_string(size())};
}

// exposition only
pointer data_{nullptr};
index_type size_{0};
};
// [span.comparison], span comparison operators 
template <class ElementType, ptrdiff_t Extent>
	constexpr bool operator==(span<ElementType, Extent> const & l, span<ElementType, Extent> const & r)  noexcept;
template <class ElementType, ptrdiff_t Extent>
	constexpr bool operator!=(span<ElementType, Extent> const & l, span<ElementType, Extent> const & r)  noexcept;
template <class ElementType, ptrdiff_t Extent>
	constexpr bool operator<(span<ElementType, Extent> const & l, span<ElementType, Extent> const & r)  noexcept;
template <class ElementType, ptrdiff_t Extent>
	constexpr bool operator<=(span<ElementType, Extent> const & l, span<ElementType, Extent> const & r)  noexcept;
template <class ElementType, ptrdiff_t Extent>
	constexpr bool operator>(span<ElementType, Extent> const & l, span<ElementType, Extent> const & r)  noexcept;
template <class ElementType, ptrdiff_t Extent>
	constexpr bool operator>=(span<ElementType, Extent> const & l, span<ElementType, Extent> const & r)  noexcept;
// [span.objectrep], views of object representation 
template <class ElementType, ptrdiff_t Extent>
constexpr span<byte const, ((Extent == dynamic_extent) ? dynamic_extent : (sizeof(ElementType) * Extent))> 
as_bytes(span<ElementType, Extent> s)	noexcept;
template <class ElementType, ptrdiff_t Extent>
constexpr 
span<byte, ((Extent == dynamic_extent) ? dynamic_extent : (sizeof(ElementType) * Extent))> 
as_writeable_bytes(span<ElementType, Extent> ) noexcept;
}
//_GLIBCXX_END_NAMESPACE_VERSION
} // namespace std





#endif /* SRC_SPAN_H_ */
