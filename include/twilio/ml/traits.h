#ifndef _TWILIO_ML_TRAITS_H_
#define _TWILIO_ML_TRAITS_H_

#include <type_traits>

#include <twilio/detail/compat.h>

namespace twilio {
namespace ml {
using namespace ::twilio::compat;

struct verb_tag {};

template <class T, class=void> struct is_verb : std::false_type {};

template <class T> struct is_verb<T
  , enable_if_t<std::is_convertible<T,verb_tag>::value>> : std::true_type {};

namespace verbs {
/// Check is Inner verb can be nested under Outer. 
// The actual specifications are in verb_*.h files
template <class Outer, class Inner> 
struct is_nestable: std::false_type {};
} // namespace verbs
}} // namespace twilio::ml

#endif // _TWILIO_ML_TRAITS_H_
