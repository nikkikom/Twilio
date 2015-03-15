#ifndef _TUPLE_H_
#define _TUPLE_H_

#include <tuple>
#include <utility>
#include <type_traits>

namespace xxx {

template<typename... Elements>
constexpr std::tuple<typename std::decay<Elements>::type...>
make_tuple(Elements&&... args)
{
  typedef std::tuple<typename std::decay<Elements>::type...> result_type;
  return result_type(std::forward<Elements>(args)...);
}

template<typename... Elements>
constexpr std::tuple<Elements&&...>
forward_as_tuple(Elements&&... args) noexcept
{ return std::tuple<Elements&&...> (std::forward<Elements>(args)...); }

}

#endif // _TUPLE_H_
