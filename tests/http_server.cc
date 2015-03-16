#include "twilio/ml.h"
#include "twilio/ml/verbs_primary.h"
#include <stdio.h>
#include <sstream>

#define LogTrace(x...) fprintf (stderr, x)

#include <boost/network/protocol/http/server.hpp>
#include <string>
#include <iostream>
#include <mutex>
#include <condition_variable>

namespace http = boost::network::http;

struct Server;
typedef boost::network::http::async_server<Server> http_server;

struct connection : std::enable_shared_from_this<connection> 
{
	http_server::request const& req;
	http_server::connection_ptr conn;

	std::string body;
	std::condition_variable cond;
	std::mutex mux;
	size_t read_sofar;

	connection (http_server::request const& req, 
	                 http_server::connection_ptr const& conn)
	  : req (req)
	  , conn (conn)
	  , body ("")
	{
		LogTrace ("connection handler created\n");
  }

  ~connection () {}

  void operator() ()
	{
		LogTrace ("connection handler called\n");
		size_t cl = 0;
		auto const& hs = req.headers;

		for (auto&& header: req.headers)
    {
    	if (boost::to_lower_copy (header.name) == "content-length")
      {
      	cl = boost::lexical_cast<size_t> (header.value);
      	break;
      }
    }

		// LogTrace ("content-length = %ld\n", cl);

    read_sofar = 0;

    while (read_sofar < cl)
    {
    	std::unique_lock<std::mutex> lock {mux};
    	read_chunk (cl - read_sofar);

    	LogTrace ("gonna wait\n");
    	cond.wait (lock);
    	LogTrace ("wokeup\n");
    }
  }

  void read_chunk (size_t left_to_read)
  {
  	LogTrace ("left2read: %ld\n", left_to_read);

    auto self = shared_from_this ();
		conn->read (
		  [this, self, left_to_read] 
		  (http_server::connection::input_range range, 
		      boost::system::error_code error, size_t size,
		      http_server::connection_ptr)
		  {
        if (! error)
        {
          LogTrace ("readSize: %ld\n", size);
          body.append (std::begin (range), size);
          size_t left = left_to_read - size;
          read_sofar += size;
          if (left > 0) read_chunk (left);
          else          cond.notify_one ();
        }
        LogTrace ("%s\n", error.message ().c_str ());
      }
    );
  }
};

struct Server 
{
  void operator() (http_server::request const & request, 
      http_server::connection_ptr conn) const
  {
  	(*std::make_shared<connection> (request, conn)) ();

  	http_server::response_header headers[] = { 
  	  {"Connection","close"} ,{"Content-Type", "text/xml"} };

  	conn->set_status(http_server::connection::ok);
  	conn->set_headers(boost::make_iterator_range(headers, headers+2));

    // by now predefine the single response
	  using namespace twilio::ml;
    constexpr auto const resp = Response () <<
      ( Gather::setTimeout<6> ("action")
           << Say::setLoop <10>
                ::setVoice <Say::woman> ("qwerty")
           << Pause::setLength <10> ()
           << Play::setLoop <2> ("http://com/m.mp3")
      )
      << Say ("hi")
      << Play ("http://url/m.mp3")
    ; 

    std::stringstream os; 
    os << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n" << resp << "\n";
  	conn->write(os.str ());
  }
};

int main(int arg, char * argv[]) {
    Server serv;
    http_server::options options(serv);
    http_server server(
        options.address("0.0.0.0")
               .port("8000"));
    server.run();
}
