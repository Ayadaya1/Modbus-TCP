#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>\
#include <stdlib.h>
#include <winsock2.h>
#include <iostream>
#include "../Packet/Packet.h"
#include <locale.h>
#include <string.h>
#include <cstring>
#pragma comment (lib, "ws2_32.lib") 

int main() {
    char* locale = setlocale(LC_ALL, "Russian");
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKET servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    sockaddr_in sockAddr;
    memset(&sockAddr, 0, sizeof(sockAddr));

    sockAddr.sin_family = PF_INET;
    sockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    sockAddr.sin_port = htons(1234);
    bind(servSock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));

    listen(servSock, 20);

    SOCKADDR clntAddr;
    int nSize;
    SOCKET clntSock;
    bool running = true;
    while (running)
    {
        nSize = sizeof(SOCKADDR);
        clntSock = accept(servSock, (SOCKADDR*)&clntAddr, &nSize);
        char szBuffer[260] = { 0 };
        recv(clntSock, szBuffer, 260, NULL);
        Packet pack;
        std::cout << std::endl;
        pack.Deserialize(szBuffer);
        std::cout << "Получен пакет: " << std::endl << std::endl;
        pack.Print();
        switch (pack.FC)
        {
        case 65:
        {
            const char* str = "Сервер работает!\n";
            strcpy(pack.Data, str);
            send(clntSock, pack.Serialize(), 260, NULL);
            break;
        }
        case 66:
        {
            const char* str = "Введите имя!\n";
            strcpy(pack.Data, str);
            send(clntSock, pack.Serialize(), 260, NULL);
            recv(clntSock, szBuffer, 260, NULL);
            pack.Deserialize(szBuffer);
            std::cout << "Получен пакет:\n\n";
            pack.Print();
            char name[30];
            strcpy(name, pack.Data);
            const char* hello = "Привет, ";
            strcpy(pack.Data, hello);
            strcat(pack.Data, name);
            strcat(pack.Data, "!\n");
            send(clntSock, pack.Serialize(), 260, NULL);
            break;
        }
        case 67:
        {
            const char* str = "Сервер остановлен.\n";
            strcpy(pack.Data, str);
            send(clntSock, pack.Serialize(), 260, NULL);
            running = false;
            break;
        }
        default:
        {
            const char* str = "Неверный код функции!\n";
            send(clntSock, str, strlen(str) + sizeof(char), NULL);
            break;
        }
        }
    }

        closesocket(clntSock);
        closesocket(servSock);
    WSACleanup();


    return 0;
}
