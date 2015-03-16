#ifndef _TWILIO_DETAIL_COMPAT_H_
#define _TWILIO_DETAIL_COMPAT_H_

#include <type_traits>

namespace twilio {
namespace compat {

// eliminate the need to write "typename.... ::type everywhere in c++11
#if __cplusplus >= 201300L
using std::enable_if_t;
using std::decay_t;
#else
template <bool B, class T=void> 
using enable_if_t = typename std::enable_if<B,T>::type;

template <class T>
using decay_t = typename std::decay<T>::type;
#endif

}}

#endif // _TWILIO_DETAIL_COMPAT_H_
