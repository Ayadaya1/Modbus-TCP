#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>
#include <string.h>
#include <iostream>
#include "../Packet/Packet.h"
#include <locale.h>

int main() 
{
    SetConsoleCP(1251);
    char* locale = setlocale(LC_ALL, "Russian");
    // Инициализируем DLL
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    // Создаем сокет
    // Инициируем запрос к серверу
    
    int nSize;
    // Получение данных, возвращаемых сервером
    char menu;
    bool running = true;
    char data[252] = {0};
    while (running)
    {
        char szBuffer[260] = { 0 };
        SOCKET sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
        sockaddr_in sockAddr;
        memset(&sockAddr, 0, sizeof(sockAddr)); // Каждый байт заполняется 0
        sockAddr.sin_family = PF_INET;
        sockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
        sockAddr.sin_port = htons(1234);
        connect(sock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));
        system("cls");
        std::cout << "1. Ping" << std::endl << "2. Hello, Name!" << std::endl << "3. Пользовательская функция" << std::endl<<"4. Выход"<<std::endl << "Выберите вариант из меню: ";
        std::cin >> menu;
        switch (menu)
        {
        case '1':
        {
            char fc = 65;
            Packet pack(fc, data);
            send(sock, pack.Serialize(), 260, NULL);
            system("cls");
            std::cout << "Отправлен пакет: " << std::endl << std::endl;
            pack.Print();
            system("pause");
            system("cls");
            recv(sock, szBuffer, 260, NULL);
            pack.Deserialize(szBuffer);
            char str[252]={0};
            strcpy(str, pack.Data);
            std::cout << "Сообщение от сервера: ";
            if (strlen(str))
                std::cout << str;
            else
                std::cout << "Сервер не отвечает!\n";
            system("pause");
            break;
        }
        case '2':
        {
            char fc = 66;
            Packet pack(fc, data);
            send(sock, pack.Serialize(), 260, NULL);
            system("cls");
            std::cout << "Отправлен пакет: " << std::endl << std::endl;
            pack.Print();
            system("pause");
            system("cls");
            recv(sock, szBuffer, 260, NULL);
            pack.Deserialize(szBuffer);
            std::cout << pack.Data<<std::endl;
            char name[30];
            std::cin >> name;
            strcpy(pack.Data, name);
            send(sock, pack.Serialize(), 260, NULL);
            recv(sock, szBuffer, 260, NULL);
            pack.Deserialize(szBuffer);
            std::cout << pack.Data<<std::endl;
            system("pause");
            break;
        }
        case '3':
        {
            char fc = 67;
            Packet pack(fc, data);
            send(sock, pack.Serialize(), 260, NULL);
            system("cls");
            std::cout << "Отправлен пакет: " << std::endl << std::endl;
            pack.Print();
            system("pause");
            system("cls");
            recv(sock, szBuffer, 260, NULL);
            pack.Deserialize(szBuffer);
            std::cout << pack.Data;
            break;
        }
        case'4':
            running = false;
            closesocket(sock);
            break;
        }
    }
    // Закрываем сокет
    // Прекращаем использование DLL
    WSACleanup();
    system("pause");
    return 0;
}
