#include "libsocktcp/csockettcp.hpp"

int main(int, char**) {
    int type;
    std::cout << "1 - server\n2 - client\n> ";
    std::cin >> type;

    std::unique_ptr<libsock::ISocketTCP> m_sock;

    if (type == 1)
        m_sock = std::make_unique<libsock::CSocketTCP>("192.168.11.148", 6534, libsock::SocketType::Server);
    else if (type == 2)
        m_sock = std::make_unique<libsock::CSocketTCP>("192.168.11.148", 6534, libsock::SocketType::Client);
    else return -1;
    if (m_sock->init() == -1)
        return -1;

    std::string input = "";
    while(input != "exit")
    {
        
            std::cout << "$ ";
            std::cin >> input;

            libsock::ByteArray arr(input.size()), arr2;
            for (int i = 0; i < input.size(); i++)
                arr[i] = input[i];
            if (arr.size() > 0)
                m_sock->sendData(arr);

            if (m_sock->recvData(arr2))
            {
                std::cout << "Message: ";
                for (int i =  0; i < arr2.size(); i++)
                    std::cout << (char)arr2[i] << " ";
                std::cout << std::endl;
            }
    }
    return 0;
}
