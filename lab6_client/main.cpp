#include <iostream>
#include <winsock2.h>
#include <Ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed." << std::endl;
        return 1;
    }

    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed." << std::endl;
        return 1;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(12345); // Use the same port as the server
    inet_pton(AF_INET, "192.168.50.34", &serverAddr.sin_addr);

    if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Connection failed." << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Connected to the server." << std::endl;

    char message[1024];
    int bytesRead;

    while (true) {
        std::cout << "Enter a message (or 'exit' to quit): ";
        std::cin.getline(message, sizeof(message));

        if (strcmp(message, "exit") == 0) {
            break;
        }

        send(clientSocket, message, strlen(message), 0);
        bytesRead = recv(clientSocket, message, sizeof(message), 0);

        if (bytesRead <= 0) {
            std::cerr << "Server disconnected." << std::endl;
            break;
        }

        message[bytesRead] = '\0';
        std::cout << "Server says: " << message << std::endl;
    }

    // Clean up
    closesocket(clientSocket);
    WSACleanup();

    return 0;
}
