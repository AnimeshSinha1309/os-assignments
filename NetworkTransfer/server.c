// Code inspired from: https://idiotdeveloper.com/file-transfer-using-tcp-socket-in-c/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#define SIZE 10240
#define CONFIG_PORT 8000

#define COLOR_RESTORE "\x1B[0m"
#define COLOR_RED "\x1B[31m"
#define COLOR_GREEN "\x1B[32m"
#define COLOR_BLUE "\x1b[34m"

void send_file(FILE *fp, int sockfd){
  char data[SIZE] = {0};
  while (fgets(data, SIZE, fp) != NULL){
    if (send(sockfd, data, sizeof(data), 0) == -1){
      perror("[-]Error in sending file.");
      exit(1);
    }
    bzero(data, SIZE);
  }
}

void get_filename(int sockfd){
  // Get the filename
  char *buffer = (char *)calloc(SIZE,sizeof(char));
  char *filename = (char *)calloc(SIZE,sizeof(char));
  recv(sockfd, buffer, SIZE, 0);
  strcpy(filename,buffer);
  bzero(buffer, SIZE);
  printf(COLOR_BLUE"DATA: The filename is : %s\n"COLOR_RESTORE, filename);

  // Actually sending the file
  FILE *fp;
  fp = fopen(filename, "r");
  if (fp == NULL){
    printf(COLOR_RED"Error in reading file."COLOR_RESTORE);
    exit(1);
  }
  printf(COLOR_GREEN"LOG: Sending file %s\n"COLOR_RESTORE, filename);
  send_file(fp, sockfd);
}

int main(){
  char *ip = "127.0.0.1";
  int port = CONFIG_PORT;

  int sockfd, new_sock;
  struct sockaddr_in server_addr, new_addr;
  socklen_t addr_size;

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0){
    printf("ERROR: Could not open a socket.\n");
    exit(1);
  }
  printf(COLOR_GREEN"Server socket created successfully.\n"COLOR_RESTORE);

  server_addr.sin_family = AF_INET;
  server_addr.sin_port = port;
  server_addr.sin_addr.s_addr = inet_addr(ip);

  int e = bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
  if (e < 0){
    perror(COLOR_RED"ERROR: Could not bind to socket."COLOR_RESTORE);
    exit(1);
  }
  printf(COLOR_GREEN"LOG: Binding to socket successful.\n"COLOR_RESTORE);

  while(1){
    if (listen(sockfd, 10) == 0) {
      printf(COLOR_GREEN"LOG: Listening on Socket.\n"COLOR_RESTORE);
    } else {
      printf(COLOR_RED"ERROR: Listening on Socket failed.\n"COLOR_RESTORE);
      exit(1);
    }

    addr_size = sizeof(new_addr);
    new_sock = accept(sockfd, (struct sockaddr *)&new_addr, &addr_size);
    get_filename(new_sock);
    printf("[+]Data sent successfully.\n");
  }
}