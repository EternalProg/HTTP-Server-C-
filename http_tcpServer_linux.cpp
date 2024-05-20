#include "http_tcpServer_linux.h"

namespace {
    const int BUFFER_SIZE = 30'720;
    char buffer[BUFFER_SIZE] = {0};

    void log(const string &msg) {
        cout << msg << endl;
    }

    void exitWithError(const string &err) {
        log("ERROR:" + err);
        exit(1);
    }
}

namespace http {
    TcpServer::TcpServer(const string &ip_address, int port)
            : m_ip_address(ip_address), m_port(port), m_socket(), m_new_socket(), m_incomingMessage(),
              m_socketAddress(), m_socketAddress_len(sizeof(m_socketAddress)), m_serverMessage(buildResponse()) {
        m_socketAddress.sin_family = AF_INET;
        m_socketAddress.sin_port = htons(m_port);
        m_socketAddress.sin_addr.s_addr = inet_addr(m_ip_address.c_str());

        if (startServer() != 0) {
            std::ostringstream ss;
            ss << "Failed to start server with PORT: " << ntohs(m_socketAddress.sin_port);
            log(ss.str());
        }
    }

    TcpServer::~TcpServer() {
        closeServer();
    }


    int TcpServer::startServer() {
        m_socket = socket(AF_INET, SOCK_STREAM, 0);
        if (m_socket < 0) {
            exitWithError("Error opening socket");
            return 1;
        }
        int bnd = bind(m_socket, (sockaddr *) &m_socketAddress, m_socketAddress_len);
        if (bnd < 0) {
            exitWithError("Error binding socket");
            return 1;
        }
        return 0;
    }

    void TcpServer::closeServer() const {
        close(m_socket);
        close(m_new_socket);
        exit(0);
    }

    void TcpServer::startListen() {
        if (listen(m_socket, 20) < 0) {
            exitWithError("Error socket listening");
        }
        ostringstream ss;
        ss << "\n*** Listening on ADDRESS: "
           << inet_ntoa(m_socketAddress.sin_addr)
           << " PORT: " << ntohs(m_socketAddress.sin_port)
           << " ***\n\n";
        log(ss.str());

        while (true) {
            acceptInformation();
            readInformation();
            writeInformation();
        }
    }

    void TcpServer::acceptInformation() {
        m_new_socket = accept(m_socket, (sockaddr *) &m_socketAddress, &m_socketAddress_len);
        if (m_new_socket < 0) {
            ostringstream ss;
            ss << "Server failed to accept connection from ADDRESS: "
               << inet_ntoa(m_socketAddress.sin_addr)
               << " PORT: " << ntohs(m_socketAddress.sin_port);
            exitWithError(ss.str());
        }
    }

    void TcpServer::readInformation() {
        m_incomingMessage = read(m_new_socket, buffer, BUFFER_SIZE);
        if (m_incomingMessage < 0) {
            exitWithError("Error reading from socket");
        }
    }

    void TcpServer::writeInformation() {
        long bytesSent = write(m_new_socket, m_serverMessage.c_str(), m_serverMessage.size());
        if (bytesSent == m_serverMessage.size()) {
            log("---- Server sent message to client ----\n\n");
        } else {
            log("Error sending message to client");
        }
    }

    string TcpServer::buildResponse() {
        std::string htmlFile = "<!DOCTYPE html><html lang=\"en\"><body><h1> HOME </h1><p> Hello from your Server :) </p></body></html>";
        std::ostringstream ss;
        ss << "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " << htmlFile.size() << "\n\n"
           << htmlFile;

        return ss.str();
    }


}