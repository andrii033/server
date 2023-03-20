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
    char name[1024] = {0};
    char password[1024] = {0};

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

    cout << "Enter name: ";
    cin.getline(name, 1024);

    cout << "Enter password: ";
    cin.getline(password, 1024);

    // Отправка имени и пароля
    int name_length = strlen(name);
    int password_length = strlen(password);
    write(sock, &name_length, sizeof(name_length));
    write(sock, name, name_length);
    write(sock, &password_length, sizeof(password_length));
    write(sock, password, password_length);

    while (true)
    {
        cout << "Enter message: ";
        cin.getline(buffer, 1024);
        send(sock, buffer, strlen(buffer), 0);
        cout << "Sent message " << endl;

        memset(buffer, 0, sizeof(buffer));

        int valread = read(sock, buffer, 1024);
        cout << "Received: " << buffer << endl;
    }

    close(sock);
    return 0;
}
