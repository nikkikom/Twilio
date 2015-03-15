#ifndef _TWILIO_ML_TRAITS_H_
#define _TWILIO_ML_TRAITS_H_

#include <type_traits>

namespace twilio {
namespace ml {

struct leaf_tag {};
struct node_tag : leaf_tag {};

template <class T, class=void> struct is_leaf : std::false_type {};

template <class T> struct is_leaf<T
  , typename std::enable_if<std::is_convertible<T,leaf_tag>::value>::type>
  : std::true_type {};

template <class T, class=void> struct is_node : std::false_type {};

template <class T> struct is_node<T
  , typename std::enable_if<std::is_convertible<T,node_tag>::value>::type>
  : std::true_type {};

}}

#endif // _TWILIO_ML_TRAITS_H_
