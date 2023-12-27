#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_IP "127.0.0.1" // Replace with the server's IP address
#define SERVER_PORT 12345     // Replace with the server's port number

#define BUFFER_SIZE 1024

int main()
{
  int get_data = 0;
  int client_socket;
  struct sockaddr_in server_addr;
  char filename[] = "new_app"; // Change this to the filename you want to download
  char filename_req[] = "new_app_update";

tryagain:;
  // Create a socket
  client_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (client_socket == -1)
  {
    perror("Socket creation failed");
    exit(EXIT_FAILURE);
  }

  // Configure server address
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(SERVER_PORT);
  server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

  // Connect to the server

  if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
  {
    perror("Connection failed");
    exit(EXIT_FAILURE);
  }

  // Send the filename to the server
  send(client_socket, filename_req, strlen(filename_req), 0);

  // Receive and save the file data
  char buffer[BUFFER_SIZE];
  ssize_t bytes_received;
  FILE *file = fopen(filename, "wb");
  if (file == NULL)
  {
    perror("File opening failed");
    exit(EXIT_FAILURE);
  }
  while ((bytes_received = recv(client_socket, buffer, sizeof(buffer), 0)) > 0)
  {
    // Open a file to save the received data
    get_data = 1;
    fwrite(buffer, 1, bytes_received, file);
  }
  fclose(file);

  if (get_data == 0)
  {
    close(client_socket);
    sleep(1);
    goto tryagain;
  }
  else
  {
    printf("File downloaded successfully.\n");
    usleep(100000);
    char cmd[100];
    memset(cmd, 0x00, 100);
    snprintf(cmd, sizeof(cmd), "chmod +x %s", filename);
    system(cmd);
    memset(cmd, 0x00, 100);
    snprintf(cmd, sizeof(cmd), "rm %s", filename_req);
    system(cmd);
    memset(cmd, 0x00, 100);
    snprintf(cmd, sizeof(cmd), "pkill main");
    system(cmd);
    snprintf(cmd, sizeof(cmd), "mv %s main", filename);
    system(cmd);
    close(client_socket);
  }

  return 0;
}
