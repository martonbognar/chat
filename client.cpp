#include <iostream>
#include <mutex>
#include <string>
#include <vector>
#include "client.hpp"

Client::Client() : connector{std::make_unique<Connector>(this)} {}

void Client::connect() {
  std::string ip;
  unsigned int port;

  std::cout << "IP?" << std::endl;
  std::cin >> ip;
  std::cout << "Port?" << std::endl;
  std::cin >> port;
  if (connector->connect(ip.c_str(), port)) {
    this->isConnected = true;
    std::thread parser = messageParser();
    parser.detach();
    users.push_back("(You)");
  }
}

void Client::login() {
  std::string neptun, password;
  std::cin >> neptun;
  connector->sendNeptun(neptun.c_str());
  std::cin >> password;
  connector->sendPassword(password.c_str());
}

std::thread Client::messageParser() {
  return std::thread{[=] { connector->receiveMessage(); }};
}

void Client::sendMessage(std::string input) {
  connector->sendMessage(input.c_str());
}

void Client::parseString(std::string input) {
  if (input[0] == '/') {
    parseCommand(input.substr(1));
  } else {
    sendMessage(input);
  }
}

void Client::parseCommand(std::string input) {
  if (input.compare("quit") == 0) {
    connector->logout();
    this->isConnected = false;
    this->quit = true;
    return;
  }

  if (input.compare("logout") == 0) {
    connector->logout();
    this->isConnected = false;
    return;
  }

  if (input.compare("ping") == 0) {
    connector->sendPing();
    return;
  }

  if (input.compare("listusers") == 0) {
    listUsers();
    return;
  }

  std::cout << "Invalid command!" << std::endl;
}

void Client::listUsers() {
  std::lock_guard<std::mutex> guard{cm};
  for (auto & user : users) {
    std::cout << user << std::endl;
  }
}

void Client::addUser(const char * name) {
  users.push_back(name);
}

void Client::removeUser(const char * name) {
  for (size_t i = 0; i < users.size(); ++i) {
    if (users[i].compare(name) == 0) {
      users.erase(users.begin() + i);
      return;
    }
  }
}
