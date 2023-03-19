#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#include "mysql_connection.h"

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

using namespace std;

#define PORT 8080

void *connection_handler(void *);
void create_table();

int main() {

    create_table();

    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    // Create a socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Set socket options
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind the socket to the specified address and port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    cout << "Listening on port " << PORT << endl;

    while (true) {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        cout << "New connection established "<<new_socket << endl;

        pthread_t thread_id;
        if (pthread_create(&thread_id, NULL, connection_handler, (void*)&new_socket) < 0) {
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}

void *connection_handler(void *socket_desc) {
    int sock = *(int*)socket_desc;
    char buffer[1024] = {0};
    int valread;

    while ((valread = read(sock, buffer, 1024)) > 0) {
        cout << "Received " <<sock <<" :" << buffer << endl;
        //send(sock, buffer, strlen(buffer), 0);
        send(sock,"messege send",strlen("messege send"),0);
        memset(buffer, 0, sizeof(buffer));
    }

    if (valread == 0) {
        cout << "Client disconnected "<< sock<< endl;
    }
    else {
        perror("read");
    }

    close(sock);

    return 0;
}

void create_table()
{
    try
    {
        sql::Driver *driver;
        sql::Connection *con;
        sql::Statement *stmt;
        //sql::ResultSet *res;

        /* Create a connection */
        driver = get_driver_instance();
        con = driver->connect("tcp://localhost:3306", "debian-sys-maint", "PUAFKnCbBczs8rLS");
        // con = driver->connect("tcp://127.0.0.1:3306", "root", "root");
        /* Connect to the MySQL test database */
        con->setSchema("my");

        stmt = con->createStatement();
        //res = stmt->executeQuery("SELECT 'Hello World!' AS _message"); // replace with your statement
        stmt->execute("CREATE TABLE IF NOT EXISTS `my`.`users` ( `id` INT NOT NULL AUTO_INCREMENT , `name` VARCHAR(255) NOT NULL , `email` VARCHAR(255) NOT NULL , `password` VARCHAR(255) NOT NULL , `created_at` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP , `updated_at` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP , PRIMARY KEY (`id`)) ENGINE = InnoDB;");
        
        stmt->execute("INSERT INTO `users` (`name`, `email`, `password`) VALUES ('test', 'test@test', 'test');");
        stmt->execute("INSERT INTO `users` (`name`, `email`, `password`) VALUES ('test', 'test@test', 'test');");


        // while (res->next())
        // {
        //     cout << "\t... MySQL replies: ";
        //     /* Access column data by alias or column name */
        //     cout << res->getString("_message") << endl;
        //     cout << "\t... MySQL says it again: ";
        //     /* Access column fata by numeric offset, 1 is the first column */
        //     cout << res->getString(1) << endl;
        // }
        //delete res;
        delete stmt;
        delete con;
    }
    catch (sql::SQLException &e)
    {
        cout << "# ERR: SQLException in " << __FILE__;
        // cout << "(" << __FUNCTION__ << ") on line "
        //      << __LINE__ << endl;
        cout << "# ERR: " << e.what();
        cout << " (MySQL error code: " << e.getErrorCode();
        cout << ", SQLState: " << e.getSQLState() << " )" << endl;
    }

    cout << endl;
}