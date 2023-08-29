#include"ConsoleChat.h"
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

#define PORT 7777 

struct sockaddr_in serveraddress, client;
#if defined(_WIN32) || defined(__CYGWIN__)
int length;
#else
socklen_t length;
#endif
int connection, bind_status, connection_status;

#if defined(_WIN32) || defined(__CYGWIN__)
SOCKET sockert_file_descriptor;
#else
int sockert_file_descriptor;
#endif

auto main() -> int
{
#if defined(_WIN32) || defined(__CYGWIN__)
    WSADATA wsaData;
    int iResult;
    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != NO_ERROR) {
        printf("WSAStartup failed: %d\n", iResult);
        return 1;
    }
#endif

    sockert_file_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (sockert_file_descriptor == -1) {
        cout << "Socket creation failed.!" << endl;
        exit(1);
    }
    serveraddress.sin_addr.s_addr = htonl(INADDR_ANY);

    serveraddress.sin_port = htons(PORT);

    serveraddress.sin_family = AF_INET;

    bind_status = bind(sockert_file_descriptor, (const struct sockaddr*)&serveraddress, (int)sizeof(serveraddress));
    if (bind_status == -1) {
        cout << "Socket binding failed.!" << endl;
        exit(1);
    }

    connection_status = listen(sockert_file_descriptor, 5);
    if (connection_status == -1) {
        cout << "Socket is unable to listen for new connections.!" << endl;
        exit(1);
    }
    else {
        cout << "Server is listening for new connection: " << endl;
    }

    ConsoleChat::initdb();

    bool flserverwork = true;
    while (flserverwork)
    {
        length = sizeof(client);
        connection = accept(sockert_file_descriptor, (struct sockaddr*)&client, &length);
        if (connection == -1) {
            cout << "Server is unable to accept the data from client.!" << endl;
            exit(1);
        }
        cout << "Connected new client..." << endl;

        ConsoleChat chat(connection);

        chat.start(); 

        while (chat.isChatStarted()) 
        {
            chat.startMenu(); 
            while (chat.getOnlineUser())
            {
                chat.chatMenu(); 
            }
        }
        cout << "Chat closed" << endl;
    }
    
#if defined(_WIN32) || defined(__CYGWIN__)
    closesocket(sockert_file_descriptor);
    WSACleanup();
#else
    close(sockert_file_descriptor);
#endif

    ConsoleChat::freedbres();

    return 0;

}