#ifndef _TWILIO_ML_H_
#define _TWILIO_ML_H_
#include <ostream>

#include <twilio/ml/traits.h>
#include <twilio/detail/is_tuple.h>
#include <twilio/detail/print_tuple.h>
#include <twilio/detail/compat.h>

namespace twilio {
namespace ml {

using namespace ::twilio::compat;

template <typename Load, typename Tuple>
class Node
{
	static_assert (detail::is_tuple<Tuple>::value, "Tuple must be std::tuple");

	Load load_;
	Tuple value_;

public:
  constexpr Node (Load const& load, Tuple const&t) 
    : load_ (set_not_empty (load))
    , value_ (t) {}

  constexpr Load const& load () const noexcept { return load_; }
  constexpr Tuple const& value () const noexcept { return value_; }

  template <class CharT, class TraitsT>
  friend constexpr std::basic_ostream<CharT,TraitsT>&
  print (std::basic_ostream<CharT,TraitsT>& os, Node const& node)
  {
  	return node.load_.print_end (
  	  node.load_.print_begin (os) << '\n' << node.value_ << '\n'
  	);
  }

  template <class CharT, class TraitsT>
  friend constexpr std::basic_ostream<CharT,TraitsT>&
  operator<< (std::basic_ostream<CharT,TraitsT>& os, Node const& node)
  {
  	return print (os, node);
  }
};

template <typename Load, typename Tuple, typename... T>
constexpr auto make_node (Load const& load, Tuple&& tuple, T&&... t)
  -> Node<Load, decltype (std::tuple_cat (std::forward<Tuple> (tuple), 
      std::make_tuple (std::forward<T> (t)...)))>
{
	static_assert (detail::is_tuple<decay_t<Tuple>>::value, 
	    "Tuple must be std::tuple");

	return Node<Load, decltype (std::tuple_cat (std::forward<Tuple> (tuple),
      std::make_tuple (std::forward<T> (t)...)))> (load, 
        std::tuple_cat (std::forward<Tuple> (tuple), 
          std::make_tuple (std::forward<T> (t)...)));
}

template <class Outer, class Tuple, class Verb
  , class = enable_if_t<is_verb<Verb>::value>
>
constexpr auto
operator<< (Node<Outer, Tuple> const& node, Verb const& verb)
  -> decltype (make_node (node.load (), node.value (), verb))
{
	static_assert (detail::is_tuple<Tuple>::value, "Tuple must be std::tuple");

	static_assert (verbs::is_nestable<Outer, Verb>::value, 
	    "Cannot nest under this outer verb");

  return make_node (node.load (), node.value (), verb);
}

template <class N, class L
  , class = enable_if_t<is_verb<N>::value>
  , class = enable_if_t<is_verb<L>::value>
  , class = enable_if_t<verbs::is_nestable<N,L>::value>
>
constexpr Node<N, std::tuple<L>>
operator<< (N const& node, L const& leaf)
{
  return Node<N, std::tuple<L>> (node, std::tuple<L> (leaf));
}

template <class Outer, class Inner
  , class = enable_if_t<is_verb<Outer>::value>
  , class = enable_if_t<is_verb<Inner>::value>
  , class = enable_if_t<! verbs::is_nestable<Outer, Inner>::value>
>
constexpr Outer const& 
operator<< (Outer const& o, Inner const&)
{
	static_assert (verbs::is_nestable<Outer, Inner>::value, 
	    "Cannot nest under this verb");
	return o;
}

template <class Root, class Load, class Tuple
  , class = enable_if_t<is_verb<Root>::value>
>
constexpr auto
operator<< (Root const& root, Node<Load, Tuple> const& node)
  -> decltype (make_node (root, std::make_tuple (node)))
{
  return make_node (root, std::make_tuple (node));
}

template <class Load1, class Tuple1, class Load2, class Tuple2>
constexpr auto
operator<< (Node<Load1,Tuple1> const& root, Node<Load2, Tuple2> const& tail)
  -> decltype (make_node (root.load (), root.value (), tail))
{
  return make_node (root.load (), root.value (), tail);
}

} // namespace ml
} // namespace twilio
#endif // _TWILIO_ML_H_
