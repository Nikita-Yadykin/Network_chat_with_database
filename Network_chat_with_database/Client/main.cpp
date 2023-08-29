#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <string.h>
#if defined(_WIN32) || defined(__CYGWIN__)
#include <winsock2.h>
#define bzero(b,len) (memset((b), '\0', (len)), (void) 0)  
#pragma comment(lib, "ws2_32.lib")
#else
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#endif

using namespace std;

#define MESSAGE_LENGTH 1024 // Максимальный размер буфера для данных
#define PORT 7777 // Будем использовать этот номер порта

int socket_file_descriptor, connection;
struct sockaddr_in serveraddress, client;
char message[MESSAGE_LENGTH];
int main() {
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

    // Создадим сокет
    socket_file_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_file_descriptor == -1) {
        cout << "Creation of Socket failed!" << endl;
        exit(1);
    }

    // Установим адрес сервера
    serveraddress.sin_addr.s_addr = inet_addr("127.0.0.1");
    //serveraddress.sin_addr.s_addr = inet_addr("192.168.1.4");
    // Зададим номер порта
    serveraddress.sin_port = htons(PORT);
    // Используем IPv4
    serveraddress.sin_family = AF_INET;
    // Установим соединение с сервером
    connection = connect(socket_file_descriptor, (struct sockaddr*)&serveraddress, sizeof(serveraddress));
    if (connection == -1) {
        cout << "Connection with the server failed.!" << endl;
        exit(1);
    }
    // Взаимодействие с сервером
    while (1) {
        bzero(message, sizeof(message));
        // Ждем ответа от сервера
#if defined(_WIN32) || defined(__CYGWIN__)
        recv(socket_file_descriptor, message, sizeof(message), 0);
#else
        read(socket_file_descriptor, message, sizeof(message));
#endif
        if (strncmp(message, "CDATA-CIN>>", 11) == 0) {
            bzero(message, sizeof(message));
            cin.getline(&message[0], sizeof(message));
#if defined(_WIN32) || defined(__CYGWIN__)
            size_t bytes = send(socket_file_descriptor, message, sizeof(message), 0);
#else
            ssize_t bytes = write(socket_file_descriptor, message, sizeof(message));
#endif
            // Если передали >= 0  байт, значит пересылка прошла успешно
            if (bytes >= 0) {
                //cout << "Data send to the server successfully.!" << endl;
            }
        }
        else if (strncmp(message, "CDATA:CMD:EXIT", 14) == 0) {
            break;
        }
        else {
            cout << message;// << endl;
        }
    }
    // закрываем сокет, завершаем соединение
#if defined(_WIN32) || defined(__CYGWIN__)
    closesocket(socket_file_descriptor);
    WSACleanup();
#else
    close(socket_file_descriptor);
#endif
    return 0;
}