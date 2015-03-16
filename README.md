# twilio
Twilio C++11/14 API 
by Nikita Chumakov &lt;nikkikom@gmail.com&gt;

Simplifies the generating TwiML rules, can be used in http servers. The simple
server (based on cpp-netlib) provided in tests directory.

API is header-only library. The classes resides in "twilio::ml" namespace. We
are going to use this namespace in the rest of the code: "using namespace
twilio::ml;".

The generating of ML responses can be acompletished with std::ostream-s ans very
simple:

  using namespace twilio::ml;
  std::cout &lt;&lt; (
    Response () 
      &lt;&lt; Play ("http://domain.com/smth.mp3")
      &lt;&lt; Say ("Hello")
  ) &lt;&lt; "\n";

Output:
  &lt;Response&gt;
  &lt;Play&gt;http://domain.com/smth.mp3&lt;/Play&gt;
  &lt;Say&gt;Hello&lt;/Say&gt;
  &lt;/Response&gt;

Please not that parenthesis before Response() call and after Say(...) must
always be used due to operators precedence rules in C++.

Nested verbs can be generated with the use of parenthesis. Also I implemented
named arguments for verbs calls:

  Response () &lt;&lt;
    ( Gather::setTimeout&lt;6&gt; ("action")
      &lt;&lt; Say::setLoop&lt;10&gt;
            ::setVoice &lt;Say::woman&gt; 
            ("Please leave a message after the tone.")
      &lt;&lt; Pause::setLength&lt;10&gt; ()
      &lt;&lt; Play::setLoop&lt;2&gt; ("http://com/m.mp3")
    )
    &lt;&lt; Say ("hi");

Output:
  &lt;Response&gt;
  &lt;Gather action="action" timeout="6"&gt;
  &lt;Say loop="woman" loop="10"&gt;Please leave a message after the tone.&lt;/Say&gt;
  &lt;Pause length="10"/&gt;
  &lt;Play loop="2"&gt;http://com/m.mp3&lt;/Play&gt;
  &lt;/Gather&gt;
  &lt;Say&gt;hi&lt;/Say&gt;
  &lt;/Response&gt;

The API checks verb's nesting rules and if you try to nest incompatible vers the library will generate the descriptive error:

  std::cout &lt;&lt; (
    Response () 
      &lt;&lt; ( Play ("http://domain.com/smth.mp3") &lt;&lt; Say ("Hello") )
  ) &lt;&lt; "\n";


In file included from runtime.cc:1:
../include/twilio/ml.h:96:2: error: static_assert failed "Nesting rules
      violation: Inner cannot be nested under Outer"
        static_assert (verbs::is_nestable&lt;Outer, Inner&gt;::value,
        ^              ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
runtime.cc:12:44: note: in instantiation of function template specialization
      'twilio::ml::operator&lt;&lt;&lt;twilio::ml::verbs::Play&lt;twilio::nargs::field&lt;unsigned
      int, 1&gt; &gt;, twilio::ml::verbs::Say&lt;twilio::nargs::field&lt;unsigned int, 1&gt;,
      twilio::nargs::field&lt;int, 5&gt;, twilio::nargs::field&lt;int, 3&gt; &gt;, void, void,
      void&gt;' requested here
                &lt;&lt; ( Play ("http://domain.com/smth.mp3") &lt;&lt; Say ("Hello") )
                                                         ^
1 error generated. 


The API can generate data structure in constant context (during compile time):

  constexpr auto response = Response () &lt;&lt;
    ( Gather::setTimeout&lt;6&gt; ("action")
      &lt;&lt; Say::setLoop&lt;10&gt;
            ::setVoice &lt;Say::woman&gt; 
            ("Please leave a message after the tone.")
      &lt;&lt; Pause::setLength&lt;10&gt; ()
      &lt;&lt; Play::setLoop&lt;2&gt; ("http://com/m.mp3")
    )
    &lt;&lt; Say ("hi");

    std::cout &lt;&lt; response &lt;&lt; '\n';

Due to compiler limitations is not supported on clang in c++11 mode. 
Clang c++14, g++ c++11, g++ c++14 work fine. Clang c++11 works fine in runtime
context.

Compilers tested: 
  gcc 4.9.2 (c++11 and c++14 modes)
  gcc 5.0.0 20150308
  clang 3.6.0
  
How to compile the tests:
  c++ -std=c++14 -I ../include -o constexpr constexpr.cc -Wall
  c++ -std=c++11 -I ../include -o runtime runtime.cc -Wall 

http servers test example needs the recent boost and cpp-netlib header only 
library (http://cpp-netlib.org/).

  c++ -std=c++14 -DBOOST_NETWORK_NO_LIB -I ../../cpp-netlib/ -I ../include \
    -o http_server http_server.cc \
    -lboost_system-mt -lboost_thread-mt -lssl -lcrypto

Bugs and limitations:

Due to a lack of time (had only one day to work with library) only a small 
subset of verbs is implemented. Basically it is not more than proof of concept 
that such syntax is possbile and that the generation of data structures in
constant context is possible.

http_server returns only one predefined response, while it should check the
query and return different responses.

The way I pass nouns into the vers should probably reworked. It is difficult to
pass string literals by named arguments (in current implementation), so I pass
them with constructors. However this is not very elegant and should be changed.
