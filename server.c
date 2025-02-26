#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/select.h>


#define PORT 8080
#define BUFFER_SIZE 10 // Small buffer to trigger overflow easily
// #define BUFFER_SIZE 5000 // Larger buffer

void handle_client(int client_socket) {
    char buffer[BUFFER_SIZE];

    printf("Client connected. Waiting for input...\n");

    ssize_t bytes_received = 0;
    bytes_received = recv(client_socket, buffer, 10000, 0);

    printf("Number of bytes: %zu\n", bytes_received);
    printf("Received: %s\n", buffer); // Possible overflow if buffer is overwritten

    // Echo the received message back to the client
    write(client_socket, buffer, bytes_received);

    close(client_socket);
}

int main() {
    int server_fd, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);

    // Create a socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    // Bind socket to the address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Start listening for incoming connections
    if (listen(server_fd, 5) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    while (1) {
        client_socket = accept(server_fd, (struct sockaddr *)&client_addr, &addr_len);
        if (client_socket < 0) {
            perror("Accept failed");
            continue;
        }

        handle_client(client_socket);
    }

    close(server_fd);
    return 0;
}

