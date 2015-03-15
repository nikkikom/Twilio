#ifndef _TWILIO_DETAIL_PRINT_TUPLE_H_
#define _TWILIO_DETAIL_PRINT_TUPLE_H_
// See http://stackoverflow.com/questions/6245735/pretty-print-stdtuple

#include <cstddef>
#include <tuple>
#include <ostream>

namespace twilio {
namespace detail {

template <std::size_t...> struct seq {};

template <std::size_t N, std::size_t... Is>
struct gen_seq : gen_seq<N-1, N-1, Is...> {};

template <std::size_t... Is>
struct gen_seq<0, Is...> : seq<Is...> {};

template <class Ch, class Tr, class Tuple, std::size_t... Is>
void print_tuple (std::basic_ostream<Ch,Tr>& os, Tuple const& t, seq<Is...>)
{
	using swallow = int[];
	(void) swallow { 0, 
	  (void (print (os<< (Is == 0 ? "" : "\n"), std::get<Is> (t))), 0)...};
}

}} // namespace detail

namespace std {

template<class Ch, class Tr, class... Args>
std::basic_ostream<Ch, Tr>&
 operator<<(std::basic_ostream<Ch, Tr>& os, std::tuple<Args...> const& t)
{
  twilio::detail::print_tuple(os, t, 
      twilio::detail::gen_seq<sizeof...(Args)>());
  return os;
}

}
#endif // _TWILIO_DETAIL_PRINT_TUPLE_H_
