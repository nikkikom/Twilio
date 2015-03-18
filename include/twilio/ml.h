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

/// All the structure is build from nodes. 
/// Load part holds the parent verb.
/// Tuples holds children (nested) verbs.
template <typename Load, typename Tuple>
class Node
{
	static_assert (detail::is_tuple<Tuple>::value, 
	  "Tuple type must be a std::tuple");

public:
	Load load_;
	Tuple value_;

public:
  /// Node constructror.
  /**
   * @param load Parent outer verb.
   * @param tuple std::tuple that holds the inner nested verbs.
   */
  template <typename _Load, typename _Tuple
    // to make a single constructor with universal refs
    , class = enable_if_t<std::is_same<decay_t<_Load>, Load>::value>
    , class = enable_if_t<std::is_same<decay_t<_Tuple>, Tuple>::value>
  >
  constexpr Node (_Load&& load, _Tuple&& t) 
    : load_  { set_not_empty (std::forward<_Load> (load)) }
    , value_ { std::forward<_Tuple> (t)                   }
  {}

  /// Load getter.
  // @returns const reference to the parent outer verb.
  constexpr Load  const& load ()  const& noexcept { return load_;  }

  /// Value getter.
  // @returns const reference to the tuple containing inner nested verbs.
  constexpr Tuple const& value () const& noexcept { return value_; }

  /// Prints the content of the node into the ostream.
  /**
   * @param os ostream.
   * @param node the Node object to print.
   * @returns The reference to the ostream.
   */
  template <class CharT, class TraitsT>
  friend constexpr std::basic_ostream<CharT,TraitsT>&
  print (std::basic_ostream<CharT,TraitsT>& os, Node const& node)
  {
  	return node.load_.print_end (
  	  node.load_.print_begin (os) << '\n' << node.value_ << '\n'
  	);
  }

  /// Outputs the content of the node into the ostream.
  /**
   * @param os ostream.
   * @param node the Node object to print.
   * @returns The reference to the ostream.
   * @notes Calls the "print" free function.
   */
  template <class CharT, class TraitsT>
  friend constexpr std::basic_ostream<CharT,TraitsT>&
  operator<< (std::basic_ostream<CharT,TraitsT>& os, Node const& node)
  {
  	return print (os, node);
  }
};

/// Creates the node object.
/*
 * Cretes the node object from the outer verb and the tuples of nested verbs.
 * Usually used to append a list of verbs to existant Node object.
 * @param load Parent outer verb.
 * @param tuple std::tuple that holds the inner nested verbs.
 * @param verbs additional nested verbs to be appended to the tuple value.
 *
 * @returns The Node object value.
 */
template <typename Load, typename Tuple, typename... Verbs>
constexpr auto make_node (Load&& load, Tuple&& tuple, Verbs&&... verbs)
  -> Node<decay_t<Load>, decltype (std::tuple_cat (std::forward<Tuple> (tuple), 
      std::make_tuple (std::forward<Verbs> (verbs)...)))>
{
	static_assert (detail::is_tuple<decay_t<Tuple>>::value, 
	    "Tuple must be a std::tuple type");

	return Node<decay_t<Load>, decltype (
	  std::tuple_cat (std::forward<Tuple> (tuple),
      std::make_tuple (std::forward<Verbs> (verbs)...)))> (
        std::forward<Load> (load), 
          std::tuple_cat (std::forward<Tuple> (tuple), 
            std::make_tuple (std::forward<Verbs> (verbs)...)));
}

/// Appends the verb to the Node.
/**
 * @param node the Node object rvalue reference.
 * @param verb the verb to be appended to the Node.
 * @returns The Node object containing the load and verbs from the original node
 * with appended verb to it.
 * @notes Uses make_node function.
 */
template <class Outer, class Tuple, class Verb
  , class = enable_if_t<is_verb<Verb>::value>
>
constexpr auto
operator<< (Node<Outer, Tuple>&& node, Verb&& verb)
  -> decltype (make_node (std::move (node.load_), std::move (node.value_), 
        std::forward<Verb> (verb)))
{
	static_assert (detail::is_tuple<Tuple>::value, "Tuple must be std::tuple");

	static_assert (verbs::is_nestable<Outer, Verb>::value, 
	    "Nesting rules violation: Verb cannot be nested under Outer");

  return make_node (std::move (node.load_), std::move (node.value_), 
        std::forward<Verb> (verb));
}

/// Composes outer and inner (nested) verbs into the Node object.
/* 
 * @param outer the outer node instance.
 * @param inner the nested node instance.
 * @returns The Node object.
 */
template <class O, class I
  , class = enable_if_t<is_verb<decay_t<O>>::value>
  , class = enable_if_t<is_verb<decay_t<I>>::value>
  , class = enable_if_t<verbs::is_nestable<decay_t<O>,decay_t<I>>::value>
>
constexpr Node<decay_t<O>, std::tuple<decay_t<I>>>
operator<< (O&& outer, I&& inner)
{
  return Node<decay_t<O>, std::tuple<decay_t<I>>> (
    std::forward<O> (outer), std::tuple<I> (std::forward<I> (inner)));
}

/// Cutoff forbidden verbs nesting.
/* 
 * Generates compile time error if Inner verb cannot be nested under the Outer.
 * @param outer the outer verb.
 * @param inner the nested verb.
 * @returns The outer verb const reference.
 */
template <class Outer, class Inner
  , class = enable_if_t<is_verb<Outer>::value>
  , class = enable_if_t<is_verb<Inner>::value>
  , class = enable_if_t<! verbs::is_nestable<Outer, Inner>::value>
>
constexpr Outer const& 
operator<< (Outer const& o, Inner const&)
{
	static_assert (verbs::is_nestable<Outer, Inner>::value, 
	    "Nesting rules violation: Inner cannot be nested under Outer");
	return o;
}

/// Compose the Node object under Outer verb.
/* 
 * @param outer the outer verb.
 * @param node the nested Node object.
 * @returns The composed Node object.
 */
template <class Outer, class Load, class Tuple
  , class = enable_if_t<is_verb<Outer>::value>
>
constexpr auto
operator<< (Outer const& outer, Node<Load, Tuple> const& node)
  -> decltype (make_node (outer, std::make_tuple (node)))
{
  return make_node (outer, std::make_tuple (node));
}

/// Compose the inner Node object under the outer Node object.
/*
 * @param outer the outer Node object.
 * @param inner the inner Node object.
 * @returns The Node object with inner node appended to the outer.
 */
template <class LoadO, class TupleO, class LoadI, class TupleI>
constexpr auto
operator<< (Node<LoadO,TupleO> outer, Node<LoadI, TupleI> inner)
  -> decltype (make_node (std::move (outer.load_), std::move (outer.value_), 
        std::move (inner)))
{
  return make_node (std::move (outer.load_), std::move (outer.value_), 
        std::move (inner));
}

} // namespace ml
} // namespace twilio
#endif // _TWILIO_ML_H_
