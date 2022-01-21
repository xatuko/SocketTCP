#ifndef ISOCKETTCP_HPP
#define ISOCKETTCP_HPP

#include "common.hpp"

namespace libsock {

class ISocketTCP
{
public:
    ISocketTCP() = default;
    virtual ~ISocketTCP() = default;

    /**
     * @brief Инициализация сокета.
     * @return 1 - в случае успеха, -1 - в случае ошибки.
     */
    virtual int init() = 0;

    /**
     * @brief Отправить массив байт.
     * @param data Массив байт.
     */
    virtual void sendData(const ByteArray & data) = 0;

    /**
     * @brief Прочитать данные.
     * @param data Массив, куда считывать данные.
     * @return true - если есть данные, false - если нет данных.
     */
    virtual bool recvData(ByteArray & data) = 0;

    /**
     * @brief Закрыть сокет.
     */
    virtual void closeSock() = 0;
};

} // namespace libsock

#endif // ISOCKETTCP_HPP