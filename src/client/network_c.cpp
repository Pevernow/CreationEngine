#include "network_c.h"
#include "../IDL/network_generated.h"
#include <cstring>
#include <iostream>
#include <thread>

#include "spdlog.h"

using namespace std;
using namespace Network;

extern bool gQuit;

void Network_c::on_recv(const char* buf, size_t size)
{
    int message_size = flatbuffers::GetPrefixedSize((const uint8_t*)buf);

    auto message = flatbuffers::GetRoot<Message>(buf + 4);

    auto request_type = message->type();
    switch (request_type) {
        case Type_RegisterNodeList: {
            auto nodes = message->registerNodes();
            auto nodes_len = nodes->size();
            for (int i = 0; i < nodes_len; i++) {
                auto node = nodes->Get(i);
                if (node->name() != NULL) {
                    const char* textures[6] = {nullptr, nullptr, nullptr,
                                               nullptr, nullptr, nullptr};
                    textures[0] = node->top()->c_str();
                    textures[1] = node->bottom()->c_str();
                    textures[2] = node->front()->c_str();
                    textures[3] = node->back()->c_str();
                    textures[4] = node->left()->c_str();
                    textures[5] = node->right()->c_str();
                    tm->registerNode(node->name()->c_str(), textures);
                }
            }
            break;
        }
        case Type_KeepAlive:
            break; // ignore
        default: {
            spdlog::error("Unsupport request from server");
            break;
        }
    }
}

// Warning: callback has no currently this!
// Use var instead of it.
void Network_c::event_callback(
    kcp_conv_t conv, asio_kcp::eEventType event_type, const std::string& msg,
    void* var)
{
    switch (event_type) {
        case asio_kcp::eConnect:
            spdlog::info("Connect to server successd");
            break;
        case asio_kcp::eConnectFailed:
            spdlog::error("Connect to server failed");
            gQuit = true;
            break;
        case asio_kcp::eRcvMsg:
            ((Network_c*)var)->on_recv(msg.c_str(), msg.length());
            break;
        case asio_kcp::eDisconnect:
            spdlog::info("Disconnect to server");
            gQuit = true;
            // you can add asio::io_service::work to prevent
            // program quit
            break;
        default:; // do nothing
    }
}

void Network_c::on_tick(const std::error_code& err, Network_c* _this)
{
    _this->kcp_client_.update();
    _this->timer.expires_from_now(chrono::milliseconds(5));
    _this->timer.async_wait(std::bind(on_tick, std::placeholders::_1, _this));
}

Network_c::Network_c(TypeManager_c* tmPtr, string ip, int port)
    : tm(tmPtr), timer(io_service_, chrono::milliseconds(5))
{
    // Config kcp client
    kcp_client_.set_event_callback(event_callback, (void*)this);
    kcp_client_.connect_async(24431, ip, port);

    timer.async_wait(std::bind(on_tick, std::placeholders::_1, this));

    thread net_client_thread([this]() { this->io_service_.run(); });
    net_client_thread.detach();
}

void Network_c::send(const char* buf, size_t len)
{
    // for (int i = 0; i + 1024 < len; i += 1024) {
    // mKcpServer.sendData(buf + i, 1024, mRetainThis.get());
    //}
    string data(buf, buf + len);
    kcp_client_.send_msg(data);
}
void Network_c::startUp()
{
    // require nodes
    flatbuffers::FlatBufferBuilder builder(0);
    auto message = CreateMessage(builder, Type_RegisterNodeList);
    builder.FinishSizePrefixed(message);

    uint8_t* buf = builder.GetBufferPointer();
    int size = builder.GetSize();
    send((const char*)buf, size);
}

void Network_c::keepAlive()
{
    flatbuffers::FlatBufferBuilder builder(0);
    auto message = CreateMessage(builder, Type_KeepAlive);
    builder.FinishSizePrefixed(message);

    uint8_t* buf = builder.GetBufferPointer();
    int size = builder.GetSize();
    send((const char*)buf, size);
}

void Network_c::shutdown()
{
    kcp_client_.stop();
    io_service_.stop();
}