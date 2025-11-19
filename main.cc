#include "async.h"
#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <string>

#include <boost/asio.hpp>
namespace ba = boost::asio;
using boost::asio::ip::tcp;

using namespace std;
size_t 		N = 3;
uint16_t	port = 9999;

class session
  : public std::enable_shared_from_this<session>
{
public:
  session(tcp::socket socket)
    : socket_(std::move(socket))
  {
  }

  void start()
  {
    h = async::connect(N);
    do_read();
  }

private:
  void do_read()
  {
    auto self(shared_from_this());
    socket_.async_read_some(boost::asio::buffer(data_, max_length),
        [this, self](boost::system::error_code ec, std::size_t length)
        {
	  if (length)
	  {
	    strings << std::string{data_, length};

	    std::string s;
	    char c;
	    while(strings.get(c))
	    {
		if ('\n' == c)
		{
    		    async::receive(h, s.c_str(), s.length());
		    s = "";
		}
		else
		    s += c;
	    }
	    strings << s;
	  }
          if (!ec)
          {
            //std::cout << "receive " << length << "=" << std::string{data_, length} << std::endl;
            do_read();
          }
	  else
	  {
	    async::disconnect(h);
	  }

        });
  }

  tcp::socket socket_;
  enum { max_length = 1024 };
  char data_[max_length];
  stringstream strings;
  decltype(async::connect(N)) h;
};

/*
void client_session(ba::ip::tcp::socket sock) {
    while (true) {
	auto h = async::connect(N);
        try {

	    ba::streambuf b;
	    ba::streambuf::mutable_buffers_type bufs = b.prepare(512);
	    boost::system::error_code ec;
	    size_t n = sock.receive(bufs, 0, ec);
	    b.commit(n);

	    std::istream is(&b);
	    std::string s;
	    while (!is.eof())
	    {
		is >> s;
	        
		async::receive(h, s.c_str(), s.length());
		s = "";
	    }
	    if (ec)
	    {
		async::disconnect(h);
		cout << "ïÛÉÂËÁ " << ec.message() << "\n";
		break;
	    }
        }
        catch (const std::exception &e) {
//            std::cerr << "client_session exception! " << e.what() << std::endl;
	    async::disconnect(h);
            break;
        }
    }
}
*/
class server
{
public:
  server(boost::asio::io_context& io_context, short port)
    : acceptor_(io_context, tcp::endpoint(tcp::v4(), port))
  {
    do_accept();
  }

private:
  void do_accept()
  {
    acceptor_.async_accept(
        [this](boost::system::error_code ec, tcp::socket socket)
        {
          if (!ec)
          {
            std::make_shared<session>(std::move(socket))->start();
          }

          do_accept();
        });
  }

  tcp::acceptor acceptor_;
};


int main(int argc, const char *argv[])
{
  try
  {
    if (argc != 3)
    {
      std::cerr << "Usage: bulk_async <port> <bulk_size>\n";
      return 1;
    }
    N = std::atoi(argv[2]);

    boost::asio::io_context io_context;

    server server(io_context, std::atoi(argv[1]));

    io_context.run();
  }
  catch (const std::exception& ex)
  {
    std::cerr << "Exception: " << ex.what() << "\n";
  }

  return 0;

}
