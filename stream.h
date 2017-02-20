#ifndef tcpstream_h
#define tcpstream_h
#define tcpstream_h

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string>

using namespace std;

class TCPStream
{
	private:
	int     m_sd;//socket descriptor
    string  m_peerIP;
    int     m_peerPort;

    bool waitForReadEvent(int timeout);
    TCPStream(int sd, struct sockaddr_in* address);
    TCPStream();
    TCPStream(const TCPStream& stream);

    public:
    friend class TCPAcceptor;
    friend class TCPConnector;

    ~TCPStream();
	string getPeerIP();
	int getPeerPort();


    ssize_t send(const string * buffer, size_t len);
    ssize_t receive(string * buffer, size_t len, int timeout=0);

    enum {
        connectionClosed = 0,
        connectionReset = -1,
        connectionTimedOut = -2
    };

};

#endif
