#ifndef _TWILIO_ML_H_
#define _TWILIO_ML_H_
#include <ostream>

#include <twilio/ml/traits.h>
#include <twilio/detail/is_tuple.h>
#include <twilio/detail/print_tuple.h>

namespace twilio {
namespace ml {

template <typename Load, typename Tuple>
class Node
{
	static_assert (detail::is_tuple<Tuple>::value, "Tuple must be std::tuple");

	Load load_;
	Tuple value_;

public:
  _constexpr Node (Load const& load, Tuple const&t) 
    : load_ (set_not_empty (load))
    , value_ (t) {}

  _constexpr Load const& load () const noexcept { return load_; }
  _constexpr Tuple const& value () const noexcept { return value_; }

  template <class CharT, class TraitsT>
  friend _constexpr std::basic_ostream<CharT,TraitsT>&
  print (std::basic_ostream<CharT,TraitsT>& os, Node const& node)
  {
  	return node.load_.print_end (
  	  node.load_.print_begin (os) << '\n' << node.value_ << '\n'
  	);
  }

  template <class CharT, class TraitsT>
  friend _constexpr std::basic_ostream<CharT,TraitsT>&
  operator<< (std::basic_ostream<CharT,TraitsT>& os, Node const& node)
  {
  	return print (os, node);
  }
};

template <typename Load, typename Tuple, typename... T>
_constexpr auto make_node (Load const& load, Tuple&& tuple, T&&... t)
  -> Node<Load, decltype (std::tuple_cat (std::forward<Tuple> (tuple), 
      std::make_tuple (std::forward<T> (t)...)))>
{
	typedef typename std::decay<Tuple>::type TTuple;

	static_assert (detail::is_tuple<TTuple>::value, "Tuple must be std::tuple");

	return Node<Load, decltype (std::tuple_cat (std::forward<Tuple> (tuple),
      std::make_tuple (std::forward<T> (t)...)))> (load, 
        std::tuple_cat (std::forward<Tuple> (tuple), 
          std::make_tuple (std::forward<T> (t)...)));
}

template <class Load, class Tuple, class Leaf
  , class = typename std::enable_if<is_leaf<Leaf>::value>::type
>
_constexpr auto
operator<< (Node<Load, Tuple> const& node, Leaf const& leaf) 
  -> decltype (make_node (node.load (), node.value (), leaf))
{
	static_assert (detail::is_tuple<Tuple>::value, "Tuple must be std::tuple");
  return make_node (node.load (), node.value (), leaf);
}

template <class N, class L
  , class = typename std::enable_if<is_node<N>::value>::type
  , class = typename std::enable_if<is_leaf<L>::value>::type
>
_constexpr Node<N, std::tuple<L>>
operator<< (N const& node, L const& leaf)
{
  return Node<N, std::tuple<L>> (node, std::tuple<L> (leaf));
}

template <class Leaf, class T
  , class = typename std::enable_if<
      is_leaf<Leaf>::value && ! is_node<Leaf>::value
    >::type
>
_constexpr Leaf const& 
operator<< (Leaf const& l, T const&)
{
	static_assert (is_node<Leaf>::value, "Cannot nest under this verb");
	return l;
}

template <class Root, class Load, class Tuple
  , class = typename std::enable_if<is_node<Root>::value>::type
>
_constexpr auto
operator<< (Root const& root, Node<Load, Tuple> const& node)
  -> decltype (make_node (root, std::make_tuple (node)))
{
  return make_node (root, std::make_tuple (node));
}

template <class Load1, class Tuple1, class Load2, class Tuple2>
_constexpr auto
operator<< (Node<Load1,Tuple1> const& root, Node<Load2, Tuple2> const& tail)
  -> decltype (make_node (root.load (), root.value (), tail))
{
  return make_node (root.load (), root.value (), tail);
}

} // namespace ml
} // namespace twilio
#endif // _TWILIO_ML_H_
