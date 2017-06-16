#include <iostream>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define BUFSIZE 1024

int die(const std::string& message) {
  std::cout << message << std::endl;
  return -1;
}

int play(const std::string& filename) {
  std::cerr << "Playing " << filename << std::endl;
  return 0;
}

std::string toHex(const uint8_t* buf, const size_t size) {
  char* str = new char[size * 2 + 1];
  const uint8_t* b = buf;
  char* s = str;
  for (size_t i = 0; i != size; ++i) {
    sprintf(s, "%02x", *b);
    b += 1;
    s += 2;
  }
  std::string result{str};
  delete[] str;
  return result;
}

int rec(const std::string& filename, const std::string& hostname, const int port) {
  std::cerr << "Recording " << filename << std::endl;

  /* socket: create the socket */
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);

  if (sockfd < 0) die("ERROR opening socket");

  /* gethostbyname: get the server's DNS entry */
  hostent* server = gethostbyname(hostname.c_str());

  if (server == NULL) die("No such host");

  sockaddr_in serveraddr;
  memset(&serveraddr, 0, sizeof(serveraddr));
  serveraddr.sin_family = AF_INET;

  memcpy(server->h_addr, &serveraddr.sin_addr.s_addr, server->h_length);
  serveraddr.sin_port = htons(port);

  /* connect: create a connection with the server */
  if (connect(sockfd, (sockaddr*)&serveraddr, sizeof(serveraddr)) < 0) die("ERROR connecting");

  uint8_t buf[BUFSIZE];

  while (true) {
    int n = read(sockfd, buf, BUFSIZE);
    if (n < 0) die("ERROR reading from socket");

    std::cerr << "RX " << n << " bytes\n";
    std::cerr << toHex(buf, n) << std::endl;
  }

  close(sockfd);
  return 0;
}

int main(const int argc, const char** argv) {
  if (argc < 2) {
    return die("Missing argument");
  }

  std::string mode{argv[1]};
  std::string filename{argv[2]};

  if (mode == "rec") return rec(filename, "localhost", 8080);

  if (mode == "play") return play(filename);

  return die("Invalid command");
}
