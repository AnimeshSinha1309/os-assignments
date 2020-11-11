// Code inspired from: https://idiotdeveloper.com/file-transfer-using-tcp-socket-in-c/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#define SIZE 1024
#define CONFIG_PORT 8081

#define COLOR_RESTORE "\x1B[0m"
#define COLOR_RED "\x1B[31m"
#define COLOR_GREEN "\x1B[32m"
#define COLOR_BLUE "\x1b[34m"

char* await_input(char* com_value) {
  printf("client> ");
  int com_length = 0;
  while ((com_value[com_length++] = (char)getchar()) != '\n');
  com_value[com_length - 1] = 0;
  if (strncmp(com_value, "get ", 4) == 0) {
    return com_value + 4;
  } else if (strcmp(com_value, "exit") == 0) {
    printf(COLOR_GREEN "LOG: Exiting terminal. Bye bye.\n" COLOR_RESTORE);
    exit(0);
  } else {
    printf(COLOR_RED "ERROR: Invalid command\n" COLOR_RESTORE);
    return NULL;
  }
}

void write_file(char *filename, int sockfd) {
  char buffer[SIZE];
  char* filename_w = (char*) calloc(sizeof(char), 1000);
  strcat(filename_w, filename);
  strcat(filename_w, ".ani_downloaded");

  // Get the size of the file
  if (recv(sockfd, buffer, SIZE, 0) == 0) {
    printf(COLOR_RED"ERROR: Could not receive file-size.\n"COLOR_RESTORE);
  }
  long size = strtol(buffer, NULL, 10);
  printf(COLOR_BLUE"Size of File is %ld.\n"COLOR_RESTORE, size);

  if (size < 0) {
    printf(COLOR_RED"The requested file does not exist on the server.\n"COLOR_RESTORE);
    free(filename_w);
    return;
  }

  FILE *fp = fopen(filename_w, "w");
  printf(COLOR_GREEN"LOG: Writing to file - %s\n"COLOR_RESTORE, filename_w);
  for (int i = 0; i <= size / SIZE; i++) {
    int n = recv(sockfd, buffer, SIZE, 0);
    printf("Download Progress %.2f%%.\r", i * SIZE * 100.0 / (double)size);
    if (n <= 0) continue;
    fprintf(fp, "%s", buffer);
    bzero(buffer, SIZE);
  }
  printf("Download Progress 100%%.\n");
  fclose(fp);
  printf(COLOR_BLUE"LOG: File has been received - %s\n"COLOR_RESTORE, filename_w);
  free(filename_w);
}

void send_filename(char *filename, int sockfd) {
  char* data = (char *) calloc(SIZE, sizeof(char));
  strcpy(data, filename);
  if (send(sockfd, filename, SIZE, 0) == -1) {
    perror(COLOR_RED"ERROR: Could not send filename.\n"COLOR_RESTORE);
    exit(1);
  }
  bzero(data, SIZE);
  printf(COLOR_GREEN"LOG: Filename sent successfully\n"COLOR_RESTORE);
  write_file(filename, sockfd);
}

int main() {
  char *ip = "127.0.0.1";
  int port = CONFIG_PORT;

  struct sockaddr_in server_addr;

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
  while (1) {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
      printf(COLOR_RED"ERROR: Could not open socket.\n"COLOR_RESTORE);
      exit(1);
    }
    printf(COLOR_GREEN"LOG: Server socket created successfully.\n"COLOR_RESTORE);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = port;
    server_addr.sin_addr.s_addr = inet_addr(ip);

    char* buffer = (char*) calloc(1, 1000);
    char* filename = await_input(buffer);
    printf("LOG: File being requested - %s\n", filename);
    if (connect(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) == -1) {
      printf(COLOR_RED"Error in socket"COLOR_RESTORE);
      exit(1);
    }
    printf(COLOR_GREEN"LOG: Connected to Server.\n"COLOR_RESTORE);

    send_filename(filename, sockfd);
    printf(COLOR_GREEN"LOG: File received successfully.\n"COLOR_RESTORE);
    printf(COLOR_GREEN"LOG: Closing the connection.\n"COLOR_RESTORE);
    close(sockfd);
  }
#pragma clang diagnostic pop
}