#ifndef HTTP_SERVER_HTTP_TCPSERVER_LINUX_H
#define HTTP_SERVER_HTTP_TCPSERVER_LINUX_H

#include <cstdio>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <cstdlib>
#include <string>
#include <unistd.h>
#include <iostream>
#include <sstream>


using namespace std;

namespace http {
    class TcpServer {
    public:
        TcpServer(const string& ip_address, int port);
        ~TcpServer();
        void startListen();

    private:
        string m_ip_address;
        int m_port;
        int m_socket;
        int m_new_socket;
        long m_incomingMessage;
        struct sockaddr_in m_socketAddress;
        unsigned int m_socketAddress_len;
        string m_serverMessage;

        int startServer();
        void closeServer() const;
        void acceptInformation();
        void readInformation();
        void writeInformation();
        string buildResponse();


    };
}


#endif //HTTP_SERVER_HTTP_TCPSERVER_LINUX_H
