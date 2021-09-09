#include "csockettcp.hpp"

CSocketTCP::CSocketTCP(const std::string & ip,
                       const int & port,
                       const SocketType & sock_type) : ISocketTCP()
{
    m_sock_addr.sin_family      = AF_INET;
    m_sock_addr.sin_port        = htons(port);
    m_sock_addr.sin_addr.s_addr = inet_addr(ip.data());
    m_sock_len                  = sizeof(m_sock_addr);
    m_sock_type                 = sock_type;
}

int CSocketTCP::initClient()
{
    m_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    
    if (m_fd < 0)
        return error("Ошибка создания сокета.");

    if (connect(m_fd, (sockaddr*)&m_sock_addr, m_sock_len) == -1)
        return error("Ошибка соединения.");
    
    print("Соединения установлено.");

    m_is_open = true;
    m_run = true;
    m_thread = std::make_unique<std::thread>(&CSocketTCP::bufproc, this, m_fd);

    return 1;
}

int CSocketTCP::initServer()
{
    m_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (bind(m_fd, (sockaddr*)&m_sock_addr, m_sock_len) == -1)
        return error("Ошибка привязки сокета.");
    
    if (listen(m_fd, 0) == -1)
        return error("Ошибка прослушки сокета.");

    sockaddr_in soc_addr;
    socklen_t sock_len = sizeof(soc_addr);
    int sock = accept(m_fd, (sockaddr*)&soc_addr, &sock_len);
    if (sock == -1)
        return error("Ошибка установки соединения.");

    print("Соединения установлено.");
    print("Входящий IP: " + std::string(inet_ntoa(soc_addr.sin_addr)));
    print("       Порт: " + std::to_string(ntohs(soc_addr.sin_port)));

    m_is_open = true;
    m_run = true;
    m_thread = std::make_unique<std::thread>(&CSocketTCP::bufproc, this, sock);

    return 1;
}

int CSocketTCP::init()
{
    return m_sock_type == SocketType::Client ? initClient() : initServer();
}

int CSocketTCP::readData(const int & sock, ByteArray & data)
{
    int size = read(sock, data.data(), data.size());
    if (size > 0)
        data.resize(size);
    else if (size < 0) return(error("Ошибка чтения."));
    return size;
}

int CSocketTCP::writeData(const int & sock, const ByteArray & data)
{
    int res = write(sock, data.data(), data.size());
    return res > 0 ? res : error("Ошибка записи.");
}

void CSocketTCP::sendData(const ByteArray & data)
{
    std::unique_lock<std::mutex> lock(m_s_buf_control);
    m_send_buffer.push(data);
    lock.unlock();
}

bool CSocketTCP::recvData(ByteArray & data)
{
    data.clear();
    std::unique_lock<std::mutex> lock(m_r_buf_control);
    if (m_recv_buffer.size() > 0)
    {
        data = m_recv_buffer.front();
        m_recv_buffer.pop();
    }
    lock.unlock();

    return !data.empty();
}

void CSocketTCP::bufproc(int sock)
{
    m_pfd = new pollfd[1];

    m_pfd[0].fd      = sock;
    m_pfd[0].events  = POLLIN;
    m_pfd[0].revents = 0;

    while (m_run)
    {   
        // Отправка сообщения.
        if (m_send_buffer.size() > 0)
        {
            std::unique_lock<std::mutex> lock(m_s_buf_control);
            if (writeData(sock, m_send_buffer.front()) > 0)
                m_send_buffer.pop();
            lock.unlock();
        }

        // Ожидание входящего сообщения.
        int res = poll(m_pfd, 1, 1000);
        if (res >= 0)
        {
            if (m_pfd[0].revents & POLLIN)
            {
                // Чтение сообщения.
                std::unique_lock<std::mutex> lock(m_r_buf_control);
                ByteArray barray(BUFFER_SIZE);
                if (readData(sock, barray) > 0)
                    m_recv_buffer.push(barray);
                lock.unlock();
            }
            m_pfd[0].revents = 0;
        }
        
        if (res < 0)
        {
            m_run = false;
            error("Ошибка пола.");
        }
    }
}

void CSocketTCP::print(const std::string & text)
{
    std::cout << "[CSocketTCP] " << text << std::endl;
}

int CSocketTCP::error(const std::string & text)
{
    print(text);
    print("error: " + std::to_string(errno));
    return -1;
}

void CSocketTCP::closeSock()
{
    if (!m_is_open)
        return;
    
    m_is_open = false;
    m_run     = false;
    if (m_thread->joinable())
        m_thread->join();
    
    close(m_fd);
}

CSocketTCP::~CSocketTCP()
{
    closeSock();
}