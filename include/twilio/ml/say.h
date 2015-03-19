#ifndef _TWILIO_ML_SAY_H_
#define _TWILIO_ML_SAY_H_

#include <twilio/ml/verbs_base.h>
#include <twilio/detail/named_args.h>

namespace twilio {
namespace ml {
using namespace ::twilio::compat;

namespace verbs {

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

} // namespace verbs

using Say = verbs::Say<>;

}} // namespace twilio::ml
#endif // _TWILIO_ML_SAY_H_
