#include <iostream>
#include <mutex>
#include <cstring>
#include <string>
#include <vector>
#include <sstream>
#include "client.hpp"

Client::Client() : connector{std::make_unique<Connector>(this)} {}

void Client::connect(const char * ip, unsigned int port) {
  try {
    connector->connect(ip, port);
    this->isConnected = true;
    std::thread parser = messageParser();
    parser.detach();
    users.push_back("(You)");
  } catch (std::system_error & e) {
    std::lock_guard<std::mutex> guard{cm};
    std::cout << "Could not connect to the server!" << std::endl;
  }
}

std::thread Client::messageParser() {
  return std::thread{[=] { connector->receiveMessage(); }};
}

void Client::sendMessage(std::string input) {
  try {
    connector->sendMessage(input.c_str());
  } catch (std::system_error & e) {
    std::lock_guard<std::mutex> guard{cm};
    std::cout << "Could not send the message (are you connected?)" << std::endl;
  }
}

void Client::sendNeptun(std::string input) {
  try {
    connector->sendNeptun(input.c_str());
  } catch (std::system_error & e) {
    std::lock_guard<std::mutex> guard{cm};
    std::cout << "Could not send the message (are you connected?)" << std::endl;
  }
}

void Client::sendPassword(std::string input) {
  try {
    connector->sendPassword(input.c_str());
  } catch (std::system_error & e) {
    std::lock_guard<std::mutex> guard{cm};
    std::cout << "Could not send the message (are you connected?)" << std::endl;
  }
}

void Client::parseString(std::string input) {
  if (input[0] == '/') {
    parseCommand(input.substr(1));
  } else {
    sendMessage(input);
  }
}

bool Client::checkStringStart(std::string haystack, std::string needle) {
  return haystack.size() >= needle.size() && haystack.substr(0, needle.size()).compare(needle) == 0;
}

void Client::parseCommand(std::string input) {
  if (checkStringStart(input, "quit")) {
    connector->disconnect();
    this->isConnected = false;
    this->quit = true;
    return;
  }

  if (checkStringStart(input, "disconnect")) {
    connector->disconnect();
    this->isConnected = false;
    return;
  }

  if (checkStringStart(input, "ping")) {
    connector->sendPing();
    return;
  }

  if (checkStringStart(input, "users")) {
    listUsers();
    return;
  }

  if (checkStringStart(input, "connect")) {
    char ip[256];
    unsigned int port;
    sscanf(input.c_str(), "%*s %[^:]:%u", ip, &port);
    connect(ip, port);
    return;
  }

  if (checkStringStart(input, "neptun")) {
    char neptun[7];
    sscanf(input.c_str(), "%*s %s", neptun);
    sendNeptun(neptun);
    return;
  }

  if (checkStringStart(input, "password")) {
    char password[7];
    sscanf(input.c_str(), "%*s %s", password);
    sendPassword(password);
    return;
  }

  std::lock_guard<std::mutex> guard{cm};
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
