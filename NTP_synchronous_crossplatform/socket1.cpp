#include "socket1.h"
#if defined(Q_OS_WIN32)

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include "winsock2.h"
#include "winsock.h"
#include "iostream"
//#include "tcpsocket.h"

#pragma comment(lib,"ws2_32.lib");

#define NTP_TIMESTAMP_DELTA 2208988800ull

#define LI(packet)   (uint8_t) ((packet.li_vn_mode & 0xC0) >> 6) // (li   & 11 000 000) >> 6
#define VN(packet)   (uint8_t) ((packet.li_vn_mode & 0x38) >> 3) // (vn   & 00 111 000) >> 3
#define MODE(packet) (uint8_t) ((packet.li_vn_mode & 0x07) >> 0) // (mode & 00 000 111) >> 0

socket1::socket1()
{

}

bool socket1::connettoserver(QString address , tm & timeptr  )
{

    int clientSocket,n;
    struct sockaddr_in server_address;
    int portno = 123;
    QByteArray array = address.toLocal8Bit();
    char* host_name = array.data();

    typedef struct
    {

        uint8_t li_vn_mode;      // Eight bits. li, vn, and mode.
        // li.   Two bits.   Leap indicator.
        // vn.   Three bits. Version number of the protocol.
        // mode. Three bits. Client will pick mode 3 for client.

        uint8_t stratum;         // Eight bits. Stratum level of the local clock.
        uint8_t poll;            // Eight bits. Maximum interval between successive messages.
        uint8_t precision;       // Eight bits. Precision of the local clock.

        uint32_t rootDelay;      // 32 bits. Total round trip delay time.
        uint32_t rootDispersion; // 32 bits. Max error aloud from primary clock source.
        uint32_t refId;          // 32 bits. Reference clock identifier.

        uint32_t refTm_s;        // 32 bits. Reference time-stamp seconds.
        uint32_t refTm_f;        // 32 bits. Reference time-stamp fraction of a second.

        uint32_t origTm_s;       // 32 bits. Originate time-stamp seconds.
        uint32_t origTm_f;       // 32 bits. Originate time-stamp fraction of a second.

        uint32_t rxTm_s;         // 32 bits. Received time-stamp seconds.
        uint32_t rxTm_f;         // 32 bits. Received time-stamp fraction of a second.

        uint32_t txTm_s;         // 32 bits and the most important field the client cares about. Transmit time-stamp seconds.
        uint32_t txTm_f;         // 32 bits. Transmit time-stamp fraction of a second.

    } ntp_packet;

    ntp_packet namesiz = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

    memset( &namesiz, 0, sizeof( ntp_packet ) );
    *( ( char * ) &namesiz + 0 ) = 0x1b;

    struct hostent *server;

    clientSocket=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
    if(clientSocket==-1){
        printf("Soket kurulamadı  \n ");
        return  false;
    }
    else
    {
        printf("soket kuruldu \n");
    }
    server = gethostbyname( host_name);

    memset( &server_address, 0, sizeof( server_address ));
    server_address.sin_family=AF_INET;
    server_address.sin_port=htons(portno);
    server_address.sin_addr.s_addr = inet_addr(host_name);

    if ( connect( clientSocket, ( struct sockaddr * ) &server_address, sizeof( server_address) ) < 0 ){
        printf("bad  \n ");
        return  false;
    }
    else
    {
        printf("good \n");
    }
    fd_set set;
    struct timeval timeout = {5,0};
    FD_ZERO(&set);
    FD_SET(clientSocket ,&set);
    int ff = sendto(clientSocket, ( char*) &namesiz, sizeof(ntp_packet), 0, (sockaddr*) &server_address, sizeof(server_address));
    if(ff == SOCKET_ERROR ){
        return  false;
    }
    int rv = select(clientSocket+1,&set, NULL , NULL ,&timeout);
    if(rv == SOCKET_ERROR || rv == 0 )
        return false;
    n = recv( clientSocket, ( char* ) &namesiz, sizeof( ntp_packet ),0);
    if(n == SOCKET_ERROR)
        return  false;

    namesiz . txTm_s = ntohl ( namesiz . txTm_s );
    // Transmit time - stamp fraction of a second .
    namesiz . txTm_f = ntohl ( namesiz . txTm_f );

    time_t txTm = ( time_t ) ( namesiz. txTm_s - NTP_TIMESTAMP_DELTA );

    timeptr =  *(localtime(&txTm));
    return true;

}
#endif
