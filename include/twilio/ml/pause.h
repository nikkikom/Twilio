#ifndef _TWILIO_ML_PAUSE_H_
#define _TWILIO_ML_PAUSE_H_

#include <twilio/ml/verbs_base.h>
#include <twilio/detail/named_args.h>

namespace twilio {
namespace ml {
namespace verbs {
			
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

} // namespace verbs

using Pause = verbs::Pause<>;

}} // namespace twilio::ml
#endif // _TWILIO_ML_PAUSE_H_
