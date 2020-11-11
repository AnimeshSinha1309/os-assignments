// Code inspired from: https://idiotdeveloper.com/file-transfer-using-tcp-socket-in-c/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#define SIZE 1024
#define CONFIG_PORT 8000

#define COLOR_RESTORE "\x1B[0m"
#define COLOR_RED "\x1B[31m"
#define COLOR_GREEN "\x1B[32m"
#define COLOR_BLUE "\x1b[34m"

void write_file(char *filename, int sockfd) {
  char buffer[SIZE];
  char* filename_w = calloc(sizeof(char), 1000);
  strcat(filename_w, filename);
  strcat(filename_w, ".ani_downloaded");
  FILE *fp = fopen(filename_w, "w");
  printf(COLOR_GREEN"LOG: Writing to file - %s\n"COLOR_RESTORE, filename_w);
  for (int i = 0; i < 8; i++) {
    int n = recv(sockfd, buffer, SIZE, 0);
    printf("LOG: Reading Bytes %d.\n", n);
    if (n <= 0) break;
    fprintf(fp, "%s", buffer);
    bzero(buffer, SIZE);
  }
  fclose(fp);
  printf(COLOR_BLUE"LOG: File has been received - %s\n"COLOR_RESTORE, filename_w);
}

void send_filename(char *filename, int sockfd) {
  char data[SIZE] = {0};
  if (send(sockfd, filename, sizeof(filename), 0) == -1) {
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

  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0){
    printf(COLOR_RED"ERROR: Could not open socket.\n"COLOR_RESTORE);
    exit(1);
  }
  printf(COLOR_GREEN"LOG: Server socket created successfully.\n"COLOR_RESTORE);

  server_addr.sin_family = AF_INET;
  server_addr.sin_port = port;
  server_addr.sin_addr.s_addr = inet_addr(ip);

  if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
    printf(COLOR_RED"Error in socket"COLOR_RESTORE);
    exit(1);
  }
  printf(COLOR_GREEN"LOG: Connected to Server.\n"COLOR_RESTORE);

  // Setup loop here
  send_filename("send.txt", sockfd);
  printf(COLOR_GREEN"LOG: File received successfully.\n"COLOR_RESTORE);

  printf(COLOR_GREEN"LOG: Closing the connection.\n"COLOR_RESTORE);
  close(sockfd);

  return 0;
}