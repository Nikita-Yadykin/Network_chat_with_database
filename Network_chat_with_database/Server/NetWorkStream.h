#include<iostream>
#if defined(_WIN32) || defined(__CYGWIN__)
#include <winsock2.h>
#define bzero(b,len) (memset((b), '\0', (len)), (void) 0)  
#pragma comment(lib, "ws2_32.lib")
#else
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>
#endif
#include<string.h>

using namespace std;
#define MESSAGE_LENGTH 1024 // Максимальный размер буфера для данных

class NetWorkStream {
private:
    int connection;
    char message[MESSAGE_LENGTH];
public:
    NetWorkStream(int connection);
    friend NetWorkStream& operator<< (NetWorkStream& os, const string& s);
    friend NetWorkStream& operator>> (NetWorkStream& is, string& s);
};