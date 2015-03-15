#ifndef _TWILIO_DETAIL_IS_TUPLE_H_
#define _TWILIO_DETAIL_IS_TUPLE_H_

#include <type_traits>

namespace twilio {
namespace detail {

template <typename> struct is_tuple : std::false_type {};
template <typename... T> 
struct is_tuple<std::tuple<T...>> : std::true_type {};

}}

#endif // _TWILIO_DETAIL_IS_TUPLE_H_
