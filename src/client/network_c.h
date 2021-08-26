#ifndef _NETWORK_C_H_
#define _NETWORK_C_H_
#include "asio.hpp"
#include "kcp_client.hpp"
#include "typemanager_c.h"
#include <string>
class Network_c
{
public:
    Network_c(TypeManager_c* tmPtr, string ip = "127.0.0.1", int port = 6180);
    void send(const char* buf, size_t len);
    void startUp();

private:
    TypeManager_c* tm;
    asio::io_service io_service_;
    asio::high_resolution_timer timer;
    asio_kcp::kcp_client kcp_client_;
    static void on_tick(const std::error_code& err, Network_c* _this);
    void on_recv(const char* buf, size_t size);
    static void event_callback(
        kcp_conv_t conv, asio_kcp::eEventType event_type,
        const std::string& msg, void* var);
};
#endif