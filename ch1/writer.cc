// writer.cpp
#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>

namespace {
const char kSocketPath[] = "/tmp/demo_socket";
constexpr char kMessage[] = "Hello from the writer process";
} // namespace

int main() {
  // Set up socket address
  struct sockaddr_un address;
  address.sun_family = AF_UNIX;
  strcpy(address.sun_path, kSocketPath);

  // Create socket
  auto server_fd = socket(AF_UNIX, SOCK_STREAM, 0);
  bind(server_fd, (struct sockaddr *)&address, sizeof(address));

  listen(server_fd, 1);

  // Accept a connection
  auto client_fd = accept(server_fd, NULL, NULL);
  send(client_fd, kMessage, strlen(kMessage), 0);

  // Cleanup
  close(client_fd);
  close(server_fd);
  unlink(kSocketPath);

  return 0;
}