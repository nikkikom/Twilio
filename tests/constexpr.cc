#include <iostream>
#include "twilio/ml.h"
#include "twilio/ml/verbs.h"


int main ()
{
	using namespace twilio::ml;
	constexpr auto const resp = Response () <<
	    ( Gather::setTimeout<6> ("action") 
	         << Say::setLoop <10>
	               ::setVoice <Say::woman> 
	               ("Please leave a message after the tone.") 
	         << Pause::setLength <10> () 
	         << Play::setLoop <2> ("http://com/m.mp3") 
	    )
	    << Say ("hi") 
	    << Play ("http://url/m.mp3")
	;

	std::cout << resp << '\n';

	// std::cout << "P:" << detail::is_leaf<verbs::Play<>::setLoop<2>>::value << "\n";
}
