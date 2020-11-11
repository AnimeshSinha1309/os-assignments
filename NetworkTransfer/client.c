#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT_SERVER 8000
#define PORT_CLIENT 8001

#define COLOR_RESTORE "\x1B[0m"
#define COLOR_RED "\x1B[31m"
#define COLOR_GREEN "\x1B[32m"
#define COLOR_BLUE "\x1b[34m"

#define READ_SIZE 10000


char* await_input(char* com_value) {
    printf("client> ");
    int com_length = 0;
    while ((com_value[com_length++] = (char)getchar()) != '\n');
    com_value[com_length - 1] = 0;
    if (strncmp(com_value, "get ", 4) == 0) {
        return com_value + 4;
    } else if (strcmp(com_value, "exit") == 0) {
        printf(COLOR_GREEN "LOG: Exitting terminal. Bye bye.\n" COLOR_RESTORE);
        exit(0);
    } else {
        printf(COLOR_RED "ERROR: Invalid command\n" COLOR_RESTORE);
        return NULL;
    }
}


int server_fr;
struct sockaddr_in address_s;

void send_setup() {
    if ((server_fr = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf(COLOR_RED "ERROR: Socket could not be created.\n" COLOR_RESTORE);
    }

    memset(&address_s, '0', sizeof(address_s));
    address_s.sin_family = AF_INET;
    address_s.sin_port = htons(PORT_CLIENT);
    if (inet_pton(AF_INET, "127.0.0.1", &address_s.sin_addr) <= 0) {
        printf(COLOR_RED "ERROR: Invalid address/ Address not supported.\n" COLOR_RESTORE);
    }
    if (connect(server_fr, (struct sockaddr *)&address_s, sizeof(address_s)) < 0) {
        printf(COLOR_RED "ERROR: Connection to socket failed.\n" COLOR_RESTORE);
    }
}

int send_message(char* message) {
    send(server_fr, message, strlen(message), 0);
    printf(COLOR_GREEN "LOG: Sent the message - %s\n" COLOR_RESTORE, message);
    return 0;
}


int server_fd;
struct sockaddr_in address_r;

void recieve_setup() {
    int socket_option = 1;

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        printf(COLOR_RED "ERROR: Socket connection failed.\n" COLOR_RESTORE);
        exit(EXIT_FAILURE);
    }
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                   &socket_option, sizeof(socket_option)))  {
        printf(COLOR_RED "ERROR: `setsockopt` command failed.\n" COLOR_RESTORE);
        exit(EXIT_FAILURE);
    }
    address_r.sin_family = AF_INET;
    address_r.sin_addr.s_addr = INADDR_ANY;
    address_r.sin_port = htons(PORT_SERVER);

    if (bind(server_fd, (struct sockaddr *)&address_r, sizeof(address_r)) < 0) {
        printf(COLOR_RED "ERROR: `bind` command failed.\n" COLOR_RESTORE);
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0) {
        printf(COLOR_RED "ERROR: `listen` command failed.\n" COLOR_RESTORE);
        exit(EXIT_FAILURE);
    }
}

char* recieve_message() {
    int new_socket;
    int address_length = sizeof(address_r);
    char* buffer = (char*)calloc(1, READ_SIZE);
    if ((new_socket = accept(server_fd, (struct sockaddr*) &address_r, (socklen_t*)&address_length)) < 0) {
        printf(COLOR_RED "ERROR: `accept` command failed.\n" COLOR_RESTORE);
        exit(EXIT_FAILURE);
    }
    read(new_socket, buffer, READ_SIZE);
    return buffer;
}

int main(int argc, char const *argv[]) {
    send_setup();
    recieve_setup();
    char* command = (char*)calloc(1, 1000);
    char* buffer = (char*) calloc(1, READ_SIZE);
    while (1) {
        char* filename = await_input(command);
        if (filename != NULL)
            send_message(filename);
        printf(COLOR_GREEN "LOG: Message was sent.\n" COLOR_RESTORE);
        buffer = recieve_message();
        printf(COLOR_BLUE "DATA received: %s\n" COLOR_RESTORE, buffer);
    }
}
