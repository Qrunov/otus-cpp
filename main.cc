#include "async.h"
#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <string>

#include <boost/asio.hpp>
namespace ba = boost::asio;

using namespace std;
size_t 		N = 3;
uint16_t	port = 9999;

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


int main(int argc, const char *argv[])
{
    try
    {
        if (argc == 3)
	{
	    port = strtoul(argv[1], 0, 10);
            N = strtoul(argv[2], 0, 10);
	}

        ba::io_context io_context;
	ba::ip::tcp::endpoint ep(
	    ba::ip::tcp::v4(), 
	    port
	);
	ba::ip::tcp::acceptor acc(io_context, ep);


        while (true) 
	{
	    auto sock = ba::ip::tcp::socket(io_context);
	    acc.accept(sock);
	    std::thread(client_session, std::move(sock)).detach();
	}

/*        auto h = async::connect(N);
        while (!cin.eof())
        {
            string oneLine;
            cin >> oneLine;
            async::receive(h, oneLine.c_str(), oneLine.length());
        }


        async::disconnect(h);*/
    }
    catch (const std::exception &e)
    {
        cerr << "Runtime error has occured:" << e.what() << " program has been terminated" << endl;
    }
    return 0;
}
