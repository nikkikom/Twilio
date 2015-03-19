#ifndef _VERBS_BASE_H_
#define _VERBS_BASE_H_
#include <ostream>
#include <utility>

#include <twilio/ml/traits.h>
#include <twilio/detail/compat.h>

namespace twilio {
namespace ml {
using namespace ::twilio::compat;

namespace verbs {

/// Base class for all verbs
template <class Derived>
class base: public verb_tag
{
	bool empty_;

  /// Curiously recurring template pattern is here.
	constexpr Derived const* derived () const noexcept
  { return static_cast<Derived const*> (this); }
  
  /// Prints verb nodes's prefix, content and suffix.
  template <class Ch, class Tr>
  friend constexpr std::basic_ostream<Ch,Tr>&
  print (std::basic_ostream<Ch,Tr>& os, base const& b)
  {
  	return b.derived ()->print_end (
  	       b.derived ()->print_content (
  	       b.derived ()->print_begin (os)));
  }

  /// This operator should not be called, an error will always be triggered.
  template <class Ch, class Tr>
  friend constexpr std::basic_ostream<Ch,Tr>&
  operator<< (std::basic_ostream<Ch,Tr>& os, base const& b)
  {
  	static_assert (!std::is_same<base,base>::value, 
  	    "Use parnsis, leaf cannot be streamed directly");
  	return os;
  }

public:
  constexpr base (bool empty = true) noexcept : empty_ {empty} {}

  /// Returs true if the verb has nouns.
  // Non-empty status changes the way of how start and end tags will be printed.
	constexpr bool empty () const noexcept { return empty_; }

  
  /// Prints the prefix tag.
  template <class Ch, class Tr>
  constexpr std::basic_ostream<Ch,Tr>&
  print_begin (std::basic_ostream<Ch,Tr>& os) const
  {
  	return derived ()->print_attrs (os << '<' << derived ()->name ()) 
  	  << (derived ()->empty () ? "/" : "") << '>';
  }

  /// Prints the suffix tag.
  template <class Ch, class Tr>
  constexpr std::basic_ostream<Ch,Tr>&
  print_end (std::basic_ostream<Ch,Tr>& os) const
  {
  	return derived ()->empty () ? os : 
  	          os << '<' << '/' << derived ()->name () << '>';
  }

  /// Prints the verbs attributes.
  template <class Ch, class Tr>
  constexpr std::basic_ostream<Ch,Tr>&
  print_attrs (std::basic_ostream<Ch,Tr>& os) const noexcept
  {
  	return os;
  }

  /// Prints the content.
  template <class Ch, class Tr>
  constexpr std::basic_ostream<Ch,Tr>&
  print_content (std::basic_ostream<Ch,Tr>& os) const noexcept
  {
  	return os;
  }

};

/// Class wrapper that makes "non-empty" verb from "empty" one.
template <class Base>
struct not_empty: public Base 
{
  using Base::Base;
  constexpr not_empty (Base const& base) : Base { base             } {}
  constexpr not_empty (Base     && base) : Base { std::move (base) } {}
};

template <class Base>
constexpr not_empty<decay_t<Base>>
set_not_empty (Base&& base)
{
	typedef decay_t<Base> _Base;
	static_assert (is_verb<_Base>::value, "argument should be Verb");

	return not_empty<_Base> { std::forward<Base> (base) }; 
}

} // namespace verbs
}} // namespace twilio::ml
#endif // _VERBS_BASE_H_
