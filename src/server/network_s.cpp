#include "network_s.h"

#include "../IDL/network_generated.h"
#include <cstring>
#include <iostream>

using namespace std;
using namespace Network;

void Network_s::event_callback(
    kcp_conv_t conv, kcp_svr::eEventType event_type,
    std::shared_ptr<std::string> msg)
{
    mConv = conv;
    if (event_type == kcp_svr::eRcvMsg) {
        on_recv(msg->c_str(), msg->length());
    }
}

void Network_s::on_recv(const char* buf, size_t size)
{
    flatbuffers::FlatBufferBuilder builder(0);

    auto message = flatbuffers::GetRoot<Message>(buf + 4);
    auto verifier = flatbuffers::Verifier((const uint8_t*)buf, size);
    if (message->Verify(verifier) == false) {
        cerr << "Error message" << endl;
        return;
    }
    auto request_type = message->type();
    switch (request_type) {
        case Type_RegisterNodeList: {
            std::vector<flatbuffers::Offset<NodeDefinition>> nodes_vector;
            for (int i = 0, l = tm->blockmodel.size(); i < l; i++) {
                auto& current_node = tm->blockmodel[i];
                flatbuffers::Offset<flatbuffers::String> strings_tmp[7];
                strings_tmp[0] = builder.CreateString(current_node.name);
                strings_tmp[1] =
                    builder.CreateString(current_node.texture_path[0]);
                strings_tmp[2] =
                    builder.CreateString(current_node.texture_path[1]);
                strings_tmp[3] =
                    builder.CreateString(current_node.texture_path[2]);
                strings_tmp[4] =
                    builder.CreateString(current_node.texture_path[3]);
                strings_tmp[5] =
                    builder.CreateString(current_node.texture_path[4]);
                strings_tmp[6] =
                    builder.CreateString(current_node.texture_path[5]);
                NodeDefinitionBuilder node_builder(builder);
                node_builder.add_name(strings_tmp[0]);
                node_builder.add_top(strings_tmp[1]);
                node_builder.add_bottom(strings_tmp[2]);
                node_builder.add_front(strings_tmp[3]);
                node_builder.add_back(strings_tmp[4]);
                node_builder.add_left(strings_tmp[5]);
                node_builder.add_right(strings_tmp[6]);
                auto node = node_builder.Finish();
                nodes_vector.push_back(node);
            }

            auto response_nodelist = builder.CreateVector(nodes_vector);
            auto response = CreateMessage(
                builder, Type_RegisterNodeList, response_nodelist);
            builder.FinishSizePrefixed(response);
            break;
        }

        default: {
            cerr << "Unsupport request from client." << endl;
            return;
            break;
        }
    }

    // send the response

    uint8_t* data = builder.GetBufferPointer();
    int len = builder.GetSize();
    send((const char*)data, len);
}

void Network_s::send(const char* buf, size_t len)
{
    shared_ptr<string> data = make_shared<string>(buf, buf + len);
    kcp_server_.send_msg(mConv, data);
}

Network_s::Network_s(TypeManager_s* tmPtr, string ip, int port)
    : tm(tmPtr), kcp_server_(io_service_, ip, to_string(port))
{
    kcp_server_.set_callback(std::bind(
        &Network_s::event_callback, this, std::placeholders::_1,
        std::placeholders::_2, std::placeholders::_3));
    thread net_server_thread([this]() { this->io_service_.run(); });
    net_server_thread.detach();
}
