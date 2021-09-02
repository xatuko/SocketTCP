#ifndef ISOCKETTCP_HPP
#define ISOCKETTCP_HPP

#include "common.hpp"

class ISocketTCP
{
public:
    ISocketTCP() = default;
    virtual ~ISocketTCP() = default;

    virtual int init() = 0;
    virtual void sendData(const ByteArray & data) = 0;
    virtual void recvData(ByteArray & data) = 0;
    virtual void closeSock() = 0;
};

#endif // ISOCKETTCP_HPP