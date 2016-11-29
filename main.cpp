#include <iostream>
#include <string>
#include <thread>
#include "client.hpp"

int main(void) {
  bool quit = false;
  Client client;
  quit = client.connect();

  if (!quit) {
    std::thread messageParser(client.parseMessages());
    messageParser.join();
  }

  std::string input;
  while (std::cin >> input || quit) {
    std::cout << "Read input" << std::endl;
    quit = client.parseString(input);
  }
}
