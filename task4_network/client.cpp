// ============================================================
// TASK 4: Network Programming and IPC -- CLIENT
// ============================================================
// Connects to server.cpp and runs through the whole protocol
// once, printing what it sends and what it gets back.
//
// Compile with:  g++ -std=c++20 client.cpp -o client
// Run with:      ./client       (server.cpp must already be running)
// ============================================================

#include <iostream>
#include <string>
#include <cstring>
#include <thread>
#include <chrono>

#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

std::string sendCommand(int sock, const std::string& command)
{
    std::string toSend = command + "\n";
    send(sock, toSend.c_str(), toSend.size(), 0);

    char buffer[1024];
    std::memset(buffer, 0, sizeof(buffer));
    ssize_t bytesRead = recv(sock, buffer, sizeof(buffer) - 1, 0);
    if (bytesRead <= 0) return "(no reply)";
    return std::string(buffer, bytesRead);
}

int main()
{
    int port = 5555;
    int sock = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    inet_pton(AF_INET, "127.0.0.1", &serverAddress.sin_addr);

    if (connect(sock, (sockaddr*)&serverAddress, sizeof(serverAddress)) < 0)
    {
        std::cerr << "Could not connect -- is server.cpp running?\n";
        return 1;
    }

    std::cout << "[Client] connected to server\n";

    // Pause here on purpose: if you start this program in a few different
    // terminals within a couple of seconds of each other, they'll all be
    // connected AT THE SAME TIME during this pause -- that's what proves
    // the server handles multiple concurrent clients, not just one at a time.
    std::cout << "[Client] waiting a few seconds (start other clients now if you want to test concurrency)...\n";
    std::this_thread::sleep_for(std::chrono::seconds(4));

    std::cout << "[Client] > LOGIN alice 123\n";
    std::cout << "[Client] < " << sendCommand(sock, "LOGIN alice 123");

    std::cout << "[Client] > MSG hello from the client\n";
    std::cout << "[Client] < " << sendCommand(sock, "MSG hello from the client");

    std::cout << "[Client] > LIST\n";
    std::cout << "[Client] < " << sendCommand(sock, "LIST");

    std::cout << "[Client] > QUIT\n";
    std::cout << "[Client] < " << sendCommand(sock, "QUIT");

    close(sock);
    return 0;
}
