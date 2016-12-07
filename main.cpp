#include <iostream>
#include <functional>
#include <string>
#include <thread>
#include "client.hpp"

int main(void) {
  Client client;

  std::string input;
  while (!client.isQuit()) {
    std::getline(std::cin, input);
    client.parseString(input);
  }
}
