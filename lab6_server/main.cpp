#include <iostream>
#include <thread>
#include <vector>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

// Function to handle a client connection
void ClientHandler(SOCKET clientSocket) {
    char buffer[1024];
    char buffer1[1024];
    int bytesRead;

    while (true) {
        bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);

        if (bytesRead <= 0) {
            // Connection closed or error
            break;
        }

        buffer[bytesRead] = '\0';
        std::cout << "Received: " << buffer << std::endl;

        // Echo the received data back to the client

        const char* message = "Your message has been received!";
        snprintf(buffer1, sizeof buffer1, "Your message %s has been received!", buffer);
        send(clientSocket, buffer1, strlen(buffer1), 0);
    }

    closesocket(clientSocket);
}

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed." << std::endl;
        return 1;
    }

    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed." << std::endl;
        return 1;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(12345); // Choose a port number
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Binding failed." << std::endl;
        return 1;
    }

    if (listen(serverSocket, 5) == SOCKET_ERROR) {
        std::cerr << "Listen failed." << std::endl;
        return 1;
    }

    std::cout << "Server is listening on port 12345..." << std::endl;

    std::vector<std::thread> clientThreads;

    while (true) {
        sockaddr_in clientAddr;
        int clientAddrSize = sizeof(clientAddr);
        SOCKET clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientAddrSize);

        if (clientSocket == INVALID_SOCKET) {
            std::cerr << "Accept failed." << std::endl;
            continue;
        }

        // Create a new thread to handle the client
        clientThreads.emplace_back(ClientHandler, clientSocket);
    }

    // Clean up
    closesocket(serverSocket);
    WSACleanup();

    return 0;
}
