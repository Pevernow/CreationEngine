#ifndef _NETWORK_S_H_
#define _NETWORK_S_H_
#include "asio.hpp"
#include "server.hpp"
#include "typemanager_s.h"

class Network_s
{
public:
    Network_s(TypeManager_s* tmPtr, string ip = "127.0.0.1", int port = 6180);
    void send(const char* buf, size_t len);
    void on_recv(const char* buf, size_t size);
    void shutdown();
private:
    void event_callback(
        kcp_conv_t conv, kcp_svr::eEventType event_type,
        std::shared_ptr<std::string> msg);
    kcp_conv_t mConv;
    asio::io_service io_service_;
    kcp_svr::server kcp_server_;
    TypeManager_s* tm;
};
#endif