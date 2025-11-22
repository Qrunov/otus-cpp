//
// async_tcp_echo_server.cpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2019 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>
#include <boost/asio.hpp>
#include <sstream>
#include <queue>

#include "command.h"
#include "controller.h"

using boost::asio::ip::tcp;

using namespace std;

class session : public std::enable_shared_from_this<session>
{
public:
    session(tcp::socket socket, std::shared_ptr<IController> ctrl)
        : socket_(std::move(socket)), cmd(ctrl)
    {  }

    void start()
    {
        do_read();
    }

private:
    void do_read()
    {
        auto self(shared_from_this());

        boost::asio::async_read_until(socket_, input_buffer_, '\n',
            [this, self](boost::system::error_code ec, std::size_t length)
            {
            if (!ec && length > 0)
            {
                std::istream is(&input_buffer_);
                std::string command;
                std::getline(is, command);

                if (!command.empty() && command.back() == '\r')
                {
                    command.pop_back();
                }
                cmd.execute(command);   
                do_write();
                
            } else {
            cout << "read error: " << ec.message() << endl;
                }
    });
}

void do_write()
{
    auto self(shared_from_this());
    auto response = std::make_shared<std::string>(cmd.getResult());

    boost::asio::async_write(socket_, boost::asio::buffer(*response),
                             [this, self, response](boost::system::error_code ec, std::size_t /*length*/)
                             {
                                if (!ec)
                                 {
                                    do_read();
                                 }
                                 else
                                 {
                                     cout << "write error: " << ec.message() << endl;
                                 }
                             });
}

tcp::socket socket_;
boost::asio::streambuf input_buffer_;
Command cmd;

std::mutex queue_mutex_;


};

class server
{
public:
    server(boost::asio::io_context &io_context, short port)
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
                    std::make_shared<session>(std::move(socket), Controller::getInstance())->start();
                }

                do_accept();
            });
    }

    tcp::acceptor acceptor_;
};

int main(int argc, char *argv[])
{
    try
    {
        if (argc != 2)
        {
            std::cerr << "Usage: async_tcp_echo_server <port>\n";
            return 1;
        }

        boost::asio::io_context io_context;

        server server(io_context, std::atoi(argv[1]));

        io_context.run();
    }
    catch (const std::exception &ex)
    {
        std::cerr << "Exception: " << ex.what() << "\n";
    }

    return 0;
}