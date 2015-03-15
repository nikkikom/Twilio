#ifndef _VERBS_PRIMARY_H_
#define _VERBS_PRIMARY_H_
#include <ostream>
#include <iostream> // FIXME: debug
#include <utility>

#include <twilio/ml/traits.h>
#include <twilio/detail/named_args.h>

namespace twilio {
namespace ml {
namespace verbs {

template <class Derived>
class base
{
	bool empty_;

	_constexpr Derived const* derived () const noexcept
  { return static_cast<Derived const*> (this); }

  template <class Ch, class Tr>
  friend _constexpr std::basic_ostream<Ch,Tr>&
  print (std::basic_ostream<Ch,Tr>& os, base const& b)
  {
  	return b.derived ()->print_end (
  	       b.derived ()->print_content (
  	       b.derived ()->print_begin (os)));
  }

  // This operator should not be called, an error will always be triggered.
  template <class Ch, class Tr>
  friend _constexpr std::basic_ostream<Ch,Tr>&
  operator<< (std::basic_ostream<Ch,Tr>& os, base const& b)
  {
  	static_assert (!std::is_same<base,base>::value, 
  	    "Use parnsis, leaf cannot be streamed directly");
  	return os;
  }

public:
  _constexpr base (bool empty = true) noexcept : empty_ (empty) {}

	_constexpr bool empty () const noexcept { return empty_; }

  template <class Ch, class Tr>
  _constexpr std::basic_ostream<Ch,Tr>&
  print_begin (std::basic_ostream<Ch,Tr>& os) const
  {
  	return derived ()->print_attrs (os << '<' << derived ()->name ()) 
  	  << (derived ()->empty () ? "/" : "") << '>';
  }

  template <class Ch, class Tr>
  _constexpr std::basic_ostream<Ch,Tr>&
  print_end (std::basic_ostream<Ch,Tr>& os) const
  {
  	return derived ()->empty () ? os : 
  	          os << '<' << '/' << derived ()->name () << '>';
  }

  template <class Ch, class Tr>
  _constexpr std::basic_ostream<Ch,Tr>&
  print_attrs (std::basic_ostream<Ch,Tr>& os) const noexcept
  {
  	return os;
  }

  template <class Ch, class Tr>
  _constexpr std::basic_ostream<Ch,Tr>&
  print_content (std::basic_ostream<Ch,Tr>& os) const noexcept
  {
  	return os;
  }

};

template <class Base>
struct not_empty: public Base 
{
  using Base::Base;
  _constexpr not_empty (Base const& base) : Base (base) {}
  _constexpr not_empty (Base && base) : Base (std::move (base)) {}
};

template <class Base>
_constexpr not_empty<typename std::decay<Base>::type>
set_not_empty (Base&& base)
{
	typedef typename std::decay<Base>::type _Base;
	static_assert (is_leaf<_Base>::value, "argument should be Verb");

	return not_empty<_Base> (std::forward<Base> (base)); 
}

struct Response: base<Response>, node_tag
{
	_constexpr Response (not_empty<Response> const& x) noexcept 
	  : base<Response> (false) {}

	_constexpr Response (bool empty = true) noexcept : base<Response> (empty) 
	{}

	_constexpr char const* name () const noexcept { return "Response"; }
};

struct SayParam
{
	enum voice { man, woman, alice, voice_unset };
	enum language {
		fr_FR,
		da_DK,
		de_DE,
		en_US,
		ru_RU,

		lang_unset
	};

	static _constexpr char const* const voice2string[3] 
#if !no_constexpr
	  = {
		  "man", "woman", "alice"
    }
#endif
  ;
	static _constexpr char const* const language2string[5] 
#if !no_constexpr
	  = {
		  "fr_FR", "da_DK", "de_DE", "en_US", "ru_RU"
		}
#endif
	;
};

_constexpr char const* const SayParam::voice2string[3]
#if no_constexpr
	  = {
		  "man", "woman", "alice"
    }
#endif
;

_constexpr char const* const SayParam::language2string[5]
#if no_constexpr
	  = {
		  "fr_FR", "da_DK", "de_DE", "en_US", "ru_RU"
		}
#endif
;

template <
    typename Loop = nargs::field<unsigned int, 1>
  , typename Language = nargs::field<int, SayParam::lang_unset>
  , typename Voice = nargs::field<int, SayParam::voice_unset>
>
class Say
  : public base<Say<Loop, Language,Voice>>
  , public nargs::const_obj<Say, Loop, Language, Voice>
  , public leaf_tag
  , public SayParam
{
	char const* what_;

public: 
  template <unsigned int _Loop>
  using setLoop = typename Say::template Set<0, _Loop>;

  template <unsigned int _Language>
  using setLanguage = typename Say::template Set<1, _Language>;

  template <unsigned int _Voice>
  using setVoice = typename Say::template Set<2, _Voice>;

  _constexpr unsigned int getLoop () const noexcept
  { return Say::template Get<0> (); }

  _constexpr language getLanguage () const noexcept
  { return (language) Say::template Get<1> (); }

  _constexpr voice getVoice () const noexcept
  { return (voice) Say::template Get<2> (); }

	_constexpr Say (char const* what) noexcept : what_ (what) {}

	_constexpr char const* name () const noexcept { return "Say"; }
	_constexpr bool empty () const noexcept { return false; }

  template <class Ch, class Tr>
  _constexpr std::basic_ostream<Ch,Tr>&
  print_attrs (std::basic_ostream<Ch,Tr>& os) const
  {
  	return print_loop (print_language (print_voice (os)));
  }

  template <class Ch, class Tr>
  _constexpr std::basic_ostream<Ch,Tr>&
  print_content (std::basic_ostream<Ch,Tr>& os) const
  {
  	return os << what_;
  }

protected:
  template <class Ch, class Tr>
  _constexpr std::basic_ostream<Ch,Tr>&
  print_loop (std::basic_ostream<Ch,Tr>& os) const
  {
  	return (getLoop () > 1 ? os << " loop=\"" << getLoop () << "\"" : os);
  }

  template <class Ch, class Tr>
  _constexpr std::basic_ostream<Ch,Tr>&
  print_language (std::basic_ostream<Ch,Tr>& os) const
  {
  	return (getLanguage () == lang_unset ? os :
  	  os << " language=\"" << language2string[getLanguage ()] << "\"");
  }

  template <class Ch, class Tr>
  _constexpr std::basic_ostream<Ch,Tr>&
  print_voice (std::basic_ostream<Ch,Tr>& os) const
  {
  	return (getVoice () == voice_unset ? os :
  	    os << " loop=\"" << voice2string[getVoice ()] << "\"");
  }
};

template <
    typename Loop = nargs::field<unsigned int, 1>
>
class Play
  : public base<Play<Loop>>
  , public nargs::const_obj<Play, Loop>
  , public leaf_tag
{
	char const* url_;

public:
  template <unsigned int _Loop>
  using setLoop = typename Play::template Set<0, _Loop>;

  _constexpr unsigned int getLoop () const noexcept
  { return Play::template Get<0> (); }

	_constexpr Play (char const* url) noexcept : url_ (url) {}
	_constexpr char const* name () const noexcept { return "Play"; }
	_constexpr bool empty () const noexcept { return false; }

  template <class Ch, class Tr>
  _constexpr std::basic_ostream<Ch,Tr>&
  print_attrs (std::basic_ostream<Ch,Tr>& os) const
  {
  	return print_loop (os);
  }

  template <class Ch, class Tr>
  _constexpr std::basic_ostream<Ch,Tr>&
  print_content (std::basic_ostream<Ch,Tr>& os) const
  {
  	return os << url_;
  }

protected:
  template <class Ch, class Tr>
  _constexpr std::basic_ostream<Ch,Tr>&
  print_loop (std::basic_ostream<Ch,Tr>& os) const
  {
  	return (getLoop () > 1 ? os << " loop=\"" << getLoop () << "\"" : os);
  }
};

template <
    typename Length = nargs::field<unsigned int, 1>
>
 class Pause
  : public base<Pause<Length>>
  , public nargs::const_obj<Pause, Length>
  , public leaf_tag
{
public:
  template <unsigned int _Length>
  using setLength = typename Pause::template Set<0, _Length>;

  _constexpr unsigned int getLength () const noexcept
  { return Pause::template Get<0> (); }

	_constexpr Pause () noexcept {}
	_constexpr char const* name () const noexcept { return "Pause"; }

  template <class Ch, class Tr>
  _constexpr std::basic_ostream<Ch,Tr>&
  print_attrs (std::basic_ostream<Ch,Tr>& os) const
  {
  	return (getLength () > 1 ? os << " length=\"" << getLength () << "\"" : os);
  }
};

struct GatherParam
{
	enum method { GET, POST };

	static _constexpr char const* const method2string[2]
#if !no_constexpr
		= {
		  "GET", "POST"
		}
#endif
	;
};

_constexpr char const* const GatherParam::method2string[2]
#if no_constexpr
		= {
		  "GET", "POST"
		}
#endif
;

template <
    typename Method = nargs::field<int, GatherParam::POST>
  , typename Timeout = nargs::field<unsigned int, 5>
  , typename FinishOnKey = nargs::field<char, '#'>
  , typename NumDigits = nargs::field<int, 0>
>
class Gather
  : public base<Gather<Method, Timeout, FinishOnKey, NumDigits>>
  , public nargs::const_obj<Gather, Method, Timeout, FinishOnKey, NumDigits>
  , public node_tag
  , public GatherParam
{
	char const* action_;

public:
  template <unsigned int _Method>
  using setMethod = typename Gather::template Set<0, _Method>;

  _constexpr unsigned int getMethod () const noexcept
  { return Gather::template Get<0> (); }

  template <unsigned int _Timeout>
  using setTimeout = typename Gather::template Set<1, _Timeout>;

  _constexpr unsigned int getTimeout () const noexcept
  { return Gather::template Get<1> (); }

  template <unsigned int _FinishOnKey>
  using setFinishOnKey = typename Gather::template Set<2, _FinishOnKey>;

  _constexpr unsigned int getFinishOnKey () const noexcept
  { return Gather::template Get<2> (); }

  template <unsigned int _NumDigits>
  using setNumDigits = typename Gather::template Set<3, _NumDigits>;

  _constexpr unsigned int getNumDigits () const noexcept
  { return Gather::template Get<3> (); }

	_constexpr Gather (not_empty<Gather> const& x) noexcept 
	  : base<Gather> (false)
	  , action_ (x.action_)
	{}

	_constexpr Gather (char const* action = nullptr) : action_ {action} {}

	_constexpr Gather () noexcept {}
	_constexpr char const* name () const noexcept { return "Gather"; }

  template <class Ch, class Tr>
  _constexpr std::basic_ostream<Ch,Tr>&
  print_attrs (std::basic_ostream<Ch,Tr>& os) const
  {
  	return print_num_digits (
  	       print_finish_on_key (
  	       print_timeout (
  	       print_method (
  	       print_action (os)))));
  }

protected:
  template <class Ch, class Tr>
  _constexpr std::basic_ostream<Ch,Tr>&
  print_action (std::basic_ostream<Ch,Tr>& os) const
  {
  	return (! action_ ? os << "XXX" :
  	  os << " action=\"" << action_ << "\"");
  }

  template <class Ch, class Tr>
  _constexpr std::basic_ostream<Ch,Tr>&
  print_timeout (std::basic_ostream<Ch,Tr>& os) const
  {
  	return (getTimeout () == 5 ? os :
  	  os << " timeout=\"" << getTimeout () << "\"");
  }

  template <class Ch, class Tr>
  _constexpr std::basic_ostream<Ch,Tr>&
  print_finish_on_key (std::basic_ostream<Ch,Tr>& os) const
  {
  	return (getFinishOnKey () == '#' ? os :
  	  os << " finishOnKey=\"" << getFinishOnKey () << "\"");
  }

  template <class Ch, class Tr>
  _constexpr std::basic_ostream<Ch,Tr>&
  print_num_digits (std::basic_ostream<Ch,Tr>& os) const
  {
  	return (getNumDigits () == 0 ? os :
  	  os << " numDigits=\"" << getNumDigits () << "\"");
  }

  template <class Ch, class Tr>
  _constexpr std::basic_ostream<Ch,Tr>&
  print_method (std::basic_ostream<Ch,Tr>& os) const
  {
  	return (getMethod () == POST ? os :
  	  os << " method=\"" << method2string[getMethod ()] << "\"");
  }

};

} // namespace verbs

template <typename... Ts>
_constexpr verbs::Response Response (Ts&&... ts)
{
	return verbs::Response (std::forward<Ts> (ts)...);
}

using Say = verbs::Say<>;
using Play = verbs::Play<>;
using Pause = verbs::Pause<>;
using Gather = verbs::Gather<>;

}} // namespace twilio::ml
#endif // _VERBS_PRIMARY_H_
