#include "twilio/ml.h"
#include "twilio/ml/verbs.h"

#include <iostream>

int main ()
{
	using namespace twilio::ml;
	std::cout << (
	  Response ()  <<
#if 0
		( Play ("http://domain.com/smth.mp3") << Say ("Hello") )
#else
	    ( Gather::setTimeout<6> ("action") 
	         << Say::setLoop <10>
	              ::setVoice <Say::woman> ("qwerty") 
	         << Pause::setLength <10> () 
	         << Play::setLoop <2> ("http://com/m.mp3") 
	    )
	    << Say ("hi") 
	    << Play ("http://url/m.mp3")
#endif
	);

	// std::cout << "P:" << detail::is_leaf<verbs::Play<>::setLoop<2>>::value << "\n";
}
