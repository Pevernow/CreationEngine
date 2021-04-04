#include "network_s.h"

#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include <cstring>
#include <iostream>
#include <thread>

using namespace std;
using namespace rapidjson;

TypeManager_s* tm_s;

void read_cb_s(struct bufferevent* bev, void* arg)
{
    char buf[1024] = {0};

    bufferevent_read(bev, buf, sizeof(buf));

    Document document, output;
    output.SetObject();
    document.Parse(buf);
    if (document.HasParseError()) {
        cout << "服务器: 客户端信息已损坏" << endl;
        return;
    }
    Value type;
    if (document.HasMember("type")) {
        if ((string)document["type"].GetString() == "register") {
            type.SetString("registerBlock");
            output.AddMember("type", type, output.GetAllocator());
            Value list;
            list.SetArray();
            for (int i = 0, l = tm_s->blockmodel.size(); i < l; i++) {
                Value name;
                name = StringRef(tm_s->blockmodel[i].name.c_str());
                Value texturePath;
                texturePath =
                    StringRef(tm_s->blockmodel[i].texture_path.c_str());
                Value pair;
                pair.SetObject();
                pair.AddMember("name", name, output.GetAllocator());
                pair.AddMember(
                    "texturePath", texturePath, output.GetAllocator());
                list.PushBack(pair, output.GetAllocator());
            }
            output.AddMember("registerList", list, output.GetAllocator());
            StringBuffer sb;
            Writer<rapidjson::StringBuffer> write(sb);
            output.Accept(write);

            bufferevent_write(bev, sb.GetString(), sb.GetSize());
        }
    } else {
        // bufferevent_write(bev, p, strlen(p) + 1);
    }
}

void write_cb_s(struct bufferevent* bev, void* arg)
{
    ; // send to client properly
}

void event_cb_s(struct bufferevent* bev, short events, void* arg)
{
    if (events & BEV_EVENT_EOF) {
        cout << "服务器: 连接已关闭" << endl;
    } else if (events & BEV_EVENT_ERROR) {
        cout << "服务器: 未知网络错误" << endl;
    }
    if (events & BEV_EVENT_CONNECTED) {
        cout << "服务器: 成功连接至客户端" << endl;
    } else {
        bufferevent_free(bev);
        cout << "服务器: 连接已中断" << endl;
    }
}

void cb_listener_s(
    struct evconnlistener* listener, evutil_socket_t fd, struct sockaddr* addr,
    int len, void* ptr)
{
    struct sockaddr_in* client = (sockaddr_in*)addr;
    // cout << "connect new client: " << inet_ntoa(client->sin_addr)
    //     << "::" << ntohs(client->sin_port) << endl;

    struct event_base* base = (struct event_base*)ptr;

    //添加新事件
    struct bufferevent* bev;
    bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);

    //给bufferevent缓冲区设置回调
    bufferevent_setcb(bev, read_cb_s, write_cb_s, event_cb_s, NULL);

    //启动 bufferevent的 读缓冲区。默认是disable 的
    bufferevent_enable(bev, EV_READ);
}

void Network_s::init(TypeManager_s* tmPtr, int port)
{
    tm_s = tmPtr;
    // init server
    memset(&serv, 0, sizeof(serv));
    serv.sin_family = AF_INET;
    serv.sin_port = htons(port);
    serv.sin_addr.s_addr = htonl(INADDR_ANY);

    //创建 event_base
    base = event_base_new();

    //创建套接字
    //绑定
    //接收连接请求
    listener = evconnlistener_new_bind(
        base, cb_listener_s, base, LEV_OPT_CLOSE_ON_FREE | LEV_OPT_REUSEABLE,
        36, (struct sockaddr*)&serv, sizeof(serv));
}

void Network_s::eventloop()
{
    //启动循环监听
    thread netThread([=]() {
        event_base_dispatch(base);
        evconnlistener_free(listener);
        event_base_free(base);
    });
    netThread.detach();
}
