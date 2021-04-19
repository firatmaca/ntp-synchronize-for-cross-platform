#ifndef SOCKET_H
#define SOCKET_H
#include "QString"
#include "QDebug"

class socket1
{
public:
    socket1();
     bool  connettoserver(QString address, tm & timeptr );
};

#endif // SOCKET_H
