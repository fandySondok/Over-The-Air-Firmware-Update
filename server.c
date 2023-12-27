#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_PORT 12345

#define BUFFER_SIZE 1024

int main()
{
  int server_socket, client_socket;
  struct sockaddr_in server_addr, client_addr;
  socklen_t client_addr_len = sizeof(client_addr);

  // Create a socket
  server_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (server_socket == -1)
  {
    perror("Socket creation failed");
    exit(EXIT_FAILURE);
  }

  // Set the SO_REUSEADDR option
  int opt = 1;
  if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
  {
    perror("Setsockopt failed");
    exit(EXIT_FAILURE);
  }

  // Configure server address
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(SERVER_PORT);
  server_addr.sin_addr.s_addr = INADDR_ANY;

  // Bind the socket
  if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
  {
    perror("Socket binding failed");
    exit(EXIT_FAILURE);
  }
  int l = 1;
  l = 5;

  // Listen for incoming connections
  if (listen(server_socket, 5) == -1)
  {
    perror("Socket listening failed");
    exit(EXIT_FAILURE);
  }

  printf("Server listening on port %d...\n", SERVER_PORT);

  while (1)
  {
    // Accept a client connection
    client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len);
    if (client_socket == -1)
    {
      perror("Client connection failed");
      continue; // Continue listening for other connections
    }

    // Receive the filename from the client
    char filename[BUFFER_SIZE];
    ssize_t filename_length = recv(client_socket, filename, sizeof(filename), 0);
    if (filename_length <= 0)
    {
      perror("Filename receiving error");
      close(client_socket);
      continue; // Continue listening for other connections
    }

    // Open the requested file
    FILE *file = fopen(filename, "rb");
    if (file == NULL)
    {
      close(client_socket);
      continue; // Continue listening for other connections
    }

    // Read and send the file data to the client
    printf("There is update file\n");
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), file)) > 0)
    {
      send(client_socket, buffer, bytes_read, 0);
    }

    // Close the file and the client socket
    fclose(file);
    close(client_socket);
  }

  // Close the server socket (never reached in this example)
  close(server_socket);

  return 0;
}
