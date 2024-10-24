// reader.cpp
#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>

namespace {
constexpr char kSocketPath[] = "/tmp/demo_socket";
} // namespace

using namespace std;

int main() {
  // Set up socket address structure
  struct sockaddr_un address;
  address.sun_family = AF_UNIX;
  strcpy(address.sun_path, kSocketPath);

  // Create socket
  int sock = socket(AF_UNIX, SOCK_STREAM, 0);
  connect(sock, (struct sockaddr *)&address, sizeof(address));

  // Read message from server
  char buffer[1024] = {0};
  read(sock, buffer, sizeof(buffer));
  cout << "Message received from server: " << buffer << endl;

  // Cleanup
  close(sock);
  return 0;
}
