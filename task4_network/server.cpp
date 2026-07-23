// ============================================================
// TASK 4: Network Programming and IPC -- SERVER
// ============================================================
// A real TCP server (plain POSIX sockets). Handles multiple
// clients at once by giving each one its own thread.
//
// Protocol (one command per line):
//   LOGIN <username> <password>   -> "OK LOGIN" or "FAIL LOGIN"
//   MSG <text>                    -> "ECHO <text>"   (must be logged in)
//   LIST                          -> "CLIENTS <n>"   (must be logged in)
//   QUIT                          -> closes the connection
//
// Compile with:  g++ -std=c++20 server.cpp -pthread -o server
// Run with:      ./server        (leave this running)
// ============================================================

#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>
#include <sstream>
#include <cstring>

#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

std::atomic<int> connectedClients{0};
std::mutex logMutex;

// Hardcoded demo accounts, just for this task -- basic authentication check.
bool checkLogin(const std::string& username, const std::string& password)
{
    if (username == "alice" && password == "123") return true;
    if (username == "bob" && password == "456") return true;
    return false;
}

void handleClient(int clientSocket)
{
    connectedClients++;
    {
        std::lock_guard<std::mutex> lock(logMutex);
        std::cout << "[Server] client connected. total clients = " << connectedClients << "\n";
    }

    bool loggedIn = false;
    char buffer[1024];

    while (true)
    {
        std::memset(buffer, 0, sizeof(buffer));
        ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
        if (bytesRead <= 0) break; // client disconnected

        std::string line(buffer, bytesRead);
        while (!line.empty() && (line.back() == '\n' || line.back() == '\r')) line.pop_back();

        // basic input validation
        if (line.empty() || line.size() > 500)
        {
            std::string reply = "FAIL invalid command\n";
            send(clientSocket, reply.c_str(), reply.size(), 0);
            continue;
        }

        std::istringstream iss(line);
        std::string command;
        iss >> command;
        std::string reply;

        if (command == "LOGIN")
        {
            std::string username, password;
            iss >> username >> password;
            loggedIn = checkLogin(username, password);
            reply = loggedIn ? "OK LOGIN\n" : "FAIL LOGIN\n";
        }
        else if (!loggedIn)
        {
            reply = "FAIL not logged in\n"; // security check: must authenticate first
        }
        else if (command == "MSG")
        {
            std::string text;
            std::getline(iss, text);
            reply = "ECHO" + text + "\n";
        }
        else if (command == "LIST")
        {
            reply = "CLIENTS " + std::to_string(connectedClients.load()) + "\n";
        }
        else if (command == "QUIT")
        {
            reply = "BYE\n";
            send(clientSocket, reply.c_str(), reply.size(), 0);
            break;
        }
        else
        {
            reply = "FAIL unknown command\n";
        }

        send(clientSocket, reply.c_str(), reply.size(), 0);
    }

    close(clientSocket);
    connectedClients--;
    std::lock_guard<std::mutex> lock(logMutex);
    std::cout << "[Server] client disconnected. total clients = " << connectedClients << "\n";
}

int main()
{
    int port = 5555;
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    int opt = 1;
    setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    bind(serverSocket, (sockaddr*)&address, sizeof(address));
    listen(serverSocket, 10);

    std::cout << "[Server] listening on port " << port << " ...\n";

    while (true)
    {
        sockaddr_in clientAddress{};
        socklen_t clientLen = sizeof(clientAddress);
        int clientSocket = accept(serverSocket, (sockaddr*)&clientAddress, &clientLen);
        if (clientSocket < 0) continue;

        // one thread per client -- this is what lets us handle several at once
        std::thread(handleClient, clientSocket).detach();
    }
}
