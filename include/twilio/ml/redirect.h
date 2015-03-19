#ifndef _TWILIO_ML_REDIRECT_H_
#define _TWILIO_ML_REDIRECT_H_

#include <twilio/ml/verbs_base.h>
#include <twilio/detail/named_args.h>
#include <twilio/ml/method_param.h>

namespace twilio {
namespace ml {
namespace verbs {

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

} // namespace verbs

using Redirect = verbs::Redirect<>;

}} // namespace twilio::ml
#endif // _TWILIO_ML_REDIRECT_H_
