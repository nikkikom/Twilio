#ifndef _TWILIO_ML_RESPONSE_H_
#define _TWILIO_ML_RESPONSE_H_

#include <twilio/ml/verbs_base.h>
#include <twilio/detail/named_args.h>

namespace twilio {
namespace ml {
namespace verbs {

/// Response verb.
struct Response: base<Response>
{
	/// Constructs "non-empty" response.
	constexpr explicit Response (not_empty<Response> const& x) noexcept 
	  : base<Response> {false} 
	{}

  /// Constructs default response.
	constexpr explicit Response (bool empty = true) noexcept 
	  : base<Response> {empty} 
	{}

  /// The tag name.
	constexpr char const* name () const noexcept { return "Response"; }
};

/// Any verb can be nested under Response verb
template <class T>
struct is_nestable<Response, T> : std::true_type {};

} // namespace verbs

using verbs::Response;
#if 0
template <typename... Ts>
constexpr verbs::Response Response (Ts&&... ts)
{
	return verbs::Response {std::forward<Ts> (ts)...};
}
#endif

}} // namespace twilio::ml
#endif // _TWILIO_ML_RESPONSE_H_
