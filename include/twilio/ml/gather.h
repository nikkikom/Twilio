#ifndef _TWILIO_ML_GATHER_H_
#define _TWILIO_ML_GATHER_H_

#include <twilio/ml/verbs_base.h>
#include <twilio/ml/method_param.h>
#include <twilio/detail/named_args.h>

#include <twilio/ml/say.h>
#include <twilio/ml/play.h>
#include <twilio/ml/pause.h>

namespace twilio {
namespace ml {
namespace verbs {

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

using Gather = verbs::Gather<>;

}} // namespace twilio::ml
#endif // _TWILIO_ML_GATHER_H_
