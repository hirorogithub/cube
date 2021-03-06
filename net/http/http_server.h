#ifndef __CUBE_HTTP_SERVER_H__
#define __CUBE_HTTP_SERVER_H__

#include "net/tcp_server.h"
#include "http_connection.h"

namespace cube {

namespace http {

class HTTPServer {
    public:
        HTTPServer(EventLoop *event_loop, const InetAddr &server_addr);
        ~HTTPServer();

        void SetRequestCallback(const RequestCallback &cb) { m_request_callback = cb; }
        bool Start();
        void Stop();

        const InetAddr &ServerAddr() const { return m_server.ServerAddr(); }

        bool KeepAlive() const { return m_enable_keepalive; }
        void SetKeepAlive(bool on) { m_enable_keepalive = on; }

        void RemoveConnection(HTTPConnectionPtr conn);

    private:
        void OnNewConnection(TcpConnectionPtr conn);
        void OnRequest(HTTPConnectionPtr conn, const HTTPRequest &request);

    private:
        EventLoop *m_event_loop;

        TcpServer m_server;

        RequestCallback m_request_callback;

        std::map<uint64_t, HTTPConnectionPtr> m_conns;
        bool m_enable_keepalive;
};

}

}

#endif
