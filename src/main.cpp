#include <iostream>
#include <string>

int die(const std::string& message) {
  std::cout << message << std::endl;
  return -1;
}

int play(const std::string& filename) {
  std::cerr << "Playing " << filename << std::endl;
  return 0;
}

int rec(const std::string& filename) {
  std::cerr << "Recording " << filename << std::endl;
  return 0;
}

int main(const int argc, const char** argv) {
  if (argc < 2) {
    return die("Missing argument");
  }

  std::string mode{argv[1]};
  std::string filename{argv[2]};

  if (mode == "rec") return rec(filename);

  if (mode == "play") return rec(filename);

  return die("Invalid command");
}
