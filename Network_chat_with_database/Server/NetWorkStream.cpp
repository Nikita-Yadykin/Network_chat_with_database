#define _CRT_SECURE_NO_WARNINGS

#include "NetWorkStream.h"

NetWorkStream::NetWorkStream(int connection) {
    this->connection = connection;
}

NetWorkStream& operator<< (NetWorkStream& os, const string& s) {
    bzero(os.message, MESSAGE_LENGTH);
    //check size
    strcpy(os.message, s.c_str());
#if defined(_WIN32) || defined(__CYGWIN__)
    size_t bytes = send(os.connection, os.message, sizeof(os.message), 0);
#else
    ssize_t bytes = write(os.connection, os.message, sizeof(os.message));
#endif
    // Если передали >= 0  байт, значит пересылка прошла успешно
    if (bytes >= 0) {
        //cout << "Data successfully sent to the client.!" << endl;
    }
    return os;
}

NetWorkStream& operator>> (NetWorkStream& is, string& s) {
    bzero(is.message, MESSAGE_LENGTH);
    strcpy(is.message, "CDATA-CIN>>");
#if defined(_WIN32) || defined(__CYGWIN__)
    size_t bytes = send(is.connection, is.message, sizeof(is.message), 0);
#else
    ssize_t bytes = write(is.connection, is.message, sizeof(is.message));
#endif
    bzero(is.message, MESSAGE_LENGTH);
#if defined(_WIN32) || defined(__CYGWIN__)
    recv(is.connection, is.message, sizeof(is.message), 0);
#else
    read(is.connection, is.message, sizeof(is.message));
#endif
    s = string(is.message);
    return is;
}