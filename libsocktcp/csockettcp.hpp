#ifndef CSOCKETTCP_HPP
#define CSOCKETTCP_HPP

#include "isockettcp.hpp"

class CSocketTCP : public ISocketTCP
{
private: // members
    sockaddr_in                  m_sock_addr;
    socklen_t                    m_sock_len;
    SocketType                   m_sock_type;
    pollfd                      *m_pfd;
    std::atomic_bool             m_run {false};
    std::mutex                   m_s_buf_control;
    std::mutex                   m_r_buf_control;
    std::queue<ByteArray>        m_send_buffer;
    std::queue<ByteArray>        m_recv_buffer;
    std::unique_ptr<std::thread> m_thread;
    bool                         m_is_open;

    int m_fd;
private: // methods
    int initClient();
    int initServer();
    int readData(const int & sock, ByteArray & data);
    int writeData(const int & sock, const ByteArray & data);
    void bufproc(int sock);

    void print(const std::string & text);
    int  error(const std::string & text);
public: // methods
    CSocketTCP(const std::string & ip, const int & port, const SocketType & sock_type);
    ~CSocketTCP() override;

    int init() override;
    void sendData(const ByteArray & data) override;
    void recvData(ByteArray & data) override;
    void closeSock();
};

#endif // CSOCKETTCP_HPP