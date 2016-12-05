#include <iostream>
#include <functional>
#include <string>
#include <thread>
#include "client.hpp"

int main(void) {
  Client client;

  client.connect();
  client.login();

  std::string input;
  while (!client.isQuit() && std::cin >> input) {
    client.parseString(input);
  }
}
