#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std;

#define PORT 8080

int main()
{
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};

    // Create a socket file descriptor
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
    {
        perror("inet_pton failed");
        exit(EXIT_FAILURE);
    }

    // Connect to the server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("connect failed");
        exit(EXIT_FAILURE);
    }

    cout << "Connected to server" << endl;

    while (true)
    {
        cout << "Enter message: ";
        cin.getline(buffer, 1024);

        send(sock, buffer, strlen(buffer), 0);
        cout << "Sent: " << buffer << endl;

        memset(buffer, 0, sizeof(buffer));

        int valread = read(sock, buffer, 1024);
        cout << "Received: " << buffer << endl;

    //     // Получаем ответ от сервера
    //     char buffer[1024];
    //     int bytes_received = recv(sock, buffer, 1024, 0);
    //     if (bytes_received == -1)
    //     {
    //         cout << "Не удалось получить ответ от сервера" << endl;
    //         return 1;
    //     }

    //     // Обрабатываем ответ от сервера
    //     string response(buffer, bytes_received);
    //     cout << "Ответ от сервера: " << response << endl;
    // }

    return 0;
}
