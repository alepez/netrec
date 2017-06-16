#include <chrono>
#include <fstream>
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

void printHex(std::ostream& os, const uint8_t* data, const size_t size) {
  char buf[3];
  auto dataEnd = data + size;

  for (auto b = data; b != dataEnd; ++b) {
    sprintf(buf, "%02x", *b);
    os << buf;
  }
}

void printTime(std::ostream& os, unsigned long t) {
  char str[17];
  sprintf(str, "%016lu", t);
  os << str;
}

int rec(const std::string& filename, const std::string& hostname, const int port) {
  std::cerr << "Recording " << filename << std::endl;

  std::ofstream os;
  os.open(filename);

  if (!os.is_open()) return die("Error opening file");

  /* socket: create the socket */
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);

  if (sockfd < 0) return die("ERROR opening socket");

  /* gethostbyname: get the server's DNS entry */
  hostent* server = gethostbyname(hostname.c_str());

  if (server == NULL) return die("No such host");

  sockaddr_in serveraddr;
  memset(&serveraddr, 0, sizeof(serveraddr));
  serveraddr.sin_family = AF_INET;

  memcpy(server->h_addr, &serveraddr.sin_addr.s_addr, server->h_length);
  serveraddr.sin_port = htons(port);

  /* connect: create a connection with the server */
  if (connect(sockfd, (sockaddr*)&serveraddr, sizeof(serveraddr)) < 0) return die("ERROR connecting");

  uint8_t buf[BUFSIZE];

  auto start = std::chrono::steady_clock::now();

  while (true) {
    int n = read(sockfd, buf, BUFSIZE);
    if (n < 0) return die("ERROR reading from socket");

    auto now = std::chrono::steady_clock::now();

    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count();

    printTime(os, elapsed);
    os << " ";
    printHex(os, buf, n);
    os << std::endl;
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
