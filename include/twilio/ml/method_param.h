#ifndef _TWILIO_ML_METHOD_PARAM_H_
#define _TWILIO_ML_METHOD_PARAM_H_

namespace twilio {
namespace ml {
namespace verbs {

struct MethodParam
{
	enum method { GET, POST };

	static constexpr char const* const method2string[2]
#if !noconstexpr
		= {
		  "GET", "POST"
		}
#endif
	;
};

constexpr char const* const MethodParam::method2string[2]
#if noconstexpr
		= {
		  "GET", "POST"
		}
#endif
;

}
}}

#endif // _TWILIO_ML_METHOD_PARAM_H_
