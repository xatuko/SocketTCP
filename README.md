## **Класс TCP-сокета на C++ под Unix.**

В описании при объявлении массивов данных используется ByteArray. Его объявление приведено ниже, а также содержится в файле common.hpp.
```C++
using ByteArray = std::vector<uint8_t>;
```

### Пример создания объекта.
```C++
                                                              // ip          порт   тип
std::unique_ptr<ISocketTCP> sock = std::make_unique<CSocketTCP>("127.0.0.1", 42069, SocketType::Server);
```

Для инициализации необходимо вызвать метод init().
В случае ошибки инициализации возвращается -1; 

### Пример инициализации.
```C++
if (sock->init() == -1)
{
  std::cout << "init error" << std::endl;
  return -1;
}
```

### Запись сообщения в сокет.
```C++
ByteArray array = {0x01, 0x02, 0x03, 0x04};
sock->sendData(array);
```

### Чтение сообщения из сокета.
```C++
ByteArray array;
array.clear();
sock->recvData(array);
if (!array.empty())
{
  std::cout << "Message: ";
  for (int i = 0; i < array.size(); i++)
    std::cout << (int)array[i] << ' ' << std::endl;
  std::cout << std::endl;
}
```
