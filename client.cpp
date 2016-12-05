#include <iostream>
#include <mutex>
#include <string>
#include <vector>
#include "client.hpp"

bool Client::connect() {
  std::string ip;
  unsigned int port;

  std::cout << "IP?" << std::endl;
  std::cin >> ip;
  std::cout << "Port?" << std::endl;
  std::cin >> port;
  return connector.connect(ip.c_str(), port);
}

void Client::login() {
  std::string neptun, password;
  std::cin >> neptun;
  connector.sendNeptun(neptun.c_str());
  std::cin >> password;
  connector.sendPassword(password.c_str());
}

void Client::parseMessages() {
  connector.receiveMessage();
}

std::thread Client::messageParser() {
  return std::thread{[=] { parseMessages(); }};
}

void Client::sendMessage(std::string input) {
  connector.sendMessage(input.c_str());
}

bool Client::parseString(std::string input) {
  if (input[0] == '/') {
    return parseCommand(input.substr(1));
  } else {
    sendMessage(input);
  }
  return false;
}

bool Client::parseCommand(std::string input) {
  if (input.compare("quit") == 0) {
    return true;
  }

  if (input.compare("listusers") == 0) {
    listUsers();
    return false;
  }

  std::cout << "Invalid command!" << std::endl;
  return false;
}

void Client::listUsers() {
  std::lock_guard<std::mutex> guard{cm};
  for (auto & user : users) {
    std::cout << user << std::endl;
  }
}
