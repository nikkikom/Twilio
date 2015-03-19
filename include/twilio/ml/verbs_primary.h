#ifndef _VERBS_PRIMARY_H_
#define _VERBS_PRIMARY_H_
#include <ostream>
#include <iostream> // FIXME: debug
#include <utility>

#include <twilio/ml/traits.h>
#include <twilio/detail/named_args.h>
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

/// Response verb.
struct Response: base<Response>
{
	/// Constructs "non-empty" response.
	constexpr explicit Response (not_empty<Response> const& x) noexcept 
	  : base<Response> {false} 
	{}

  /// Constructs default response.
	constexpr explicit Response (bool empty = true) noexcept 
	  : base<Response> {empty} 
	{}

  /// The tag name.
	constexpr char const* name () const noexcept { return "Response"; }
};

/// Any verb can be nested under Response verb
template <class T>
struct is_nestable<Response, T> : std::true_type {};

/// Optional parameters for the Say verb.
struct SayParam
{
	/// Type of the voice.
	enum voice { man, woman, alice, voice_unset };

  /// Message language.
	enum language {
		fr_FR,
		da_DK,
		de_DE,
		en_US,
		ru_RU,

		lang_unset
	};

	static constexpr char const* const voice2string[3] 
#if !noconstexpr
	  = {
		  "man", "woman", "alice"
    }
#endif
  ;
	static constexpr char const* const language2string[5] 
#if !noconstexpr
	  = {
		  "fr_FR", "da_DK", "de_DE", "en_US", "ru_RU"
		}
#endif
	;
};

constexpr char const* const SayParam::voice2string[3]
#if noconstexpr
	  = {
		  "man", "woman", "alice"
    }
#endif
;

constexpr char const* const SayParam::language2string[5]
#if noconstexpr
	  = {
		  "fr_FR", "da_DK", "de_DE", "en_US", "ru_RU"
		}
#endif
;

/// Say verb.
template <
    typename Loop = nargs::field<unsigned int, 1>
  , typename Language = nargs::field<int, SayParam::lang_unset>
  , typename Voice = nargs::field<int, SayParam::voice_unset>
>
class Say
  : public base<Say<Loop, Language,Voice>>
  , public nargs::const_obj<Say, Loop, Language, Voice>
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

  constexpr unsigned int getLoop () const noexcept
  { return Say::template Get<0> (); }

  constexpr language getLanguage () const noexcept
  { return (language) Say::template Get<1> (); }

  constexpr voice getVoice () const noexcept
  { return (voice) Say::template Get<2> (); }

	constexpr explicit Say (char const* what) noexcept : what_ {what} {}

	constexpr char const* name () const noexcept { return "Say"; }
	constexpr bool empty () const noexcept { return false; }

  template <class Ch, class Tr>
  constexpr std::basic_ostream<Ch,Tr>&
  print_attrs (std::basic_ostream<Ch,Tr>& os) const
  {
  	return print_loop (print_language (print_voice (os)));
  }

  template <class Ch, class Tr>
  constexpr std::basic_ostream<Ch,Tr>&
  print_content (std::basic_ostream<Ch,Tr>& os) const
  {
  	return os << what_;
  }

protected:
  template <class Ch, class Tr>
  constexpr std::basic_ostream<Ch,Tr>&
  print_loop (std::basic_ostream<Ch,Tr>& os) const
  {
  	return (getLoop () > 1 ? os << " loop=\"" << getLoop () << "\"" : os);
  }

  template <class Ch, class Tr>
  constexpr std::basic_ostream<Ch,Tr>&
  print_language (std::basic_ostream<Ch,Tr>& os) const
  {
  	return (getLanguage () == lang_unset ? os :
  	  os << " language=\"" << language2string[getLanguage ()] << "\"");
  }

  template <class Ch, class Tr>
  constexpr std::basic_ostream<Ch,Tr>&
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
{
	char const* url_;

public:
  template <unsigned int _Loop>
  using setLoop = typename Play::template Set<0, _Loop>;

  constexpr unsigned int getLoop () const noexcept
  { return Play::template Get<0> (); }

	constexpr explicit Play (char const* url) noexcept : url_ {url} {}
	constexpr char const* name () const noexcept { return "Play"; }
	constexpr bool empty () const noexcept { return false; }

  template <class Ch, class Tr>
  constexpr std::basic_ostream<Ch,Tr>&
  print_attrs (std::basic_ostream<Ch,Tr>& os) const
  {
  	return print_loop (os);
  }

  template <class Ch, class Tr>
  constexpr std::basic_ostream<Ch,Tr>&
  print_content (std::basic_ostream<Ch,Tr>& os) const
  {
  	return os << url_;
  }

protected:
  template <class Ch, class Tr>
  constexpr std::basic_ostream<Ch,Tr>&
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
{
public:
  template <unsigned int _Length>
  using setLength = typename Pause::template Set<0, _Length>;

  constexpr unsigned int getLength () const noexcept
  { return Pause::template Get<0> (); }

	constexpr Pause () noexcept {}
	constexpr char const* name () const noexcept { return "Pause"; }

  template <class Ch, class Tr>
  constexpr std::basic_ostream<Ch,Tr>&
  print_attrs (std::basic_ostream<Ch,Tr>& os) const
  {
  	return (getLength () > 1 ? os << " length=\"" << getLength () << "\"" : os);
  }
};

struct MethodParam
{
	enum method { GET, POST };

	static constexpr char const* const method2string[2]
#if !noconstexpr
		= {
		  "GET", "POST"
		}
#endif
	;
};

constexpr char const* const MethodParam::method2string[2]
#if noconstexpr
		= {
		  "GET", "POST"
		}
#endif
;

template <
    typename Method = nargs::field<int, MethodParam::POST>
>
class Redirect
  : public base<Redirect<Method>>
  , public nargs::const_obj<Redirect, Method>
  , public MethodParam
{
	char const* url_;

public:
  template <unsigned int _Method>
  using setMethod = typename Redirect::template Set<0, _Method>;

  constexpr unsigned int getMethod () const noexcept
  { return Redirect::template Get<0> (); }

	constexpr explicit Redirect (char const* url) noexcept : url_ {url} {}
	constexpr char const* name () const noexcept { return "Redirect"; }
	constexpr bool empty () const noexcept { return false; }

  template <class Ch, class Tr>
  constexpr std::basic_ostream<Ch,Tr>&
  print_attrs (std::basic_ostream<Ch,Tr>& os) const
  {
  	return print_method (
  	       print_url (os));
  }

  template <class Ch, class Tr>
  constexpr std::basic_ostream<Ch,Tr>&
  print_content (std::basic_ostream<Ch,Tr>& os) const
  {
  	return os << url_;
  }

protected:
  template <class Ch, class Tr>
  constexpr std::basic_ostream<Ch,Tr>&
  print_method (std::basic_ostream<Ch,Tr>& os) const
  {
  	return (getMethod () == POST ? os :
  	  os << " method=\"" << method2string[getMethod ()] << "\"");
  }
}; // Redirect

template <
    typename Method = nargs::field<int, MethodParam::POST>
  , typename Timeout = nargs::field<unsigned int, 5>
  , typename FinishOnKey = nargs::field<char, '#'>
  , typename NumDigits = nargs::field<int, 0>
>
class Gather
  : public base<Gather<Method, Timeout, FinishOnKey, NumDigits>>
  , public nargs::const_obj<Gather, Method, Timeout, FinishOnKey, NumDigits>
  , public MethodParam
{
	char const* action_;

public:
  template <unsigned int _Method>
  using setMethod = typename Gather::template Set<0, _Method>;

  constexpr unsigned int getMethod () const noexcept
  { return Gather::template Get<0> (); }

  template <unsigned int _Timeout>
  using setTimeout = typename Gather::template Set<1, _Timeout>;

  constexpr unsigned int getTimeout () const noexcept
  { return Gather::template Get<1> (); }

  template <unsigned int _FinishOnKey>
  using setFinishOnKey = typename Gather::template Set<2, _FinishOnKey>;

  constexpr unsigned int getFinishOnKey () const noexcept
  { return Gather::template Get<2> (); }

  template <unsigned int _NumDigits>
  using setNumDigits = typename Gather::template Set<3, _NumDigits>;

  constexpr unsigned int getNumDigits () const noexcept
  { return Gather::template Get<3> (); }

	constexpr Gather (not_empty<Gather> const& x) noexcept 
	  : base<Gather> {false}
	  , action_ {x.action_}
	{}

	constexpr Gather (char const* action = nullptr) noexcept : action_ {action} {}

	constexpr Gather () noexcept {}
	constexpr char const* name () const noexcept { return "Gather"; }

  template <class Ch, class Tr>
  constexpr std::basic_ostream<Ch,Tr>&
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
  constexpr std::basic_ostream<Ch,Tr>&
  print_action (std::basic_ostream<Ch,Tr>& os) const
  {
  	return (! action_ ? os << "XXX" :
  	  os << " action=\"" << action_ << "\"");
  }

  template <class Ch, class Tr>
  constexpr std::basic_ostream<Ch,Tr>&
  print_timeout (std::basic_ostream<Ch,Tr>& os) const
  {
  	return (getTimeout () == 5 ? os :
  	  os << " timeout=\"" << getTimeout () << "\"");
  }

  template <class Ch, class Tr>
  constexpr std::basic_ostream<Ch,Tr>&
  print_finish_on_key (std::basic_ostream<Ch,Tr>& os) const
  {
  	return (getFinishOnKey () == '#' ? os :
  	  os << " finishOnKey=\"" << getFinishOnKey () << "\"");
  }

  template <class Ch, class Tr>
  constexpr std::basic_ostream<Ch,Tr>&
  print_num_digits (std::basic_ostream<Ch,Tr>& os) const
  {
  	return (getNumDigits () == 0 ? os :
  	  os << " numDigits=\"" << getNumDigits () << "\"");
  }

  template <class Ch, class Tr>
  constexpr std::basic_ostream<Ch,Tr>&
  print_method (std::basic_ostream<Ch,Tr>& os) const
  {
  	return (getMethod () == POST ? os :
  	  os << " method=\"" << method2string[getMethod ()] << "\"");
  }
}; // Gather

// Say and Play verbs can be nested under Gather
template <class Method, class Timeout, class Finish, class Digits,
          class Loop, class Lang, class Voice>
struct is_nestable<
    Gather<Method,Timeout,Finish,Digits>
  , Say<Loop,Lang,Voice>
> : std::true_type {};

template <class Method, class Timeout, class Finish, class Digits, class Loop>
struct is_nestable<
    Gather<Method,Timeout,Finish,Digits>
  , Play<Loop>
> : std::true_type {};

template <class Method, class Timeout, class Finish, class Digits, class Len>
struct is_nestable<
    Gather<Method,Timeout,Finish,Digits>
  , Pause<Len>
> : std::true_type {};


} // namespace verbs

template <typename... Ts>
constexpr verbs::Response Response (Ts&&... ts)
{
	return verbs::Response {std::forward<Ts> (ts)...};
}

using Say = verbs::Say<>;
using Play = verbs::Play<>;
using Pause = verbs::Pause<>;
using Gather = verbs::Gather<>;

}} // namespace twilio::ml
#endif // _VERBS_PRIMARY_H_
