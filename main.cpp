#include <iostream>
#include <functional>
#include <string>
#include <thread>
#include "client.hpp"

int main(void) {
  bool quit = false;
  Client client;
  quit = client.connect();

  if (!quit) {
    std::thread receiver = client.messageParser();
    receiver.detach();
  }

  client.login();

  std::string input;
  while (!quit && std::cin >> input) {
    quit = client.parseString(input);
  }
}
