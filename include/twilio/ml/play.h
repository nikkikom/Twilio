#ifndef _TWILIO_ML_PLAY_H_
#define _TWILIO_ML_PLAY_H_

#include <twilio/ml/verbs_base.h>
#include <twilio/detail/named_args.h>

namespace twilio {
namespace ml {
namespace verbs {
			
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

} // namespace verbs

using Play = verbs::Play<>;

}} // namespace twilio::ml
#endif // _TWILIO_ML_PLAY_H_
