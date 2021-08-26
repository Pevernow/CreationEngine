#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <iostream>
#include <string>

#include "../essential/utility/strutil.h"
#include "client_with_asio.hpp"

using asio::ip::tcp;

enum
{
    max_length = 1024
};

void test_kcp(
    asio::io_service& io_service, const int port_bind_to, const char* ip,
    const int port, size_t test_msg_size)
{
    client_with_asio client(
        io_service, port_bind_to, std::string(ip), port, test_msg_size);
    io_service.run();
}

int main(int argc, char* argv[])
{
    try {
        if (argc != 5) {
            std::cerr
                << "Usage: asio_kcp_client <port_bind_to> <connect_to_host> "
                   "<connect_to_port> <test_msg_lenth>\n";
            std::cerr << "asio_kcp_client 22222 232.23.223.1 12345 500\n";
            return 1;
        }

        asio::io_service io_service;
        test_kcp(
            io_service, std::atoi(argv[1]), argv[2], std::atoi(argv[3]),
            std::atoi(argv[4]));
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
